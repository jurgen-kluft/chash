#include "cbase/c_target.h"
#include "cbase/c_buffer.h"
#include "chash/c_hash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(xxhash64_t)
{
	UNITTEST_FIXTURE(xxhash)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}
		

		UNITTEST_TEST(hash1)
		{
			u32 len=10;
			u8 indata[]={1,2,3,4,5,6,7,8,9,13};
			xdigest::xxhash64 hash = xxhash64.compute(cbuffer_t(len, indata));
			CHECK_EQUAL(hash.m_data, 0x0942d2129c275a72);
		}
		
	}
}
UNITTEST_SUITE_END