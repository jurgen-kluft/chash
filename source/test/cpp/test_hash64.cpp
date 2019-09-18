#include "xbase/x_target.h"
#include "xbase/x_buffer.h"
#include "xhash/x_hash.h"

#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xmurmur64)
{
	UNITTEST_FIXTURE(xdigest_murmur64)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static u64 murmur64_hash(xcbuffer const& b)
		{
			xhash::hash::murmur64 h1 = xhash::murmur64.compute(b);
			xbinary_reader reader(h1.cbuffer());
			u64 h2;
			reader.read(h2);
			return h2;
		}

		UNITTEST_TEST(test)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13},indata2[]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10;
			u64 ruhash  = murmur64_hash(xcbuffer(len,indata));
			u64 ruhash2 = murmur64_hash(xcbuffer(len,indata));
			CHECK_EQUAL(ruhash>0,true);
			CHECK_EQUAL(ruhash,ruhash2);
		}
		
	}
}
UNITTEST_SUITE_END