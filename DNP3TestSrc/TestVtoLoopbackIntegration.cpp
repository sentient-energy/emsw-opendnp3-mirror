/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership.  Green Enery
 * Corp licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <boost/test/unit_test.hpp>

#include <opendnp3/APL/PhysLoopback.h>
#include <opendnp3/APL/RandomizedBuffer.h>

#include <APLTestTools/MockPhysicalLayerMonitor.h>

#include "VtoIntegrationTestBase.h"

using namespace apl;
using namespace apl::dnp;

class VtoLoopbackTestStack : public VtoIntegrationTestBase
{
public:
	VtoLoopbackTestStack(
	    bool clientOnSlave = true,
	    bool aImmediateOutput = false,
	    bool aLogToFile = false,
	    FilterLevel level = LEV_INFO,
	    boost::uint16_t port = MACRO_PORT_VALUE) :

		VtoIntegrationTestBase(clientOnSlave, aImmediateOutput, aLogToFile, level, port),
		loopback(mLog.GetLogger(level, "loopback"), &vtoServer, &timerSource),
		local(mLog.GetLogger(level, "mock-client-connection"), &vtoClient, &timerSource, 500) {
	}

	virtual ~VtoLoopbackTestStack() {
		local.Shutdown();
		loopback.Shutdown();
	}

	bool WaitForLocalState(PhysicalLayerState aState, millis_t aTimeout = 30000) {
		return testObj.ProceedUntil(boost::bind(&MockPhysicalLayerMonitor::NextStateIs, &local, aState), aTimeout);
	}

	bool WaitForExpectedDataToBeReceived(millis_t aTimeout = 30000) {
		return testObj.ProceedUntil(boost::bind(&MockPhysicalLayerMonitor::AllExpectedDataHasBeenReceived, &local), aTimeout);
	}

	PhysLoopback loopback;
	MockPhysicalLayerMonitor local;
};


BOOST_AUTO_TEST_SUITE(VtoLoopbackIntegrationSuite)

void TestLargeDataLoopback(VtoLoopbackTestStack& arTest, size_t aSizeInBytes)
{
	// start everything
	arTest.loopback.Start();
	arTest.local.Start();
	BOOST_REQUIRE(arTest.WaitForLocalState(PLS_OPEN));

	// test that a large set of data flowing one way works
	CopyableBuffer data(aSizeInBytes);
	for(size_t i = 0; i < data.Size(); ++i) data[i] = static_cast<boost::uint8_t>(i % (0xAA));

	arTest.local.ExpectData(data);
	arTest.local.WriteData(data);
	BOOST_REQUIRE(arTest.WaitForExpectedDataToBeReceived(60000));

	// this will cause an exception if we receive any more data beyond what we wrote
	arTest.testObj.ProceedForTime(1000);
}

#define MACRO_BUFFER_SIZE 1<<20 // 1 << 20 == 1MB, 1<<24 == 16MB

BOOST_AUTO_TEST_CASE(LargeDataLoopbackMasterWritesSlaveEchoes)
{
	VtoLoopbackTestStack stack(true, false);	
	stack.tcpPipe.client.SetCorruptionProbability(0.005);
	stack.tcpPipe.server.SetCorruptionProbability(0.005);
	TestLargeDataLoopback(stack, MACRO_BUFFER_SIZE);
}

BOOST_AUTO_TEST_CASE(LargeDataLoopbackSlaveWritesMasterEchoes)
{
	VtoLoopbackTestStack stack(false, false);
	stack.tcpPipe.client.SetCorruptionProbability(0.005);
	stack.tcpPipe.server.SetCorruptionProbability(0.005);
	TestLargeDataLoopback(stack, MACRO_BUFFER_SIZE);
}

BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
