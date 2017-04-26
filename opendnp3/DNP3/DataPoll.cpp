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
#include <opendnp3/APL/ITimeSource.h>
#include <opendnp3/DNP3/APDU.h>
#include <opendnp3/DNP3/DataPoll.h>
#include <opendnp3/DNP3/PointClass.h>
#include <opendnp3/DNP3/ResponseLoader.h>
#include <opendnp3/DNP3/VtoReader.h>
//#include <opendnp3/APL/Loggable.h>

namespace apl
{
namespace dnp
{

/* DataPoll - base class */

DataPoll::DataPoll(Logger* apLogger, IDataObserver* apObs, VtoReader* apVtoReader) :
	MasterTaskBase(apLogger),
	mpObs(apObs),
	mpVtoReader(apVtoReader)
{}

TaskResult DataPoll::_OnPartialResponse(const APDU& f)
{
	this->ReadData(f);
	return TR_CONTINUE;
}

TaskResult DataPoll::_OnFinalResponse(const APDU& f)
{
	this->ReadData(f);
	return TR_SUCCESS;
}

void DataPoll::ReadData(const APDU& f)
{
	ResponseLoader loader(mpLogger, mpObs, mpVtoReader);
	HeaderReadIterator hdr = f.BeginRead();
	for ( ; !hdr.IsEnd(); ++hdr) {
		loader.Process(hdr);
	}
}

/* Class Poll */

ClassPoll::ClassPoll(Logger* apLogger, IDataObserver* apObs, VtoReader* apVtoReader) :
	DataPoll(apLogger, apObs, apVtoReader),
	mClassMask(PC_INVALID)
{}

void ClassPoll::Set(int aClassMask)
{
	mClassMask = aClassMask;
}

int ClassPoll::GetClassMask()
{
	return mClassMask;
}

void ClassPoll::ConfigureRequest(APDU& arAPDU)
{
	if (mClassMask == PC_INVALID) {
		throw InvalidStateException(LOCATION, "Class mask has not been set");
	}

	arAPDU.Set(FC_READ);
	if (mClassMask & PC_CLASS_0) arAPDU.DoPlaceholderWrite(Group60Var1::Inst());
	if (mClassMask & PC_CLASS_1) arAPDU.DoPlaceholderWrite(Group60Var2::Inst());
	if (mClassMask & PC_CLASS_2) arAPDU.DoPlaceholderWrite(Group60Var3::Inst());
	if (mClassMask & PC_CLASS_3) arAPDU.DoPlaceholderWrite(Group60Var4::Inst());
}

/* Free-Form Poll */

FreeFormPoll::FreeFormPoll(Logger* apLogger, IDataObserver* apObs, VtoReader* apVtoReader) :
	ClassPoll(apLogger, apObs, apVtoReader) //,
	//ClassPoll::mClassMask(PC_INVALID)
{}

void FreeFormPoll::SetDataPoints(std::unordered_map<apl::DataTypes, std::vector<uint32_t>, std::EnumClassHash> pnts)
{
	ffInputPoints.clear();
	ffInputPoints = pnts;
}

//TODO kept this code for Index based writing in future
//void FreeFormPoll::WriteIndexed(const SizeByVariationObject* apObj, std::vector<uint32_t> element, APDU& arAPDU)
//{
	//Use Group30Var1* pObj = Group30Var1::Inst(); in calling method for analog points
	/*IndexedWriteIterator i = arAPDU.WriteIndexed(apObj, element.size(), QC_1B_CNT_1B_INDEX);
	for (int ind=0; ind<element.size(); i++) {
		i.SetIndex(size_t(element.at(ind)));
		++i;
	}*/
//}


void FreeFormPoll::ConfigureRequest(APDU& arAPDU)
{
	LOG_BLOCK(LEV_DEBUG, "FreeFormPoll::ConfigureRequest");

	if (this->GetClassMask() == PC_INVALID) {
		throw InvalidStateException(LOCATION, "Class mask has not been set");
	}

	arAPDU.Set(FC_READ);
    std::lock_guard<std::mutex> guard{ffInputPoints_mutex_};

	if ((this->GetClassMask() & PC_CLASS_0) && ffInputPoints.size() > 0) {

		for (std::pair<apl::DataTypes, std::vector<uint32_t>> element : ffInputPoints)
		{
			if (element.second.size() <= 0)
				continue; //should not come here
			if (element.first == apl::DataTypes::DT_ANALOG) {
				for (int ind=0; ind<element.second.size(); ++ind) {
					size_t index = element.second.at(ind);
					ObjectWriteIterator i = arAPDU.WriteContiguous(Group30Var1::Inst(), index, index, QC_1B_START_STOP );
				}
			}
			if (element.first == apl::DataTypes::DT_BINARY) {
				for (int ind=0; ind<element.second.size(); ++ind) {
					size_t index = element.second.at(ind);
					ObjectWriteIterator i = arAPDU.WriteContiguous(Group1Var2::Inst(), index, index, QC_1B_START_STOP );
				}
			}
			if (element.first == apl::DataTypes::DT_COUNTER) {
				for (int ind=0; ind<element.second.size(); ++ind) {
					size_t index = element.second.at(ind);
					ObjectWriteIterator i = arAPDU.WriteContiguous(Group20Var1::Inst(), index, index, QC_1B_START_STOP );
				}
			}
			if (element.first == apl::DataTypes::DT_CONTROL_STATUS) {
				for (int ind=0; ind<element.second.size(); ++ind) {
					size_t index = element.second.at(ind);
					ObjectWriteIterator i = arAPDU.WriteContiguous(Group10Var2::Inst(), index, index, QC_1B_START_STOP );
				}
			}
			if (element.first == apl::DataTypes::DT_SETPOINT_STATUS) {
				for (int ind=0; ind<element.second.size(); ++ind) {
					size_t index = element.second.at(ind);
					ObjectWriteIterator i = arAPDU.WriteContiguous(Group40Var1::Inst(), index, index, QC_1B_START_STOP );
				}
			}

		}
	}
}

}
} //end ns

/* vim: set ts=4 sw=4: */
