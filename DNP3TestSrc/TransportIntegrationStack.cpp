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
#include "TransportIntegrationStack.h"

#include <opendnp3/DNP3/LinkRoute.h>

namespace apl
{
namespace dnp
{

TransportIntegrationStack::TransportIntegrationStack(Logger* apLogger, ITimerSource* apTimerSrc, IPhysicalLayerAsync* apPhys, LinkConfig aCfg) :
	mRouter(apLogger, "integration", apPhys, apTimerSrc, 1000),
	mLink(apLogger, apTimerSrc, aCfg),
	mTransport(apLogger),
	mUpper(apLogger)
{
	LinkRoute route(aCfg.RemoteAddr, aCfg.LocalAddr);
	mRouter.AddContext(&mLink, route);
	mLink.SetUpperLayer(&mTransport);
	mTransport.SetUpperLayer(&mUpper);
	mLink.SetRouter(&mRouter);
}


}
}
