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

#include <opendnp3/APL/Log.h>
#include <opendnp3/APL/Exception.h>
#include <opendnp3/APL/LogToStdio.h>

#include <opendnp3/DNP3/VtoRouterManager.h>
#include <opendnp3/DNP3/VtoRouterSettings.h>
#include <opendnp3/DNP3/VtoWriter.h>
#include <opendnp3/DNP3/VtoReader.h>

#include <APLTestTools/MockTimerSource.h>
#include <APLTestTools/MockPhysicalLayerSource.h>
#include <APLTestTools/MockPhysicalLayerAsync.h>

using namespace apl;
using namespace apl::dnp;

class TestObject
{
public:
	TestObject(FilterLevel aLevel = LEV_INFO, bool aImmediate = false) :
		log(),
		mts(),
		mpls(log.GetLogger(aLevel, "source"), &mts),
		mgr(log.GetLogger(aLevel, "vto"), &mts, &mpls),
		writer(log.GetLogger(aLevel, "writer"), 100),
		reader(log.GetLogger(aLevel, "reader")) {
		if(aImmediate) log.AddLogSubscriber(LogToStdio::Inst());
	}

	EventLog log;
	MockTimerSource mts;
	MockPhysicalLayerSource mpls;
	VtoRouterManager mgr;
	VtoWriter writer;
	VtoReader reader;
};

BOOST_AUTO_TEST_SUITE(VtoRouterManagerSuite)

BOOST_AUTO_TEST_CASE(Construction)
{
	TestObject t;
}

BOOST_AUTO_TEST_CASE(ManagerCreatesRouterAndStartsIt)
{
	TestObject t;
	t.mgr.StartRouter("port", VtoRouterSettings(1, true, false), &t.writer, &t.reader);
	MockPhysicalLayerAsync* pMock = t.mpls.GetMock("port");
	BOOST_REQUIRE(pMock != NULL);
	BOOST_REQUIRE(pMock->IsOpening());
}

BOOST_AUTO_TEST_CASE(StoppingUnknownRouterExcepts)
{
	TestObject t;
	t.mgr.StartRouter("port", VtoRouterSettings(1, true, false), &t.writer, &t.reader);
	BOOST_REQUIRE_THROW(t.mgr.StopRouter(&t.writer, 2), ArgumentException);

}

BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
