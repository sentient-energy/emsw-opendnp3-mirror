//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//

#include "IntegrationTest.h"

#include <sstream>

#include <opendnp3/APL/PhysicalLayerFactory.h>
#include <opendnp3/APL/IPhysicalLayerAsync.h>

#include <APLTestTools/AsyncTestObjectASIO.h>

#include <opendnp3/DNP3/MasterStackConfig.h>
#include <opendnp3/DNP3/SlaveStackConfig.h>

#include <opendnp3/DNP3/MasterStack.h>
#include <opendnp3/DNP3/SlaveStack.h>

#include <boost/asio.hpp>
#include <boost/foreach.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace apl;
using namespace apl::dnp;

IntegrationTest::IntegrationTest(Logger* apLogger, FilterLevel aLevel, boost::uint16_t aStartPort, size_t aNumPairs, size_t aNumPoints) :
	Loggable(apLogger),
	M_START_PORT(aStartPort),
	mManager(apLogger),
	NUM_POINTS(aNumPoints)
{
	this->InitLocalObserver();

	for (size_t i = 0; i < aNumPairs; ++i) {
		AddStackPair(aLevel, aNumPoints);
	}
	mFanout.AddObserver(&mLocalFDO);
}

void IntegrationTest::InitLocalObserver()
{
	Transaction tr(&mLocalFDO);
	for (size_t i = 0; i < NUM_POINTS; ++i) {
		mLocalFDO.Update(this->RandomBinary(), i);
		mLocalFDO.Update(this->RandomAnalog(), i);
		mLocalFDO.Update(this->RandomCounter(), i);
	}
}

void IntegrationTest::ResetObservers()
{
	for (size_t i = 0; i < this->mMasterObservers.size(); ++i) {
		mMasterObservers[i]->Reset();
	}
}

bool IntegrationTest::WaitForSameData(millis_t aTimeout, bool aDescribeAnyMissingData)
{
	LOG_BLOCK(LEV_EVENT, "Wait for same data");

	for (size_t i = 0; i < this->mMasterObservers.size(); ++i) {
		ComparingDataObserver* pObs = mMasterObservers[i].get();
		if(!pObs->WaitForSameData(aTimeout)) {
			if(aDescribeAnyMissingData) pObs->DescribeMissingData();
			return false;
		}
	}

	return true;
}

size_t IntegrationTest::IncrementData()
{
	LOG_BLOCK(LEV_EVENT, "Incrementing data");

	size_t num = 0;

	this->ResetObservers();
	/*
	 * Resource Acquisition Is Initialization (RAII) Pattern.
	 * When the Transaction instance is created, it acquires the resource.
	 * When it is destroyed, it releases the resource.  The scoping using
	 * the {} block forces destruction of the Transaction at the right time.
	*/
	Transaction tr(&mFanout);
	for (size_t i = 0; i < NUM_POINTS; ++i) {
		mFanout.Update(this->Next(mLocalFDO.mBinaryMap[i]), i);
		mFanout.Update(this->Next(mLocalFDO.mAnalogMap[i]), i);
		mFanout.Update(this->Next(mLocalFDO.mCounterMap[i]), i);
		num += 3;
	}
	return num;
}

Binary IntegrationTest::RandomBinary()
{
	Binary v(mRandomBool.NextBool(), BQ_ONLINE);
	return v;
}

Analog IntegrationTest::RandomAnalog()
{
	Analog v(mRandomInt32.Next(), AQ_ONLINE);
	return v;
}

Counter IntegrationTest::RandomCounter()
{
	Counter v(mRandomUInt32.Next(), CQ_ONLINE);
	return v;
}

Binary IntegrationTest::Next(const Binary& arPoint)
{
	Binary point(!arPoint.GetValue(), arPoint.GetQuality());
	return point;
}

Analog IntegrationTest::Next(const Analog& arPoint)
{
	Analog point(arPoint.GetValue() + 1, arPoint.GetQuality());
	return point;
}

Counter IntegrationTest::Next(const Counter& arPoint)
{
	Counter point(arPoint.GetValue() + 1, arPoint.GetQuality());
	return point;
}

void IntegrationTest::AddStackPair(FilterLevel aLevel, size_t aNumPoints)
{
	boost::uint16_t port = M_START_PORT + static_cast<boost::uint16_t>(this->mMasterObservers.size());

	ostringstream oss;
	oss << "Port: " << port;
	std::string client = oss.str() + " Client ";
	std::string server = oss.str() + " Server ";

	boost::shared_ptr<ComparingDataObserver> pMasterFDO(new ComparingDataObserver(mpLogger->GetSubLogger(client), &mLocalFDO));
	mMasterObservers.push_back(pMasterFDO);

	PhysLayerSettings s(aLevel, 1000);
	this->mManager.AddTCPv4Client(client, s, TcpSettings("127.0.0.1", port));
	this->mManager.AddTCPv4Server(server, s, TcpSettings("127.0.0.1", port));

	/*
	 * Add a Master instance.  The code is wrapped in braces so that we can
	 * re-use the 'cfg' variable name.
	 */
	{
		MasterStackConfig cfg;
		cfg.app.RspTimeout = 20000;
		cfg.master.IntegrityRate = 60000;	// set this to retry, if the task
		// timer doesn't close properly,
		// this will seal the deal
		cfg.master.EnableUnsol = true;
		cfg.master.DoUnsolOnStartup = true;
		cfg.master.UnsolClassMask = PC_ALL_EVENTS;
		this->mManager.AddMaster(client, client, aLevel, pMasterFDO.get(), cfg);
	}

	/*
	 * Add a Slave instance.  The code is wrapped in braces so that we can
	 * re-use the 'cfg' variable name.
	 */
	{
		SlaveStackConfig cfg;
		cfg.app.RspTimeout = 20000;
		cfg.slave.mDisableUnsol = false;
		cfg.slave.mUnsolPackDelay = 0;
		cfg.device = DeviceTemplate(aNumPoints, aNumPoints, aNumPoints);
		IDataObserver* pObs = this->mManager.AddSlave(server, server, aLevel, &mCmdAcceptor, cfg);
		this->mFanout.AddObserver(pObs);
	}

}

/* vim: set ts=4 sw=4: */

