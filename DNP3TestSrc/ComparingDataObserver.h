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
#ifndef __COMPARING_DATA_OBSERVER_H_
#define __COMPARING_DATA_OBSERVER_H_

#include <opendnp3/APL/DataInterfaces.h>
#include <opendnp3/APL/Loggable.h>
#include <opendnp3/APL/Lock.h>
#include <opendnp3/APL/FlexibleDataObserver.h>
#include <opendnp3/APL/Logger.h>

#include <map>

namespace apl
{

namespace dnp
{

class ComparingDataObserver : public apl::IDataObserver, private Loggable
{
public:

	ComparingDataObserver(Logger* apLogger, FlexibleDataObserver* apObserver);

	void Reset();

	bool WaitForSameData(millis_t aWaitMs);

	void DescribeMissingData();

private:

	bool mSameData;

	bool IsSameData();

	SigLock mLock;
	FlexibleDataObserver* mpObserver;

	typedef std::map<size_t, bool> CompareMap;

	CompareMap mBinaryMap;
	CompareMap mAnalogMap;
	CompareMap mCounterMap;
	CompareMap mControlStatusMap;
	CompareMap mSetpointStatusMap;

	void _Start();
	void _End();

	void _Update(const Binary& arPoint, size_t aIndex);
	void _Update(const Analog& arPoint, size_t aIndex);
	void _Update(const Counter& arPoint, size_t aIndex);
	void _Update(const ControlStatus& arPoint, size_t aIndex);
	void _Update(const SetpointStatus& arPoint, size_t aIndex);

	template <class T>
	void UpdateAny(const T& arPoint, size_t aIndex, const typename PointMap<T>::Type& arMap, CompareMap& arCompareMap);

	template <class T>
	void DescribeAny(const typename PointMap<T>::Type& arMap, const CompareMap& arCompareMap);
};


template <class T>
void ComparingDataObserver::DescribeAny(const typename PointMap<T>::Type& arMap, const CompareMap& arCompareMap)
{
	for(typename PointMap<T>::Type::const_iterator i = arMap.begin(); i != arMap.end(); ++i) {
		CompareMap::const_iterator j = arCompareMap.find(i->first);
		if(j == arCompareMap.end()) {
			LOG_BLOCK(LEV_EVENT, "Missing: " << i->first << " - " << i->second.ToString());
		}
	}
}

template <class T>
void ComparingDataObserver::UpdateAny(const T& arPoint, size_t aIndex, const typename PointMap<T>::Type& arMap, CompareMap& arCompareMap)
{
	typename PointMap<T>::Type::const_iterator i = arMap.find(aIndex);
	if(i == arMap.end()) {
		LOG_BLOCK(LEV_ERROR, "Unexpected index: " << aIndex << " - " << arPoint.ToString());
	}
	else {
		if(i->second == arPoint) {
			arCompareMap[aIndex] = true;
		}
		else arCompareMap.erase(aIndex);
	}
}

}

}

#endif

