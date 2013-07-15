#include "xbase\x_target.h"
#include "xbase\x_string_std.h"
#include "xhash\x_hash64_generator.h"
#include "xhash\private\x_digest_murmur64.h"

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xhash64)
{
	UNITTEST_FIXTURE(xdigest_murmur64)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(xhash_generator_murmur_Hash)
		{
			s32 indata[10]={1,2,3,4,5,6,7,8,9,13},indata2[10]={-1,-2,-3,-4,-5,0,1,2,3,4};
			u32 len=10;
			u64 ruhash=xdigest_murmur64::buf(indata,len,100),ruhash2=xdigest_murmur64::buf(indata,len,100);
			CHECK_EQUAL(ruhash>0,true);
			CHECK_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur64::buf(indata,9,100);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur64::buf(indata2,len,100);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur64::buf(indata,len);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_void)
		{
			s32 indata[10]={1,2,3,4,5,6,7,8,9,13},indata2[10]={-1,-2,-3,-4,-5,0,1,2,3,4};
			u32 len=10,len2=23;
			u64 ru=xdigest_murmur64::buf(indata,len),ru2;
			CHECK_EQUAL(xdigest_murmur64::buf(indata,len),ru);
			ru2 = xdigest_murmur64::buf(indata,len,ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len),ru2);
			ru2 = xdigest_murmur64::buf(indata,len2,ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len),ru2);
			ru = xdigest_murmur64::buf(indata,len,ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len),ru2);
			ru2 = xdigest_murmur64::buf(indata2,len,ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len),ru2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_u32)
		{
			s32 indata[10]={1,2,3,4,5,6,7,8,9,13},indata2[10]={-1,-2,-3,-4,-5,0,1,2,3,4};
			u32 len=10,len2=23;
			for(s32 i=0;i<50;i++)
			{
				CHECK_EQUAL((xdigest_murmur64::buf(indata,len,i*i)>0),true);
			}
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len),xdigest_murmur64::buf(indata,len,100));
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len2,100),xdigest_murmur64::buf(indata,len,100));
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len,100),xdigest_murmur64::buf(indata2,len,100));
			CHECK_EQUAL(xdigest_murmur64::buf(indata,len,100),xdigest_murmur64::buf(indata,len,100));
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_void2)
		{
			u32 indata[10]={1,2,3,4,5,6,7,8,9,13};
			u32 len=10;
			u64 ru=xdigest_murmur64::buf(indata,len),ru2;
			CHECK_EQUAL(xdigest_murmur64::buf(indata,len),ru);
			ru2 = xdigest_murmur64::buf(indata,len,ru);
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len),ru2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_u322)
		{
			u32 indata[10]={1,2,3,4,5,6,7,8,9,13};
			u32 len=10;
			for(s32 i=0;i<50;i++)
			{
				CHECK_EQUAL((xdigest_murmur64::buf(indata,len,i*i)>0),true);
			}
			CHECK_NOT_EQUAL(xdigest_murmur64::buf(indata,len),xdigest_murmur64::buf(indata,len,100));
		}
		
	}
}
UNITTEST_SUITE_END