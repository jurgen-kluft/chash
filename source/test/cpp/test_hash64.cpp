#include "xbase/x_target.h"
#include "xhash/private/x_digest_murmur64.h"

#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xhash64)
{
	UNITTEST_FIXTURE(xdigest_murmur64)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(xhash_generator_murmur_Hash)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13},indata2[]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10;
			u64 ruhash  = xdigest_murmur64::buf(xcbuffer(len,indata),100);
			u64 ruhash2 = xdigest_murmur64::buf(xcbuffer(len,indata),100);
			CHECK_EQUAL(ruhash>0,true);
			CHECK_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur64::buf(xcbuffer(9,indata),100);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur64::buf(xcbuffer(len,indata2),100);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur64::buf(xcbuffer(len,indata));
			CHECK_NOT_EQUAL(ruhash,ruhash2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_void)
		{
			xbyte indata[10]={1,2,3,4,5,6,7,8,9,13},indata2[10]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10,len2=23;
			u64 ru=xdigest_murmur64::buf(xcbuffer(len, indata)),ru2;
			CHECK_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),ru);
			ru2 = xdigest_murmur64::buf(xcbuffer(len, indata),ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),ru2);
			ru2 = xdigest_murmur64::buf(xcbuffer(len2, indata),ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),ru2);
			ru = xdigest_murmur64::buf(xcbuffer(len, indata),ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),ru2);
			ru2 = xdigest_murmur64::buf(xcbuffer(len,indata2),ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),ru2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_u32)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13},indata2[]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10,len2=23;
			for(s32 i=0;i<50;i++)
			{
				CHECK_EQUAL((xdigest_murmur64::buf(xcbuffer(len, indata),i*i)>0),true);
			}
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),xdigest_murmur64::buf(xcbuffer(len, indata),100));
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len2, indata),100),xdigest_murmur64::buf(xcbuffer(len, indata),100));
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata),100),xdigest_murmur64::buf(xcbuffer(len, indata2),100));
			CHECK_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata),100),xdigest_murmur64::buf(xcbuffer(len, indata),100));
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_void2)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13};
			u32 len=10;
			u64 ru=xdigest_murmur64::buf(xcbuffer(len, indata)),ru2;
			CHECK_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),ru);
			ru2 = xdigest_murmur64::buf(xcbuffer(len, indata),ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),ru2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_u322)
		{
			xbyte indata[]={1,2,3,4,5,6,7,8,9,13};
			u32 len=10;
			for(s32 i=0;i<50;i++)
			{
				CHECK_EQUAL((xdigest_murmur64::buf(xcbuffer(len, indata),i*i)>0),true);
			}
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(xcbuffer(len, indata)),xdigest_murmur64::buf(xcbuffer(len, indata),100));
		}
		
	}
}
UNITTEST_SUITE_END