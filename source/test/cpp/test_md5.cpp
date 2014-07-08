#include "xbase\x_target.h"
#include "xhash\x_md5.h"

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xmd5)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(xmd5_operator_equal)
		{
			xmd5 smd5h;
			for(s32 i=0;i<50;i++)
			{
				xmd5 smd5h2;
				smd5h.set(i+1,i+2,i+3,i+4);
				smd5h2.set(i+1,i+2,i+3,i+4);
				bool sEqual=(smd5h==smd5h2);
				CHECK_EQUAL(sEqual,true);
				smd5h2.set(i+1,i+2,i+3,i+1234);
				sEqual=(smd5h==smd5h2);
				CHECK_EQUAL(sEqual,false);
				smd5h2.set(i+1,i+2,i+123,i+4);
				sEqual=(smd5h==smd5h2);
				CHECK_EQUAL(sEqual,false);
				smd5h2.set(i+1,i+12,i+3,i+4);
				sEqual=(smd5h==smd5h2);
				CHECK_EQUAL(sEqual,false);
				smd5h2.set(i+11,i+2,i+3,i+4);
				sEqual=(smd5h==smd5h2);
				CHECK_EQUAL(sEqual,false);
			}
		}
		UNITTEST_TEST(xmd5_operator_not_equal)
		{
			xmd5 smd5h;
			for(s32 i=0;i<50;i++)
			{
				xmd5 smd5h2;
				smd5h.set(i,i,i,i);
				smd5h2.set(i+1,i+2,i+3,i+4);
				bool sEqual=(smd5h!=smd5h2);
				CHECK_EQUAL(sEqual,true);
				smd5h2.set(i,i,i,i);
				sEqual=(smd5h!=smd5h2);
				CHECK_EQUAL(sEqual,false);
			}
		}
		UNITTEST_TEST(xmd5_operator_max)
		{
			xmd5 smd5h,smd5h2,smd5h3;
			const char* rch ="000002222222222222222222222222222222222222";
			const char* rch2="111111111111111111111111111111111111111111";
			const char* rch3="111111111111111111111111122222211111111111";
			CHECK_TRUE(smd5h.fromString(rch));
			CHECK_TRUE(smd5h2.fromString(rch2));
			bool sBig=(smd5h2>smd5h);
			CHECK_EQUAL(sBig,true);
			smd5h3.fromString(rch3);
			sBig=(smd5h3>smd5h);
			CHECK_EQUAL(sBig,true);
		}
		UNITTEST_TEST(xmd5_operator_min)
		{
			xmd5 smd5h,smd5h2;
			const char* rch="2222222222222222222222222222222222222";
			char* rch2="111111111111111111111111111111111111111111111111111111111111";
			char* rch3="33333333333333333333333333333333333333333333333333333333";
			smd5h.fromString(rch);
			smd5h2.fromString(rch2);
			bool sBig=(smd5h2<smd5h);
			CHECK_EQUAL(sBig,true);
			smd5h2.fromString(rch3);
			sBig=(smd5h2<smd5h);
			CHECK_EQUAL(sBig,false);
		}
		UNITTEST_TEST(xmd5_SetMD5)
		{
			xmd5 smd5h,smd5h2,smd5h3;
			for(s32 i=0;i<50;i++)
			{
				smd5h.set(i,i+1,i+2,i+3);
				smd5h2.set(i,i+1,i+2,i+3);
				smd5h3.set(i,i,i+2,i+3);
				CHECK_EQUAL((smd5h==smd5h2),true);
				CHECK_EQUAL((smd5h3!=smd5h2),true);
			}
		}
		UNITTEST_TEST(xmd5_GetMD5)
		{
			xmd5 smd5h;
			for(s32 i=1;i<50;i++)
			{
				u32 ru,ru2,ru3,ru4;
				smd5h.set(i,i,i,i);
				smd5h.get(ru,ru2,ru3,ru4);
				CHECK_EQUAL(ru,i);
				CHECK_EQUAL(ru4,i);
			}
		}
		UNITTEST_TEST(xmd5_GetHash)
		{
			xmd5 smd5h;
			for(s32 i=10;i<50;i++)
			{
				u32 ru,ru2,ru3,ru4;
				smd5h.set(i,i,i,i);
				smd5h.get(ru,ru2,ru3,ru4);
				CHECK_EQUAL((smd5h.getHash32()==ru),true);
			}
		}
		UNITTEST_TEST(xmd5_ToString)
		{
			char rch[100];
			xmd5 smd5h;
			s32 sTOStr=smd5h.toString(rch, 100-1);
			CHECK_EQUAL(sTOStr,16*2);
		}
		UNITTEST_TEST(xmd5_FromString)
		{
			char const* rch = "da39a3ee5e6b4b0d3255bfef95601890";
			xmd5 smd5h;
			
			bool sFromStr = smd5h.fromString(rch);
			CHECK_EQUAL(sFromStr, true);
			
			sFromStr = smd5h.fromString("too short");
			CHECK_EQUAL(sFromStr, false);
		}
	}
	UNITTEST_FIXTURE(generator)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Open)
		{
			xdigest_engine_md5 smd5,smd52;
			u32 ru[4];
			u32 ruu[4];
			smd5.reset();
			xmd5 smd5h; smd5.digest(smd5h);
			xmd5 smd5h2; smd52.digest(smd5h2);
			
			
			smd5h.get(ru[0], ru[1], ru[2], ru[3]);
			smd5h2.get(ruu[0], ruu[1],ruu[2],ruu[3]);
			
			CHECK_TRUE( ru[0] != ruu[0] || ru[1] != ruu[1] || ru[2] != ruu[2] || ru[3] != ruu[3]);

			CHECK_EQUAL(smd5h==smd5h2,false);
			smd52.reset();
			smd52.digest(smd5h2);
			smd5h2.get(ruu[0], ruu[1],ruu[2],ruu[3]);
			CHECK_EQUAL(smd5h==smd5h2,true);
		}
		UNITTEST_TEST(Compute)
		{
			s32 inBuffer[10]={1,1,2,3,5,8,13,21,34,55};
			s32	inLength=10;
			xdigest_engine_md5 smd5;
			smd5.reset();
			smd5.update(inBuffer,inLength);
		}
		UNITTEST_TEST(Digest)
		{
			xdigest_engine_md5 smd5,smd52;
			u32 ru,ru2,ru3,ru4;
			u32 ruu,ruu2,ruu3,ruu4;
			smd5.reset();
			xmd5 smd5h("a unique string");  
			smd5.digest(smd5h);
			xmd5 smd5h2("a different string"); 
			smd52.digest(smd5h2);
			smd5h.get(ru,ru2,ru3,ru4);
			smd5h2.get(ruu,ruu2,ruu3,ruu4);
			CHECK_NOT_EQUAL(ru,ruu);
			CHECK_NOT_EQUAL(ru3,ruu3);
			smd52.reset();
			smd52.digest(smd5h2);
			smd5h2.get(ruu,ruu2,ruu3,ruu4);
			CHECK_EQUAL(ru,ruu);
			CHECK_EQUAL(ru3,ruu3);
		}
	}
}
UNITTEST_SUITE_END