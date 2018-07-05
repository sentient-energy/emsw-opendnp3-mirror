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
#include "VtoIntegrationTestBase.h"

#include <opendnp3/APL/LogToStdio.h>

#include <opendnp3/DNP3/SlaveStackConfig.h>
#include <opendnp3/DNP3/MasterStackConfig.h>
#include <opendnp3/DNP3/VtoRouterSettings.h>

/** Platforms have different reserved port ranges */

namespace apl
{
namespace dnp
{


VtoIntegrationTestBase::VtoIntegrationTestBase(
    bool clientOnSlave,
    bool aImmediateOutput,
    bool aLogToFile,
    FilterLevel level,
    boost::uint16_t port) :

	LogTester(),
	Loggable(mpTestLogger),
	mpMainLogger(mLog.GetLogger(level, "main")),
	mpLtf(aLogToFile ? new LogToFile(&mLog, "integration.log", true) : NULL),
	testObj(),
	timerSource(testObj.GetService()),
	vtoClient(mLog.GetLogger(level, "local-tcp-client"), testObj.GetService(), TcpSettings("127.0.0.1", port + 20)),
	vtoServer(mLog.GetLogger(level, "loopback-tcp-server"), testObj.GetService(), TcpSettings("0.0.0.0", port + 10)),
	manager(mLog.GetLogger(level, "manager")),
	tcpPipe(mLog.GetLogger(level,  "pipe"), manager.GetIOService(), port)
{

	if(aImmediateOutput) mLog.AddLogSubscriber(LogToStdio::Inst());

	{
	manager.AddPhysicalLayer("dnp-tcp-server", PhysLayerSettings(), &tcpPipe.server);
	SlaveStackConfig config;
	config.app.NumRetry = 3;
	config.app.RspTimeout = 500;
	manager.AddSlave("dnp-tcp-server", "slave", level, &cmdAcceptor, config);
	}

	{
	manager.AddPhysicalLayer("dnp-tcp-client", PhysLayerSettings(), &tcpPipe.client);
	MasterStackConfig config;
	config.app.NumRetry = 3;
	config.app.RspTimeout = 500;
	config.master.UseNonStandardVtoFunction = true;
	manager.AddMaster("dnp-tcp-client", "master", level, &fdo, config);
	}

	// switch if master or slave gets the loopback half of the server

	std::string clientSideOfStack = clientOnSlave ? "slave" : "master";
	std::string serverSideOfStack = clientOnSlave ? "master" : "slave";

	manager.AddTCPv4Client("vto-tcp-client", PhysLayerSettings(), TcpSettings("localhost", port + 10));
	manager.StartVtoRouter("vto-tcp-client", clientSideOfStack, VtoRouterSettings(88, false, false, 1000));
	manager.AddTCPv4Server("vto-tcp-server", PhysLayerSettings(), TcpSettings("localhost", port + 20));
	manager.StartVtoRouter("vto-tcp-server", serverSideOfStack, VtoRouterSettings(88, true, false, 1000));
}

VtoIntegrationTestBase::~VtoIntegrationTestBase()
{
	manager.Shutdown();
}

}

}

/* vim: set ts=4 sw=4: */
