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
#ifndef _PHYSICAL_LAYER_FACTORY_H_
#define _PHYSICAL_LAYER_FACTORY_H_

#include <opendnp3/APL/Exception.h>
#include <opendnp3/APL/PhysicalLayerFunctors.h>
#include <opendnp3/APL/SerialTypes.h>
#include <opendnp3/APL/TcpSettings.h>
#include <opendnp3/APL/UdpSettings.h>

#include <boost/cstdint.hpp>
#include <map>

namespace apl
{

class PhysicalLayerFactory
{
public:

	static IPhysicalLayerAsyncFactory GetSerialAsync(SerialSettings s);
	static IPhysicalLayerAsyncFactory GetTCPv4ClientAsync(TcpSettings s);
	static IPhysicalLayerAsyncFactory GetTCPv4ServerAsync(TcpSettings s);
	static IPhysicalLayerAsyncFactory GetTCPv6ClientAsync(TcpSettings s);
	static IPhysicalLayerAsyncFactory GetTCPv6ServerAsync(TcpSettings s);
	static IPhysicalLayerAsyncFactory GetUDPv4ClientAsync(UdpSettings s);
	static IPhysicalLayerAsyncFactory GetUDPv4ServerAsync(UdpSettings s);
	static IPhysicalLayerAsyncFactory GetUDPv6ClientAsync(UdpSettings s);
	static IPhysicalLayerAsyncFactory GetUDPv6ServerAsync(UdpSettings s);

	//normal factory functions
	static IPhysicalLayerAsync* FGetSerialAsync(SerialSettings s, boost::asio::io_service* apSrv, Logger* apLogger);
	static IPhysicalLayerAsync* FGetTCPv4ClientAsync(TcpSettings s, boost::asio::io_service* apSrv, Logger* apLogger);
	static IPhysicalLayerAsync* FGetTCPv4ServerAsync(TcpSettings s, boost::asio::io_service* apSrv, Logger* apLogger);
	static IPhysicalLayerAsync* FGetTCPv6ClientAsync(TcpSettings s, boost::asio::io_service* apSrv, Logger* apLogger);
	static IPhysicalLayerAsync* FGetTCPv6ServerAsync(TcpSettings s, boost::asio::io_service* apSrv, Logger* apLogger);
	static IPhysicalLayerAsync* FGetUDPv4ClientAsync(UdpSettings s, boost::asio::io_service* apSrv, Logger* apLogger);
	static IPhysicalLayerAsync* FGetUDPv4ServerAsync(UdpSettings s, boost::asio::io_service* apSrv, Logger* apLogger);
	static IPhysicalLayerAsync* FGetUDPv6ClientAsync(UdpSettings s, boost::asio::io_service* apSrv, Logger* apLogger);
	static IPhysicalLayerAsync* FGetUDPv6ServerAsync(UdpSettings s, boost::asio::io_service* apSrv, Logger* apLogger);

};

}

#endif
