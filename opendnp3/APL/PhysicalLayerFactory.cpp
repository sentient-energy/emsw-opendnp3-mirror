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

#include <opendnp3/APL/Log.h>
#include <opendnp3/APL/PhysicalLayerAsyncSerial.h>
#include <opendnp3/APL/PhysicalLayerAsyncTCPv4Client.h>
#include <opendnp3/APL/PhysicalLayerAsyncTCPv4Server.h>
#include <opendnp3/APL/PhysicalLayerAsyncTCPv6Client.h>
#include <opendnp3/APL/PhysicalLayerAsyncTCPv6Server.h>
#include <opendnp3/APL/PhysicalLayerAsyncUDPClient.h>
#include <opendnp3/APL/PhysicalLayerAsyncUDPServer.h>
#include <opendnp3/APL/PhysicalLayerFactory.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ip/udp.hpp>

namespace apl
{

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetSerialAsync(SerialSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetSerialAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetTCPv4ClientAsync(TcpSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetTCPv4ClientAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetTCPv4ServerAsync(TcpSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetTCPv4ServerAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetTCPv6ClientAsync(TcpSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetTCPv6ClientAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetTCPv6ServerAsync(TcpSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetTCPv6ServerAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetUDPv4ClientAsync(UdpSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetUDPv4ClientAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetUDPv4ServerAsync(UdpSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetUDPv4ServerAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetUDPv6ClientAsync(UdpSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetUDPv6ClientAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetUDPv6ServerAsync(UdpSettings s)
{
	return boost::bind(&PhysicalLayerFactory::FGetUDPv6ServerAsync, s, _2, _1);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetSerialAsync(SerialSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncSerial(apLogger, apSrv, s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetTCPv4ClientAsync(TcpSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncTCPv4Client(apLogger, apSrv, s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetTCPv4ServerAsync(TcpSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncTCPv4Server(apLogger, apSrv, s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetTCPv6ClientAsync(TcpSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncTCPv6Client(apLogger, apSrv, s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetTCPv6ServerAsync(TcpSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncTCPv6Server(apLogger, apSrv, s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetUDPv4ClientAsync(UdpSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncUDPClient(
			apLogger,
			apSrv,
			boost::asio::ip::udp::endpoint(
				boost::asio::ip::udp::v4(),
				s.mPort),
			s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetUDPv4ServerAsync(UdpSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncUDPServer(
			apLogger,
			apSrv,
			boost::asio::ip::udp::endpoint(
				boost::asio::ip::udp::v4(),
				s.mPort),
			s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetUDPv6ClientAsync(UdpSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncUDPClient(apLogger,
			apSrv,
			boost::asio::ip::udp::endpoint(
				boost::asio::ip::udp::v6(),
				s.mPort),
			s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetUDPv6ServerAsync(UdpSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncUDPServer(apLogger,
			apSrv,
			boost::asio::ip::udp::endpoint(
				boost::asio::ip::udp::v6(),
				s.mPort),
			s);
}

}
