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
#ifndef __PHYSICAL_LAYER_ASYNC_BASE_UDP_H_
#define __PHYSICAL_LAYER_ASYNC_BASE_UDP_H_

#include <opendnp3/APL/PhysicalLayerAsyncASIO.h>
#include <opendnp3/APL/UdpSettings.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/udp.hpp>
#include <memory>

namespace apl
{

/**
Common UDP socket class and some shared implementations for server/client.
*/
class PhysicalLayerAsyncBaseUDP : public PhysicalLayerAsyncASIO
{
public:
	PhysicalLayerAsyncBaseUDP(Logger*, boost::asio::io_service* apIOService,
		   	const UdpSettings& arSettings);

	virtual ~PhysicalLayerAsyncBaseUDP() {}

	/* Implement the shared client/server actions */
	void DoClose();
	void DoOpeningClose();
	void DoOpenFailure();

protected:
	boost::asio::ip::udp::socket mSocket;
	UdpSettings mSettings;
	void CloseSocket();

	boost::asio::ip::address ResolveAddress(const std::string& arEndpoint,
			boost::system::error_code& ec);

private:
	void ShutdownSocket();

};

}

#endif
