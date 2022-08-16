#include "cbase/c_target.h"
#include "cbase/c_buffer.h"
#include "chash/c_hash.h"

#include "cunittest/xunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(murmur64_t)
{
	UNITTEST_FIXTURE(xdigest_murmur64)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static u64 murmur64_hash(cbuffer_t const& b)
		{
			xdigest::murmur64 h1 = murmur64.compute(b);
			binary_reader_t reader(h1.cbuffer());
			u64 h2;
			reader.read(h2);
			return h2;
		}

		UNITTEST_TEST(test)
		{
			u8 indata[]={1,2,3,4,5,6,7,8,9,13},indata2[]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10;
			u64 ruhash  = murmur64_hash(cbuffer_t(len,indata));
			u64 ruhash2 = murmur64_hash(cbuffer_t(len,indata));
			CHECK_EQUAL(ruhash>0,true);
			CHECK_EQUAL(ruhash,ruhash2);
		}
		
	}
}
UNITTEST_SUITE_END