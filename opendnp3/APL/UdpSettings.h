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
#ifndef __UDP_SETTINGS_H_
#define __UDP_SETTINGS_H_

#include <string>
#include <boost/cstdint.hpp>

namespace apl
{

struct UdpSettings {
	UdpSettings() :
		mAddress(),
		mPort(0),
		mSendBufferSize(0),
		mRecvBufferSize(0)
	{
	}

	UdpSettings(const std::string& aAddress, boost::uint16_t aPort) :
		mAddress(aAddress),
		mPort(aPort),
		mSendBufferSize(0),
		mRecvBufferSize(0)
	{
	}

	std::string mAddress; // For server, this is the endpoint
	boost::uint16_t mPort;
	size_t mSendBufferSize; // 0 indicates to use system default
	size_t mRecvBufferSize; // 0 indicates to use system default
};

}

#endif

