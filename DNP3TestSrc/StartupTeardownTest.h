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
#ifndef __STARTUP_TEARDOWN_TEST_H_
#define __STARTUP_TEARDOWN_TEST_H_

#include <opendnp3/APL/Log.h>
#include <opendnp3/APL/FlexibleDataObserver.h>
#include <opendnp3/DNP3/AsyncStackManager.h>


namespace boost
{
namespace asio
{
class io_service;
}
}

namespace apl
{
class IPhysicalLayerAsync;
}

namespace apl
{
namespace dnp
{

class StartupTeardownTest
{
public:

	StartupTeardownTest(FilterLevel aLevel, bool aImmediate = false);

	void CreatePort(const std::string& arName, FilterLevel aLevel);
	void AddMaster(const std::string& arName, const std::string& arPortName, boost::uint16_t aLocalAddress, FilterLevel aLevel);

	EventLog log;
	AsyncStackManager manager;
	FlexibleDataObserver fdo;
};

}
}

#endif

