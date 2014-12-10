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
#include <opendnp3/APL/PhysicalLayerAsyncBaseUDP.h>

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

PhysicalLayerAsyncBaseUDP::PhysicalLayerAsyncBaseUDP(Logger* apLogger,
	boost::asio::io_service* apIOService, const UdpSettings& arSettings)
	: PhysicalLayerAsyncASIO(apLogger, apIOService),
	  mSocket(*apIOService),
	  mSettings(arSettings)
{
}

/* Implement the actions */

void PhysicalLayerAsyncBaseUDP::DoClose()
{
	this->ShutdownSocket();
	this->CloseSocket();
}

void PhysicalLayerAsyncBaseUDP::DoOpeningClose()
{
	this->CloseSocket();
}

void PhysicalLayerAsyncBaseUDP::DoOpenFailure()
{
	LOG_BLOCK(LEV_DEBUG, "Failed socket open, closing socket");
	this->CloseSocket();
}

void PhysicalLayerAsyncBaseUDP::CloseSocket()
{
	boost::system::error_code ec;

	mSocket.close(ec);
	if(ec) LOG_BLOCK(LEV_WARNING, "Error while closing socket: " << ec.message());
}

void PhysicalLayerAsyncBaseUDP::ShutdownSocket()
{
	boost::system::error_code ec;

	mSocket.shutdown(ip::udp::socket::shutdown_both, ec);
	if(ec) LOG_BLOCK(LEV_WARNING, "Error while shutting down socket: " << ec.message());
}

boost::asio::ip::address PhysicalLayerAsyncBaseUDP::ResolveAddress(const std::string& arEndpoint,
		boost::system::error_code& ec)
{
	LOG_BLOCK(LEV_DEBUG, "converting address '" << arEndpoint << "' to an IP address object");
	try {
		boost::system::error_code address_ec;
		boost::asio::ip::address addr = boost::asio::ip::address::from_string(arEndpoint, address_ec);
		if (address_ec) {
			LOG_BLOCK(LEV_DEBUG, "unable to convert address to object");
			throw ArgumentException(LOCATION, "endpoint: " + arEndpoint + " is invalid");
		}

		LOG_BLOCK(LEV_DEBUG, "address converted to object: " << addr.to_string());
		if (addr.is_v6()) {
			boost::asio::ip::address_v6 addrv6(addr.to_v6());
			if (addrv6.is_link_local())
				LOG_BLOCK(LEV_DEBUG, "IPv6 link local address found");
		}
		return addr;
	} catch (...) {
		LOG_BLOCK(LEV_DEBUG, "attempting to resolve address '" << arEndpoint << "'");

		boost::asio::io_service                  io_service;
		boost::asio::ip::udp::resolver           resolver(io_service);
		boost::asio::ip::udp::resolver::query    query(arEndpoint, "");
		boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(query, ec);

		// On failure to resolve
		if (ec) return boost::asio::ip::address();

		boost::asio::ip::udp::resolver::iterator end;
		while (iter != end)
		{
			boost::asio::ip::udp::endpoint ep = *iter++;
			LOG_BLOCK(LEV_DEBUG, "address '" << arEndpoint << "' resolved to " << ep.address().to_string());
			if (ep.address().is_v6()) {
				boost::asio::ip::address_v6 addrv6(ep.address().to_v6());
				if (addrv6.is_link_local())
					LOG_BLOCK(LEV_DEBUG, "IPv6 link local address found");
			}
			return ep.address();
		}
	}
}

}

/* vim: set ts=4 sw=4: */
