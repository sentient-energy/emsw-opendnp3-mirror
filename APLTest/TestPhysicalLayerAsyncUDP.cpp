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

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/test/unit_test.hpp>

#include <opendnp3/APL/LogToStdio.h>
#include <opendnp3/APL/TimerSourceASIO.h>
#include <opendnp3/APL/Exception.h>
#include <opendnp3/APL/IOServiceThread.h>

#include <APLTestTools/TestHelpers.h>
#include <APLTestTools/BufferHelpers.h>
#include <APLTestTools/AsyncTestObjectASIO.h>
#include <APLTestTools/AsyncPhysTestObject.h>

#include <iostream>

using namespace apl;
using namespace boost;


BOOST_AUTO_TEST_SUITE(PhysicalLayerAsyncUDPSuite)

BOOST_AUTO_TEST_CASE(TestStateClosed)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	uint8_t buff[100];

	// Test that reads/writes of length 0 throw ArgumentException
	BOOST_REQUIRE_THROW(t.mUDPClient.AsyncWrite(buff,0), ArgumentException);
	BOOST_REQUIRE_THROW(t.mUDPClient.AsyncRead(buff,0), ArgumentException);

	//Test that in the closed state we get the proper invalid state exceptions
	BOOST_REQUIRE_THROW(t.mUDPClient.AsyncWrite(buff,100), InvalidStateException);
	BOOST_REQUIRE_THROW(t.mUDPClient.AsyncRead(buff,100), InvalidStateException);
	BOOST_REQUIRE_THROW(t.mUDPClient.AsyncClose(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(OpenClose)
{
	AsyncPhysTestObject t(LEV_INFO, false);

	t.mUDPServer.AsyncOpen();
	BOOST_REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUDPServerUpper)));

	t.mUDPServer.AsyncClose();
	BOOST_REQUIRE(t.ProceedUntilFalse(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUDPServerUpper)));

	t.mUDPClient.AsyncOpen();
	BOOST_REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUDPClientUpper)));

	t.mUDPClient.AsyncClose();
	BOOST_REQUIRE(t.ProceedUntilFalse(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUDPClientUpper)));
}

BOOST_AUTO_TEST_CASE(TwoWaySend)
{
	// const size_t SEND_SIZE = 1 << 20; // 1 MB
	const size_t SEND_SIZE = 65507;

	AsyncPhysTestObject t(LEV_INFO, false);

	t.mUDPServer.AsyncOpen();
	t.mUDPClient.AsyncOpen();
	BOOST_REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUDPServerUpper)));
	BOOST_REQUIRE(t.ProceedUntil(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUDPClientUpper)));

	//both layers are now up and reading, start them both writing
	ByteStr bs(SEND_SIZE, 77); //give some interesting seed value to make sure bytes are correctly written

	t.mUDPClientUpper.SendDown(bs.Buffer(), bs.Size());
	BOOST_REQUIRE(t.ProceedUntil(boost::bind(&MockUpperLayer::SizeEquals, &t.mUDPServerUpper, SEND_SIZE)));
	BOOST_REQUIRE(t.mUDPServerUpper.BufferEquals(bs.Buffer(), bs.Size()));

	t.mUDPServerUpper.SendDown(bs.Buffer(), bs.Size());
	BOOST_REQUIRE(t.ProceedUntil(boost::bind(&MockUpperLayer::SizeEquals, &t.mUDPClientUpper, SEND_SIZE)));
	BOOST_REQUIRE(t.mUDPClientUpper.BufferEquals(bs.Buffer(), bs.Size()));

	t.mUDPServer.AsyncClose(); //stop both sides (UDP does not sending close messages)
	t.mUDPClient.AsyncClose();
	BOOST_REQUIRE(t.ProceedUntilFalse(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUDPServerUpper)));
	BOOST_REQUIRE(t.ProceedUntilFalse(bind(&MockUpperLayer::IsLowerLayerUp, &t.mUDPClientUpper)));
}

BOOST_AUTO_TEST_SUITE_END()
