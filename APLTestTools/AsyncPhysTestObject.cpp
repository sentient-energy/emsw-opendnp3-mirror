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

#include "AsyncPhysTestObject.h"
#include <opendnp3/APL/TcpSettings.h>

namespace apl
{

AsyncPhysTestObject::AsyncPhysTestObject(FilterLevel aLevel, bool aImmediate, bool aAutoRead) :
	AsyncTestObjectASIO(),
	LogTester(aImmediate),
	mTCPClient(mLog.GetLogger(aLevel, "TCPClient"), this->GetService(), TcpSettings("127.0.0.1", 50000)),
	mTCPServer(mLog.GetLogger(aLevel, "TCPSever"), this->GetService(), TcpSettings("127.0.0.1", 50000)),
	mUDPClient(mLog.GetLogger(aLevel,"UDPClient"),
	           this->GetService(),
	           boost::asio::ip::udp::endpoint(
	               boost::asio::ip::udp::v4(),
	               50000),
	           UdpSettings("127.0.0.1", 50000)),
	mUDPServer(mLog.GetLogger(aLevel,"UDPSever"),
	           this->GetService(),
	           boost::asio::ip::udp::endpoint(
	               boost::asio::ip::udp::v4(),
	               50000),
			   UdpSettings("127.0.0.1", 50000)),
	mTCPClientAdapter(mLog.GetLogger(aLevel, "TCPClientAdapter"), &mTCPClient, aAutoRead),
	mTCPServerAdapter(mLog.GetLogger(aLevel, "TCPServerAdapter"), &mTCPServer, aAutoRead),
	mUDPClientAdapter(mLog.GetLogger(aLevel, "UDPClientAdapter"), &mUDPClient, aAutoRead),
	mUDPServerAdapter(mLog.GetLogger(aLevel, "UDPServerAdapter"), &mUDPServer, aAutoRead),
	mTCPClientUpper(mLog.GetLogger(aLevel, "MockUpperTCPClient")),
	mTCPServerUpper(mLog.GetLogger(aLevel, "MockUpperTCPServer")),
	mUDPClientUpper(mLog.GetLogger(aLevel, "MockUpperUDPClient")),
	mUDPServerUpper(mLog.GetLogger(aLevel, "MockUpperUDPServer"))
{
	mTCPClientAdapter.SetUpperLayer(&mTCPClientUpper);
	mTCPServerAdapter.SetUpperLayer(&mTCPServerUpper);

	mUDPClientAdapter.SetUpperLayer(&mUDPClientUpper);
	mUDPServerAdapter.SetUpperLayer(&mUDPServerUpper);
}

}
