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
#include <opendnp3/APL/PhysicalLayerAsyncUDPServer.h>

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

PhysicalLayerAsyncUDPServer::PhysicalLayerAsyncUDPServer(Logger* apLogger,
	boost::asio::io_service* apIOService, const boost::asio::ip::udp::endpoint& arEndpoint,
	const UdpSettings& arSettings)
	: PhysicalLayerAsyncBaseUDP(apLogger, apIOService, arSettings),
	  mLocalEndpoint(arEndpoint)
{
	boost::system::error_code ec;
	boost::asio::ip::address addr = ResolveAddress(mSettings.mAddress, ec);
	if (ec) {
		throw ArgumentException(LOCATION, "endpoint: " + mSettings.mAddress + ", " + ec.message());
	}

	mLocalEndpoint.address(addr);
}

/* Implement the actions */

void PhysicalLayerAsyncUDPServer::DoOpen()
{
	boost::system::error_code ec;

	if (!mSocket.is_open())
	{
		mSocket.open(mLocalEndpoint.protocol(), ec);
		if (!ec) {
			mSocket.set_option(ip::tcp::acceptor::reuse_address(true));
			mSocket.bind(mLocalEndpoint, ec);
		}
	}

	mpService->post(boost::bind(&PhysicalLayerAsyncUDPServer::OnOpenCallback, this, ec));
}

void PhysicalLayerAsyncUDPServer::DoOpenSuccess()
{
	LOG_BLOCK(LEV_INFO, "Port successfully opened: " << mLocalEndpoint);

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

void PhysicalLayerAsyncUDPServer::DoAsyncRead(boost::uint8_t* apBuffer, size_t aMaxBytes)
{
	mSocket.async_receive_from(buffer(apBuffer, aMaxBytes),
	                           mRemoteEndpoint,
	                           boost::bind(&PhysicalLayerAsyncUDPServer::OnReadCallback,
	                                       this,
	                                       boost::asio::placeholders::error,
	                                       apBuffer,
	                                       boost::asio::placeholders::bytes_transferred));
}

void PhysicalLayerAsyncUDPServer::DoAsyncWrite(const boost::uint8_t* apBuffer, size_t aNumBytes)
{
	mSocket.async_send_to(buffer(apBuffer, aNumBytes),
	                      mRemoteEndpoint,
	                      boost::bind(&PhysicalLayerAsyncUDPServer::OnWriteCallback,
	                                  this,
	                                  boost::asio::placeholders::error,
	                                  aNumBytes));
}

}

/* vim: set ts=4 sw=4: */
