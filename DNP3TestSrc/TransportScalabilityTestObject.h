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
#ifndef __TRANSPORT_SCALABILITY_TEST_OBJECT_H_
#define __TRANSPORT_SCALABILITY_TEST_OBJECT_H_

#include "TransportStackPair.h"
#include <APLTestTools/AsyncTestObjectASIO.h>

#include <opendnp3/APL/TimerSourceASIO.h>
#include <APLTestTools/LogTester.h>

namespace apl
{
namespace dnp
{

class TransportScalabilityTestObject : public LogTester, public AsyncTestObjectASIO
{
public:
	TransportScalabilityTestObject(
	        LinkConfig aClientCfg,
	        LinkConfig aServerCfg,
	        boost::uint16_t aPortStart,
	        boost::uint16_t aNumPair,
	        FilterLevel aLevel = LEV_INFO,
	        bool aImmediate = false);

	~TransportScalabilityTestObject();

	void Start();


	// Test helpers
	bool AllLayersUp();
	bool AllLayerReceived(size_t aNumBytes);
	bool AllLayerEqual(const boost::uint8_t*, size_t);

	void SendToAll(const boost::uint8_t*, size_t);

public:
	Logger* mpLogger;
	TimerSourceASIO mTimerSource;
	std::vector<TransportStackPair*> mPairs;
};

}
}

#endif
