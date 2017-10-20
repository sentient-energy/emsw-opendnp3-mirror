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
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>

#include <opendnp3/APL/Util.h>
#include <APLTestTools/BufferHelpers.h>

using namespace std;
using namespace apl;


BOOST_AUTO_TEST_SUITE(UtilSuite)
template <int N>
void TestHex(const std::string& aHex, boost::uint8_t* aCompareBytes, size_t aCount)
{
	HexSequence hs(aHex);

	BOOST_REQUIRE(hs.Size() <= N);

	BOOST_REQUIRE_EQUAL(hs.Size(), aCount );
	for ( size_t i = 0; i < aCount; i++ )
		BOOST_REQUIRE_EQUAL(hs[i], aCompareBytes[i]);
}

BOOST_AUTO_TEST_CASE(HexToBytes2TestSmall)
{
	boost::uint8_t values[] = { 0xAF, 0x23 };
	TestHex<2>( "AF23", values, 2 );
}
BOOST_AUTO_TEST_CASE(HexToBytes2Test64)
{
	boost::uint8_t values[] = { 0x13, 0xA2, 0x00, 0x40, 0x56, 0x1D, 0x08 };
	TestHex<7>( "13A20040561D08", values, 7 );
}

BOOST_AUTO_TEST_CASE(HexToBytes2Test64TooBig)
{
	boost::uint8_t values[] = { 0x13, 0xA2, 0x00, 0x40, 0x56, 0x1D, 0x08 };
	TestHex<8>( "13A20040561D08", values, 7 );
}


BOOST_AUTO_TEST_CASE(HexToBytes2Test64Hole)
{
	boost::uint8_t values[] = { 0x13, 0xA2, 0x00, 0x40, 0x56, 0x1D, 0x08 };
	TestHex<8>( "13A200 40561   D08", values, 7 );
}

BOOST_AUTO_TEST_SUITE_END()
