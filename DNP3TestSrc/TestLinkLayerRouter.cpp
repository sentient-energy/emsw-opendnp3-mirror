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
#include <boost/bind.hpp>
#include <APLTestTools/TestHelpers.h>

#include <opendnp3/APL/Exception.h>
#include <opendnp3/APL/ToHex.h>

#include <opendnp3/DNP3/LinkRoute.h>

#include "LinkLayerRouterTest.h"
#include "MockFrameSink.h"

using namespace apl;
using namespace apl::dnp;

BOOST_AUTO_TEST_SUITE(LinkLayerRouterSuite)

// Test that send frames from unknown sources are rejected
BOOST_AUTO_TEST_CASE(UnknownSourceException)
{
	LinkLayerRouterTest t;
	LinkFrame f;
	f.FormatAck(true, false, 1, 2);
	BOOST_REQUIRE_THROW(t.router.Transmit(f), ArgumentException);
}

// Test that frames with unknown destinations are correctly logged
BOOST_AUTO_TEST_CASE(UnknownDestination)
{
	LinkLayerRouterTest t;

	MockFrameSink mfs;
	t.router.AddContext(&mfs, LinkRoute(1, 1024));
	t.phys.SignalOpenSuccess();

	t.phys.TriggerRead("05 64 05 C0 01 00 00 04 E9 21");
	LogEntry le;
	BOOST_REQUIRE(t.GetNextEntry(le));
	BOOST_REQUIRE_EQUAL(le.GetErrorCode(), DLERR_UNKNOWN_ROUTE);
	int address;
	BOOST_REQUIRE(le.GetValue("DESTINATION", address));
	BOOST_REQUIRE_EQUAL(1, address);
	BOOST_REQUIRE(le.GetValue("SOURCE", address));
	BOOST_REQUIRE_EQUAL(1024, address);
}

// Test that the router rejects sends until it is online
BOOST_AUTO_TEST_CASE(LayerNotOnline)
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	t.router.AddContext(&mfs, LinkRoute(1, 1024));
	LinkFrame f;
	f.FormatAck(true, false, 1, 1024);
	BOOST_REQUIRE_THROW(t.router.Transmit(f), InvalidStateException);
}

// Test that the router rejects sends until it is online
BOOST_AUTO_TEST_CASE(AutomaticallyClosesWhenAllContextsAreRemoved)
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	t.router.AddContext(&mfs, LinkRoute(1, 1024));
	BOOST_REQUIRE_EQUAL(PLS_OPENING, t.router.GetState());
	t.router.RemoveContext(LinkRoute(1, 1024));
	BOOST_REQUIRE_EQUAL(PLS_OPENING, t.router.GetState());
	t.phys.SignalOpenFailure();
	BOOST_REQUIRE_EQUAL(PLS_CLOSED, t.router.GetState());
}

/// Test that router is correctly clears the send buffer on close
BOOST_AUTO_TEST_CASE(CloseBehavior)
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	t.router.AddContext(&mfs, LinkRoute(1, 1024));
	t.phys.SignalOpenSuccess();
	LinkFrame f;
	f.FormatAck(true, false, 1, 1024);
	t.router.Transmit(f); // puts the router in the send state
	BOOST_REQUIRE_EQUAL(t.phys.NumWrites(), 1);
	t.phys.AsyncClose(); //we're both reading and writing so this doesn't trigger a callback yet
	BOOST_REQUIRE(mfs.mLowerOnline);
	t.phys.SignalSendFailure();
	BOOST_REQUIRE(mfs.mLowerOnline);
	t.phys.SignalReadFailure();

	// now the layer should go offline, this should clear the transmit queue,
	BOOST_REQUIRE_FALSE(mfs.mLowerOnline);

	t.phys.ClearBuffer();

	// Sleep the minimum retry timeout used by the router and dispatch the open
	// retry timer to trigger restart
	sleep(1);
	t.mts.Dispatch(1);
	t.phys.SignalOpenSuccess();

	LinkFrame f2; f2.FormatAck(true, false, 1, 1024);
	t.router.Transmit(f2);
	BOOST_REQUIRE_EQUAL(t.phys.NumWrites(), 2);
	BOOST_REQUIRE(t.phys.BufferEquals(f2.GetBuffer(), f2.GetSize()));
	t.phys.SignalSendSuccess();
	BOOST_REQUIRE_EQUAL(t.phys.NumWrites(), 2);
}


BOOST_AUTO_TEST_CASE(ReentrantCloseWorks)
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	t.router.AddContext(&mfs, LinkRoute(1, 1024));
	t.phys.SignalOpenSuccess();
	BOOST_REQUIRE(mfs.mLowerOnline);
	mfs.AddAction(boost::bind(&LinkLayerRouter::Shutdown, &t.router));
	LinkFrame f; f.FormatAck(true, false, 1024, 1);
	t.phys.TriggerRead(toHex(f.GetBuffer(), f.GetSize()));
	BOOST_REQUIRE(t.IsLogErrorFree());
}

/// Test that the second bind fails when a non-unique address is added
BOOST_AUTO_TEST_CASE(MultiAddressBindError)
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	t.router.AddContext(&mfs, LinkRoute(1, 1024));
	BOOST_REQUIRE_THROW(t.router.AddContext(&mfs, LinkRoute(1, 1024)), ArgumentException);
}

/// Test that the second bind fails when a non-unique context is added
BOOST_AUTO_TEST_CASE(MultiContextBindError)
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	t.router.AddContext(&mfs, LinkRoute(1, 1024));
	BOOST_REQUIRE_THROW(t.router.AddContext(&mfs, LinkRoute(1, 2048)), ArgumentException);
}

/// Test that router correctly buffers and sends frames from multiple contexts
BOOST_AUTO_TEST_CASE(MultiContextSend)
{
	LinkLayerRouterTest t;
	MockFrameSink mfs1;
	MockFrameSink mfs2;
	t.router.AddContext(&mfs1, LinkRoute(1, 1024));
	t.router.AddContext(&mfs2, LinkRoute(1, 2048));
	LinkFrame f1; f1.FormatAck(true, false, 1, 1024);
	LinkFrame f2; f2.FormatAck(true, false, 1, 2048);
	t.phys.SignalOpenSuccess();
	t.router.Transmit(f1);
	t.router.Transmit(f2);
	BOOST_REQUIRE_EQUAL(t.phys.NumWrites(), 1);
	t.phys.SignalSendSuccess();
	BOOST_REQUIRE_EQUAL(t.phys.NumWrites(), 2);
	t.phys.SignalSendSuccess();
	BOOST_REQUIRE_EQUAL(t.phys.NumWrites(), 2);
}
BOOST_AUTO_TEST_SUITE_END()
