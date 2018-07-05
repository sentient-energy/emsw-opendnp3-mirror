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
#ifndef __INTEGRATION_TEST_H_
#define __INTEGRATION_TEST_H_

#include <APLTestTools/AsyncTestObjectASIO.h>
#include <APLTestTools/LogTester.h>
#include <APLTestTools/MockCommandAcceptor.h>
#include <APLTestTools/FanoutDataObserver.h>
#include <opendnp3/APL/Loggable.h>

#include <opendnp3/APL/FlexibleDataObserver.h>
#include <opendnp3/APL/Random.h>

#include <opendnp3/DNP3/AsyncStackManager.h>

#include "ComparingDataObserver.h"

#include <vector>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

namespace apl
{
namespace dnp
{

class IntegrationTest : private Loggable
{
public:

	IntegrationTest(Logger* apLogger, FilterLevel aLevel, boost::uint16_t aStartPort, size_t aNumPairs, size_t aNumPoints);

	size_t IncrementData();

	bool WaitForSameData(millis_t aTimeout, bool aDescribeAnyMissingData);

	AsyncStackManager* GetManager() {
		return &mManager;
	}

private:

	void InitLocalObserver();

	void ResetObservers();

	Binary RandomBinary();
	Analog RandomAnalog();
	Counter RandomCounter();

	Binary Next(const Binary& arPoint);
	Analog Next(const Analog& arPoint);
	Counter Next(const Counter& arPoint);


	void RegisterChange();
	void AddStackPair(FilterLevel aLevel, size_t aNumPoints);

	std::vector< boost::shared_ptr<ComparingDataObserver> > mMasterObservers;
	FanoutDataObserver<NullLock> mFanout;

	Random<boost::int32_t> mRandomInt32;
	Random<boost::uint32_t> mRandomUInt32;
	RandomBool mRandomBool;

	const boost::uint16_t M_START_PORT;

	FlexibleDataObserver mLocalFDO;
	MockCommandAcceptor mCmdAcceptor;

	AsyncStackManager mManager;
	size_t NUM_POINTS;
};

}
}

/* vim: set ts=4 sw=4: */

#endif

