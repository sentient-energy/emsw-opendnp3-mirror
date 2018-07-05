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
#ifndef __TRANSPORT_STACK_PAIR_H_
#define __TRANSPORT_STACK_PAIR_H_

namespace boost
{
namespace asio
{
class io_service;
}
}


#include <opendnp3/APL/PhysicalLayerAsyncTCPv4Client.h>
#include <opendnp3/APL/PhysicalLayerAsyncTCPv4Server.h>
#include <opendnp3/APL/ITimerSource.h>

#include "TransportIntegrationStack.h"

namespace apl
{
namespace dnp
{

class TransportStackPair
{
public:
	TransportStackPair(
	        LinkConfig aClientCfg,
	        LinkConfig aServerCfg,
	        Logger* apLogger,
	        boost::asio::io_service* apService,
	        ITimerSource* apTimerSrc,
	        boost::uint16_t aPort);

	void Start();

	//test helper functions
	bool BothLayersUp();

public:
	PhysicalLayerAsyncTCPv4Client mClient;
	PhysicalLayerAsyncTCPv4Server mServer;

	TransportIntegrationStack mClientStack;
	TransportIntegrationStack mServerStack;

};

}
}

#endif
