#include "xbase/x_target.h"
#include "xbase/x_buffer.h"
#include "xhash/x_hash.h"

#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xhash32)
{
	UNITTEST_FIXTURE(xdigest_murmur32)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(xhash_generator_murmur_Hash)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13},indata2[]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10;
			u32 ruhash=murmur32_hash(xcbuffer(len, indata),100),ruhash2=murmur32_hash(xcbuffer(len, indata),100);
			CHECK_EQUAL(ruhash>0,true);
			CHECK_EQUAL(ruhash,ruhash2);
			ruhash2=murmur32_hash(xcbuffer(9,indata),100);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
			ruhash2=murmur32_hash(xcbuffer(len, indata2),100);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
			ruhash2=murmur32_hash(xcbuffer(len, indata));
			CHECK_NOT_EQUAL(ruhash,ruhash2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_void)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13},indata2[10]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10,len2=23,ru=murmur32_hash(xcbuffer(len, indata)),ru2;
			CHECK_EQUAL(murmur32_hash(xcbuffer(len, indata)),ru);
			ru2 = murmur32_hash(xcbuffer(len, indata),ru);
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len, indata)),ru2);
			ru2 = murmur32_hash(xcbuffer(len2, indata),ru);
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len, indata)),ru2);
			ru = murmur32_hash(xcbuffer(len, indata),ru);
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len, indata)),ru2);
			ru2 = murmur32_hash(xcbuffer(len, indata2),ru);
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len, indata)),ru2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_u32)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13},indata2[10]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10,len2=23;
			for(s32 i=0;i<50;i++)
			{
				CHECK_EQUAL((murmur32_hash(xcbuffer(len, indata),i*i)>0),true);
			}
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len, indata)),murmur32_hash(xcbuffer(len, indata),100));
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len2, indata),100),murmur32_hash(xcbuffer(len, indata),100));
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len, indata),100),murmur32_hash(xcbuffer(len, indata2),100));
			CHECK_EQUAL(murmur32_hash(xcbuffer(len, indata),100),murmur32_hash(xcbuffer(len, indata),100));
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_void2)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13};
			u32 len=10,ru=murmur32_hash(xcbuffer(len, indata)),ru2;
			CHECK_EQUAL(murmur32_hash(xcbuffer(len, indata)),ru);
			ru2 = murmur32_hash(xcbuffer(len, indata),ru);
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len, indata)),ru2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_u322)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13};
			u32 len=10;
			for(s32 i=0;i<50;i++)
			{
				CHECK_EQUAL((murmur32_hash(xcbuffer(len, indata),i*i)>0),true);
			}
			CHECK_NOT_EQUAL(murmur32_hash(xcbuffer(len, indata)),murmur32_hash(xcbuffer(len, indata),100));
		}
	}
	
}
UNITTEST_SUITE_END