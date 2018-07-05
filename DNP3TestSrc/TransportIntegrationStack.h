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
#ifndef __TRANSPORT_INTEGRATION_STACK_H_
#define __TRANSPORT_INTEGRATION_STACK_H_

#include <opendnp3/DNP3/LinkLayerRouter.h>
#include <opendnp3/DNP3/LinkLayer.h>
#include <opendnp3/DNP3/TransportLayer.h>

#include <APLTestTools/MockUpperLayer.h>

namespace apl
{

class Logger;
class ITimerSource;
class IPhysicalLayerAsync;

}

namespace apl
{
namespace dnp
{

class TransportIntegrationStack
{
public:
	TransportIntegrationStack(Logger*, ITimerSource*, IPhysicalLayerAsync*, LinkConfig);

	LinkLayerRouter mRouter;
	LinkLayer mLink;
	TransportLayer mTransport;
	MockUpperLayer mUpper;
};

}
}

#endif
