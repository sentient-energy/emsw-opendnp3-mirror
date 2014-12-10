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


#include "PhysicalLayerXMLFactory.h"

#include <opendnp3/APL/PhysicalLayerFactory.h>
#include <boost/numeric/conversion/converter.hpp>

namespace apl
{
namespace xml
{

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetFactoryAsync(const APLXML_Base::PhysicalLayerDescriptor_t* apCfg)
{
	const APLXML_Base::Serial_t* pSerial = dynamic_cast<const APLXML_Base::Serial_t*>(apCfg);
	if(pSerial != NULL) return GetAsync(pSerial);

	const APLXML_Base::TCPv4Server_t* pTCPServerV4 = dynamic_cast<const APLXML_Base::TCPv4Server_t*>(apCfg);
	if(pTCPServerV4 != NULL) return GetAsync(pTCPServerV4);

	const APLXML_Base::TCPv4Client_t* pTCPClientV4 = dynamic_cast<const APLXML_Base::TCPv4Client_t*>(apCfg);
	if(pTCPClientV4 != NULL) return GetAsync(pTCPClientV4);

	const APLXML_Base::TCPv6Server_t* pTCPServerV6 = dynamic_cast<const APLXML_Base::TCPv6Server_t*>(apCfg);
	if(pTCPServerV6 != NULL) return GetAsync(pTCPServerV6);

	const APLXML_Base::TCPv6Client_t* pTCPClientV6 = dynamic_cast<const APLXML_Base::TCPv6Client_t*>(apCfg);
	if(pTCPClientV6 != NULL) return GetAsync(pTCPClientV6);

	const APLXML_Base::UDPv4Server_t* pUDPServerV4 = dynamic_cast<const APLXML_Base::UDPv4Server_t*>(apCfg);
	if(pUDPServerV4 != NULL) return GetAsync(pUDPServerV4);

	const APLXML_Base::UDPv4Client_t* pUDPClientV4 = dynamic_cast<const APLXML_Base::UDPv4Client_t*>(apCfg);
	if(pUDPClientV4 != NULL) return GetAsync(pUDPClientV4);

	const APLXML_Base::UDPv6Server_t* pUDPServerV6 = dynamic_cast<const APLXML_Base::UDPv6Server_t*>(apCfg);
	if(pUDPServerV6 != NULL) return GetAsync(pUDPServerV6);

	const APLXML_Base::UDPv6Client_t* pUDPClientV6 = dynamic_cast<const APLXML_Base::UDPv6Client_t*>(apCfg);
	if(pUDPClientV6 != NULL) return GetAsync(pUDPClientV6);

	throw Exception(LOCATION, "Unknown PhysicalLayerDescriptor_t");
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::Serial_t* apCfg)
{
	SerialSettings s = GetSerialSettings(apCfg);
	return PhysicalLayerFactory::GetSerialAsync(s);
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::TCPv4Client_t* apCfg)
{
	return PhysicalLayerFactory::GetTCPv4ClientAsync(GetTcpV4ClientSettings(apCfg));
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::TCPv4Server_t* apCfg)
{
	return PhysicalLayerFactory::GetTCPv4ServerAsync(GetTcpV4ServerSettings(apCfg));
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::TCPv6Client_t* apCfg)
{
	return PhysicalLayerFactory::GetTCPv6ClientAsync(GetTcpV6ClientSettings(apCfg));
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::TCPv6Server_t* apCfg)
{
	return PhysicalLayerFactory::GetTCPv6ServerAsync(GetTcpV6ServerSettings(apCfg));
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::UDPv4Client_t* apCfg)
{
	return PhysicalLayerFactory::GetUDPv4ClientAsync(GetUdpV4ClientSettings(apCfg));
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::UDPv4Server_t* apCfg)
{
	return PhysicalLayerFactory::GetUDPv4ServerAsync(GetUdpV4ServerSettings(apCfg));
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::UDPv6Client_t* apCfg)
{
	return PhysicalLayerFactory::GetUDPv6ClientAsync(GetUdpV6ClientSettings(apCfg));
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::UDPv6Server_t* apCfg)
{
	return PhysicalLayerFactory::GetUDPv6ServerAsync(GetUdpV6ServerSettings(apCfg));
}

SerialSettings GetSerialSettings(const APLXML_Base::Serial_t* apCfg)
{
	SerialSettings s;
	s.mBaud = BaudToInt(apCfg->BaudRate);
	s.mDataBits = ((apCfg->DBits == APLXML_Base::DATABITS_7) ? 7 : 8);
	s.mDevice = apCfg->Device;
	s.mFlowType = EnumToFlow(apCfg->FlowControl);
	s.mParity = EnumToParity(apCfg->Parity);
	s.mStopBits = ((apCfg->StopBits == APLXML_Base::STOPBITS_0) ? 0 : 1);
	return s;
}

int BaudToInt(APLXML_Base::BaudRateEnum aBaud)
{
	switch(aBaud) {
	case(APLXML_Base::BAUDRATE_1200): return 1200;
	case(APLXML_Base::BAUDRATE_1800): return 1800;
	case(APLXML_Base::BAUDRATE_2400): return 2400;
	case(APLXML_Base::BAUDRATE_4800): return 4800;
	case(APLXML_Base::BAUDRATE_9600): return 9600;
	case(APLXML_Base::BAUDRATE_19200): return 19200;
	case(APLXML_Base::BAUDRATE_38400): return 38400;
	case(APLXML_Base::BAUDRATE_57600): return 57600;
	case(APLXML_Base::BAUDRATE_115200): return 115200;
	case(APLXML_Base::BAUDRATE_230400): return 230400;
	}
	assert(false);
	return -1;
}

ParityType EnumToParity(APLXML_Base::ParityEnum aParity)
{
	switch(aParity) {
	case (APLXML_Base::PARITY_NONE): return PAR_NONE;
	case (APLXML_Base::PARITY_ODD): return PAR_ODD;
	case (APLXML_Base::PARITY_EVEN): return PAR_EVEN;
	}
	assert(false);
	return PAR_NONE;
}

FlowType EnumToFlow(APLXML_Base::FlowControlEnum aFlow)
{
	switch(aFlow) {
	case (APLXML_Base::FLOW_NONE): return FLOW_NONE;
	case (APLXML_Base::FLOW_HARDWARE): return FLOW_HARDWARE;
	case (APLXML_Base::FLOW_XONXOFF): return FLOW_XONXOFF;
	}
	assert(false);
	return FLOW_NONE;
}

TcpSettings GetTcpV4ClientSettings(const APLXML_Base::TCPv4Client_t* apCfg)
{
	TcpSettings s;
	s.mAddress = apCfg->Address;
	s.mPort = boost::numeric::converter<boost::uint16_t, int>::convert(apCfg->Port);
	s.mUseKeepAlives = apCfg->UseKeepAlives;
	s.mSendBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->SendBufferSize);
	s.mRecvBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->RecvBufferSize);
	return s;
}

TcpSettings GetTcpV4ServerSettings(const APLXML_Base::TCPv4Server_t* apCfg)
{
	TcpSettings s;
	s.mAddress = apCfg->Endpoint;
	s.mPort = boost::numeric::converter<boost::uint16_t, int>::convert(apCfg->Port);
	s.mUseKeepAlives = apCfg->UseKeepAlives;
	s.mSendBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->SendBufferSize);
	s.mRecvBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->RecvBufferSize);
	return s;
}

TcpSettings GetTcpV6ClientSettings(const APLXML_Base::TCPv6Client_t* apCfg)
{
	TcpSettings s;
	s.mAddress = apCfg->Address;
	s.mPort = boost::numeric::converter<boost::uint16_t, int>::convert(apCfg->Port);
	s.mUseKeepAlives = apCfg->UseKeepAlives;
	s.mSendBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->SendBufferSize);
	s.mRecvBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->RecvBufferSize);
	return s;
}

TcpSettings GetTcpV6ServerSettings(const APLXML_Base::TCPv6Server_t* apCfg)
{
	TcpSettings s;
	s.mAddress = apCfg->Endpoint;
	s.mPort = boost::numeric::converter<boost::uint16_t, int>::convert(apCfg->Port);
	s.mUseKeepAlives = apCfg->UseKeepAlives;
	s.mSendBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->SendBufferSize);
	s.mRecvBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->RecvBufferSize);
	return s;
}

UdpSettings GetUdpV4ClientSettings(const APLXML_Base::UDPv4Client_t* apCfg)
{
	UdpSettings s;
	s.mAddress = apCfg->Address;
	s.mPort = boost::numeric::converter<boost::uint16_t, int>::convert(apCfg->Port);
	s.mSendBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->SendBufferSize);
	s.mRecvBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->RecvBufferSize);
	return s;
}

UdpSettings GetUdpV4ServerSettings(const APLXML_Base::UDPv4Server_t* apCfg)
{
	UdpSettings s;
	s.mAddress = apCfg->Endpoint;
	s.mPort = boost::numeric::converter<boost::uint16_t, int>::convert(apCfg->Port);
	s.mSendBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->SendBufferSize);
	s.mRecvBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->RecvBufferSize);
	return s;
}

UdpSettings GetUdpV6ClientSettings(const APLXML_Base::UDPv6Client_t* apCfg)
{
	UdpSettings s;
	s.mAddress = apCfg->Address;
	s.mPort = boost::numeric::converter<boost::uint16_t, int>::convert(apCfg->Port);
	s.mSendBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->SendBufferSize);
	s.mRecvBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->RecvBufferSize);
	return s;
}

UdpSettings GetUdpV6ServerSettings(const APLXML_Base::UDPv6Server_t* apCfg)
{
	UdpSettings s;
	s.mAddress = apCfg->Endpoint;
	s.mPort = boost::numeric::converter<boost::uint16_t, int>::convert(apCfg->Port);
	s.mSendBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->SendBufferSize);
	s.mRecvBufferSize = boost::numeric::converter<size_t, int>::convert(apCfg->RecvBufferSize);
	return s;
}

}
}
