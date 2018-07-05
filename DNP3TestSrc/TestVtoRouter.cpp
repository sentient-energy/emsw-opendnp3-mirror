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

#include <boost/test/unit_test.hpp>

#include <opendnp3/APL/Log.h>
#include <opendnp3/APL/ToHex.h>

#include <APLTestTools/LogTester.h>
#include <APLTestTools/MockPhysicalLayerAsync.h>
#include <APLTestTools/MockTimerSource.h>
#include <APLTestTools/TestHelpers.h>

#include <opendnp3/DNP3/AlwaysOpeningVtoRouter.h>
#include <opendnp3/DNP3/VtoRouterSettings.h>

#include "ReadableVtoWriter.h"

using namespace std;
using namespace apl;
using namespace apl::dnp;

class RouterTestClass : LogTester
{
public:
	RouterTestClass(const VtoRouterSettings& arSettings = VtoRouterSettings(0, true, true), const size_t aWriterSize = 100) :
		LogTester(false),
		phys(mLog.GetLogger(LEV_DEBUG, "phys")),
		writer(mLog.GetLogger(LEV_DEBUG, "writer"), aWriterSize),
		mts(),
		router(arSettings, mLog.GetLogger(LEV_DEBUG, "router"), &writer, &phys, &mts) {
		writer.AddVtoCallback(&router);
	}

	MockPhysicalLayerAsync phys;
	ReadableVtoWriter writer;
	MockTimerSource mts;
	AlwaysOpeningVtoRouter router;
};

BOOST_AUTO_TEST_SUITE(VtoRouterTests)

boost::uint8_t data[3] = { 0xA, 0xB, 0xC };
VtoData vtoData(data, 3);

void CheckVtoEvent(const VtoEvent& arEvent, const std::string& arData, boost::uint8_t aChannelId, PointClass aClass)
{
	BOOST_REQUIRE_EQUAL(aChannelId, arEvent.mIndex); // the channel id
	BOOST_REQUIRE_EQUAL(aClass, arEvent.mClass);

	const std::string hex = toHex(arEvent.mValue.mpData, arEvent.mValue.GetSize(), true);

	BOOST_REQUIRE_EQUAL(arData, hex);
}

BOOST_AUTO_TEST_CASE(Construction)
{
	RouterTestClass rtc;
}

BOOST_AUTO_TEST_CASE(StartsReadingAfterOpen)
{
	RouterTestClass rtc;
	BOOST_REQUIRE(rtc.phys.IsOpening());
	rtc.phys.SignalOpenSuccess();
	BOOST_REQUIRE(rtc.phys.IsReading());
}

BOOST_AUTO_TEST_CASE(WriteVtoBeforeConnect)
{
	RouterTestClass rtc;
	BOOST_REQUIRE(rtc.phys.IsOpening());
	rtc.router.OnVtoDataReceived(vtoData);

	/* When physical layer comes up, it should read and write */
	rtc.phys.SignalOpenSuccess();
	BOOST_REQUIRE(rtc.phys.IsReading());
	BOOST_REQUIRE(rtc.phys.IsWriting());

	BOOST_REQUIRE_EQUAL(1, rtc.phys.NumWrites());
	BOOST_REQUIRE(rtc.phys.BufferEquals("0A 0B 0C"));
	rtc.phys.SignalSendSuccess();
	BOOST_REQUIRE_EQUAL(1, rtc.phys.NumWrites());
}

BOOST_AUTO_TEST_CASE(WriteVtoAfterConnect)
{
	RouterTestClass rtc;
	BOOST_REQUIRE(rtc.phys.IsOpening());

	/* When physical layer comes up, it should read and write */
	rtc.phys.SignalOpenSuccess();
	BOOST_REQUIRE(rtc.phys.IsReading());
	BOOST_REQUIRE(rtc.phys.IsOpen());
	rtc.router.OnVtoDataReceived(vtoData);

	BOOST_REQUIRE(rtc.phys.IsWriting());

	BOOST_REQUIRE_EQUAL(1, rtc.phys.NumWrites());
	BOOST_REQUIRE(rtc.phys.BufferEquals("0A 0B 0C"));
	rtc.phys.SignalSendSuccess();
	BOOST_REQUIRE_EQUAL(1, rtc.phys.NumWrites());
}

BOOST_AUTO_TEST_CASE(WriteVtoData)
{
	RouterTestClass rtc(VtoRouterSettings(8, true, true));
	rtc.phys.SignalOpenSuccess();

	std::string stringData("0A 0B 0C");

	rtc.phys.TriggerRead(stringData);
	BOOST_REQUIRE_EQUAL(rtc.writer.Size(), 1);
	VtoEvent vto;
	BOOST_REQUIRE(rtc.writer.Read(vto));
	CheckVtoEvent(vto, stringData, 8, PC_CLASS_1);
}

BOOST_AUTO_TEST_CASE(PhysReadBuffering)
{
	RouterTestClass rtc(VtoRouterSettings(0, true, true), 1); // writer only takes 1 chunk!
	rtc.phys.SignalOpenSuccess();

	std::string stringData1("0A 0B 0C");
	std::string stringData2("0D 0E 0F");
	std::string stringData3("10 11 12");

	rtc.phys.TriggerRead(stringData1);
	rtc.phys.TriggerRead(stringData2);
	rtc.phys.TriggerRead(stringData3);
	BOOST_REQUIRE_EQUAL(rtc.writer.Size(), 1);

	VtoEvent vto;
	BOOST_REQUIRE(rtc.writer.Read(vto));
	CheckVtoEvent(vto, stringData1, 0, PC_CLASS_1);

	BOOST_REQUIRE_EQUAL(1, rtc.writer.Size());

	// the 2nd set of data will get merged as a single vto object
	BOOST_REQUIRE(rtc.writer.Read(vto));
	BOOST_REQUIRE_EQUAL(1, rtc.writer.Size());
	CheckVtoEvent(vto, "0D 0E 0F", 0, PC_CLASS_1);
	BOOST_REQUIRE_EQUAL(1, rtc.writer.Size());
	BOOST_REQUIRE(rtc.writer.Read(vto));
	CheckVtoEvent(vto, "10 11 12", 0, PC_CLASS_1);

	BOOST_REQUIRE_EQUAL(rtc.writer.Size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
