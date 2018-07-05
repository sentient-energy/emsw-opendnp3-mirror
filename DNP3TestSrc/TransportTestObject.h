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
#ifndef __TRANSPORT_TEST_OBJECT_H_
#define __TRANSPORT_TEST_OBJECT_H_

#include <opendnp3/APL/Log.h>

#include <vector>
#include <string>

#include <APLTestTools/MockLowerLayer.h>
#include <APLTestTools/MockUpperLayer.h>
#include <opendnp3/DNP3/TransportLayer.h>
#include <APLTestTools/LogTester.h>

namespace apl
{
namespace dnp
{

class TransportTestObject : public LogTester
{
public:
	TransportTestObject(bool aOpenOnStart = false, FilterLevel aLevel = LEV_INFO, bool aImmediate = false);

	// Generate a complete packet sequence inside the vector and
	// return the corresponding reassembled APDU
	std::string GeneratePacketSequence(std::vector<std::string>&, size_t aNumPackets, size_t aLastPacketLength);

	// Get a Sequence of data w/ optional header
	std::string GetData(const std::string& arHdr, boost::uint8_t aSeed = 0, size_t aLength = TL_MAX_TPDU_PAYLOAD);

private:
	Logger* mpLogger;
	TransportLayer transport;

public:

	MockLowerLayer lower;
	MockUpperLayer upper;


};

}
}

#endif
