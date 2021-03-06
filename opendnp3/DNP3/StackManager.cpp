//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright
// ownership.  Green Enery Corp licenses this file to you under the
// Apache License, Version 2.0 (the "License"); you may not use this
// file except in compliance with the License.  You may obtain a copy of
// the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied.  See the License for the specific language governing
// permissions and limitations under the License.
//
#include <opendnp3/APL/Log.h>
#include <opendnp3/DNP3/AsyncStackManager.h>
#include <opendnp3/DNP3/StackManager.h>

namespace apl
{
namespace dnp
{

StackManager::StackManager(FilterLevel aLevel, const std::string& logFile)
	: mpLog      ( new EventLog() )
	, mpLogToFile( new LogToFile(mpLog, logFile) )
	, mpImpl     ( new AsyncStackManager(mpLog->GetLogger(aLevel, "dnp")) )
{}

void StackManager::AddLogHook(ILogBase* apHook)
{
	mpLog->AddLogSubscriber(apHook);
}

StackManager::~StackManager()
{
	delete mpImpl;
	delete mpLog;
}

//used for defining ports
void StackManager::AddTCPClient(const std::string& arName, PhysLayerSettings s, TcpSettings aTcp)
{
	AddTCPv4Client(arName, s, aTcp);
}

void StackManager::AddTCPv4Client(const std::string& arName, PhysLayerSettings s, TcpSettings aTcp)
{
	mpImpl->AddTCPv4Client(arName, s, aTcp);
}

void StackManager::AddTCPServer(const std::string& arName, PhysLayerSettings s, TcpSettings aTcp)
{
	AddTCPv4Server(arName, s, aTcp);
}

void StackManager::AddTCPv4Server(const std::string& arName, PhysLayerSettings s, TcpSettings aTcp)
{
	mpImpl->AddTCPv4Server(arName, s, aTcp);
}

void StackManager::AddTCPv6Client(const std::string& arName, PhysLayerSettings s, TcpSettings aTcp)
{
	mpImpl->AddTCPv6Client(arName, s, aTcp);
}

void StackManager::AddTCPv6Server(const std::string& arName, PhysLayerSettings s, TcpSettings aTcp)
{
	mpImpl->AddTCPv6Server(arName, s, aTcp);
}

void StackManager::AddUDPClient(const std::string& arName, PhysLayerSettings s, UdpSettings aUdp)
{
	AddUDPv4Client(arName, s, aUdp);
}

void StackManager::AddUDPv4Client(const std::string& arName, PhysLayerSettings s, UdpSettings aUdp)
{
	mpImpl->AddUDPv4Client(arName, s, aUdp);
}

void StackManager::AddUDPServer(const std::string& arName, PhysLayerSettings s, UdpSettings aUdp)
{
	AddUDPv4Server(arName, s, aUdp);
}

void StackManager::AddUDPv4Server(const std::string& arName, PhysLayerSettings s, UdpSettings aUdp)
{
	mpImpl->AddUDPv4Server(arName, s, aUdp);
}

void StackManager::AddUDPv6Client(const std::string& arName, PhysLayerSettings s, UdpSettings aUdp)
{
	mpImpl->AddUDPv6Client(arName, s, aUdp);
}

void StackManager::AddUDPv6Server(const std::string& arName, PhysLayerSettings s, UdpSettings aUdp)
{
	mpImpl->AddUDPv6Server(arName, s, aUdp);
}

void StackManager::AddSerial(const std::string& arName, PhysLayerSettings s, SerialSettings aSerial)
{
	mpImpl->AddSerial(arName, s, aSerial);
}

ICommandAcceptor* StackManager::AddMaster(const std::string& arPortName, const std::string& arStackName, FilterLevel aLevel,
        IDataObserver* apPublisher, const MasterStackConfig& arCfg)
{
	return mpImpl->AddMaster(arPortName, arStackName, aLevel, apPublisher, arCfg);
}

IDataObserver* StackManager::AddSlave(const std::string& arPortName, const std::string& arStackName, FilterLevel aLevel,
                                      ICommandAcceptor* apCmdAcceptor, const SlaveStackConfig& arCfg)
{
	return mpImpl->AddSlave(arPortName, arStackName, aLevel, apCmdAcceptor, arCfg);
}

void StackManager::Shutdown()
{
	mpImpl->Shutdown();
}

void StackManager::RemovePort(const std::string& arPortName)
{
	mpImpl->RemovePort(arPortName);
}
void StackManager::RemoveStack(const std::string& arStackName)
{
	mpImpl->RemoveStack(arStackName);
}
Stack* StackManager::GetStack(const std::string& arStackName)
{
	return mpImpl->GetStack(arStackName);
}
std::vector<std::string> StackManager::GetStackNames()
{
	return mpImpl->GetStackNames();
}
std::vector<std::string> StackManager::GetPortNames()
{
	return mpImpl->GetPortNames();
}

void StackManager::StartVtoRouter(const std::string& arPortName, const std::string& arStackName, const VtoRouterSettings& arSettings)
{
	mpImpl->StartVtoRouter(arPortName, arStackName, arSettings);
}

void StackManager::StopVtoRouter(const std::string& arStackName, boost::uint8_t aVtoChannelId)
{
	mpImpl->StopVtoRouter(arStackName, aVtoChannelId);
}

void StackManager::StopAllRoutersOnStack(const std::string& arStackName)
{
	mpImpl->StopAllRoutersOnStack(arStackName);
}

IVtoWriter* StackManager::GetVtoWriter(const std::string& arStackName)
{
	return mpImpl->GetVtoWriter(arStackName);
}

}
}

/* vim: set ts=4 sw=4 noexpandtab: */

