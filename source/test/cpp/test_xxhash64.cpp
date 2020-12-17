#include "xbase/x_target.h"
#include "xbase/x_buffer.h"
#include "xhash/x_hash.h"

#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xxhash64_t)
{
	UNITTEST_FIXTURE(xxhash)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}
		

		UNITTEST_TEST(hash1)
		{
			u32 len=10;
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13};
			xhash::hash::xxhash64_t hash = xhash::xxhash.compute(cbuffer_t(len, indata));
			CHECK_EQUAL(hash.m_data, 0x0942d2129c275a72);
		}
		
	}
}
UNITTEST_SUITE_END