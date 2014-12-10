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

#include <opendnp3/APL/Exception.h>
#include <opendnp3/APL/IHandlerAsync.h>
#include <opendnp3/APL/Logger.h>
#include <opendnp3/APL/PhysicalLayerAsyncUDPClient.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/bind.hpp>
#include <string>

using namespace boost;
using namespace boost::asio;
using namespace boost::system;
using namespace std;

namespace apl
{

PhysicalLayerAsyncUDPClient::PhysicalLayerAsyncUDPClient(Logger* apLogger,
	boost::asio::io_service* apIOService, const boost::asio::ip::udp::endpoint& arEndpoint,
	const UdpSettings& arSettings)
	: PhysicalLayerAsyncBaseUDP(apLogger, apIOService, arSettings),
	  mRemoteEndpoint(arEndpoint)
{
}

/* Implement the actions */

void PhysicalLayerAsyncUDPClient::DoOpen()
{
	/* Re-resolve the remote address each time just in case DNS shifts things on us */
	boost::system::error_code ec;
	boost::asio::ip::address addr = ResolveAddress(mSettings.mAddress, ec);
	if (ec) {
		OnOpenCallback(ec);
	}
	else {
		mRemoteEndpoint.address(addr);
		mSocket.async_connect(mRemoteEndpoint,
	                      	  boost::bind(&PhysicalLayerAsyncUDPClient::OnOpenCallback,
	                                      this,
	                                      boost::asio::placeholders::error));
	}
}

void PhysicalLayerAsyncUDPClient::DoOpenSuccess()
{
	LOG_BLOCK(LEV_INFO, "Connected to: " << mRemoteEndpoint);

	if (mSettings.mSendBufferSize > 0) {
		boost::asio::socket_base::send_buffer_size option(mSettings.mSendBufferSize);
		mSocket.set_option(option);
		mSocket.get_option(option);
		LOG_BLOCK(LEV_DEBUG, "Set send buffer size to " << option.value());
	}

	if (mSettings.mRecvBufferSize > 0) {
		boost::asio::socket_base::receive_buffer_size option(mSettings.mRecvBufferSize);
		mSocket.set_option(option);
		mSocket.get_option(option);
		LOG_BLOCK(LEV_DEBUG, "Set receive buffer size to " << option.value());
	}
}

void PhysicalLayerAsyncUDPClient::DoAsyncRead(boost::uint8_t* apBuffer, size_t aMaxBytes)
{
	mSocket.async_receive(buffer(apBuffer, aMaxBytes),
	                      boost::bind(&PhysicalLayerAsyncUDPClient::OnReadCallback,
	                                  this,
	                                  boost::asio::placeholders::error,
	                                  apBuffer,
	                                  boost::asio::placeholders::bytes_transferred));
}

void PhysicalLayerAsyncUDPClient::DoAsyncWrite(const boost::uint8_t* apBuffer, size_t aNumBytes)
{
	mSocket.async_send(buffer(apBuffer, aNumBytes),
	                   boost::bind(&PhysicalLayerAsyncUDPClient::OnWriteCallback,
	                               this,
	                               boost::asio::placeholders::error,
	                               aNumBytes));
}

}

/* vim: set ts=4 sw=4: */
