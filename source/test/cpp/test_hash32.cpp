#include "xbase\x_target.h"
#include "xhash\x_digest32.h"
#include "xhash\private\x_digest_murmur32.h"

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xhash32)
{
	UNITTEST_FIXTURE(xdigest_murmur32)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(xhash_generator_murmur_Hash)
		{
			int indata[10]={1,2,3,4,5,6,7,8,9,13},indata2[10]={-1,-2,-3,-4,-5,0,1,2,3,4};
			u32 len=10;
			u32 ruhash=xdigest_murmur32::buf(indata,len,100),ruhash2=xdigest_murmur32::buf(indata,len,100);
			CHECK_EQUAL(ruhash>0,true);
			CHECK_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur32::buf(indata,9,100);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur32::buf(indata2,len,100);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
			ruhash2=xdigest_murmur32::buf(indata,len);
			CHECK_NOT_EQUAL(ruhash,ruhash2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_void)
		{
			int indata[10]={1,2,3,4,5,6,7,8,9,13},indata2[10]={-1,-2,-3,-4,-5,0,1,2,3,4};
			u32 len=10,len2=23,ru=xdigest_murmur32::buf(indata,len),ru2;
			CHECK_EQUAL(xdigest_murmur32::buf(indata,len),ru);
			ru2 = xdigest_murmur32::buf(indata,len,ru);
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len),ru2);
			ru2 = xdigest_murmur32::buf(indata,len2,ru);
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len),ru2);
			ru = xdigest_murmur32::buf(indata,len,ru);
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len),ru2);
			ru2 = xdigest_murmur32::buf(indata2,len,ru);
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len),ru2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_u32)
		{
			int indata[10]={1,2,3,4,5,6,7,8,9,13},indata2[10]={-1,-2,-3,-4,-5,0,1,2,3,4};
			u32 len=10,len2=23;
			for(s32 i=0;i<50;i++)
			{
				CHECK_EQUAL((xdigest_murmur32::buf(indata,len,i*i)>0),true);
			}
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len),xdigest_murmur32::buf(indata,len,100));
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len2,100),xdigest_murmur32::buf(indata,len,100));
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len,100),xdigest_murmur32::buf(indata2,len,100));
			CHECK_EQUAL(xdigest_murmur32::buf(indata,len,100),xdigest_murmur32::buf(indata,len,100));
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_void2)
		{
			u32 indata[10]={1,2,3,4,5,6,7,8,9,13};
			u32 len=10,ru=xdigest_murmur32::buf(indata,len),ru2;
			CHECK_EQUAL(xdigest_murmur32::buf(indata,len),ru);
			ru2 = xdigest_murmur32::buf(indata,len,ru);
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len),ru2);
		}
		UNITTEST_TEST(xhash_generator_murmur_Hash2_u322)
		{
			u32 indata[10]={1,2,3,4,5,6,7,8,9,13};
			u32 len=10;
			for(s32 i=0;i<50;i++)
			{
				CHECK_EQUAL((xdigest_murmur32::buf(indata,len,i*i)>0),true);
			}
			CHECK_NOT_EQUAL(xdigest_murmur32::buf(indata,len),xdigest_murmur32::buf(indata,len,100));
		}
		
	}
	UNITTEST_FIXTURE(hashable)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}
		UNITTEST_TEST(hashable_operator_assignment)
		{
			hashable<u32> shashs;
			shashs=(123);
			CHECK_EQUAL(123,(u32 &)shashs);
			hashable<f32> shashf;
			shashf=(123.456f);
			CHECK_EQUAL(123.456f,(f32 &)shashf);
			hashable<char> shashc;
			shashc=('x');
			CHECK_EQUAL('x',(char &)shashc);
		}
		UNITTEST_TEST(hashable_operator_get)
		{
			hashable<u32> shashs;
			shashs=(496513);
			CHECK_EQUAL(496513,(u32 &)shashs);
			hashable<f32> shashf;
			shashf=(65413.351f);
			CHECK_EQUAL(65413.351f,(f32 &)shashf);
			hashable<char> shashc;
			shashc=('s');
			CHECK_EQUAL('s',(char &)shashc);
		}
		UNITTEST_TEST(hashable_operator_small)
		{
			hashable<u32> shashs;
			hashable<f32> shashf;
			hashable<char> shashc;
			shashs=(891219);
			CHECK_EQUAL(shashs<(u32 &)shashs,false);
			//CHECK_EQUAL(shashs<((f32 &)shashs),true);
			shashf=(132.456f);
			CHECK_EQUAL(shashf<(f32 &)shashf,false);
			shashc=('a');
			CHECK_EQUAL(shashc<(char &)shashc,false);
		}
		UNITTEST_TEST(hashable_operator_smallequal)
		{
			hashable<u32> shashs;
			hashable<f32> shashf;
			hashable<char> shashc;
			shashs=(891219);
			CHECK_EQUAL(shashs<=(u32 &)shashs,true);
			CHECK_EQUAL(shashs<=((u32 &)shashs-1),false);
			shashf=(132.456f);
			CHECK_EQUAL(shashf<=(f32 &)shashf,true);
			shashc=('a');
			CHECK_EQUAL(shashc<=(char &)shashc,true);
		}
		UNITTEST_TEST(hashable_operator_equal)
		{
			hashable<u32> shashs;
			hashable<f32> shashf;
			hashable<char> shashc;
			shashs=(891219);
			CHECK_EQUAL(shashs==(u32 &)shashs,true);
			CHECK_EQUAL(shashs==((u32 &)shashs-1),false);
			shashf=(132.456f);
			CHECK_EQUAL(shashf==(f32 &)shashf,true);
			shashc=('a');
			CHECK_EQUAL(shashc==(char &)shashc,true);
		}
		UNITTEST_TEST(hashable_operator_not_equal)
		{
			hashable<u32> shashs;
			hashable<f32> shashf;
			hashable<char> shashc;
			CHECK_EQUAL(shashs!=(u32 &)shashs,false);
			CHECK_EQUAL(shashs!=((u32 &)shashs-1),true);
			shashf=(132.456f);
			CHECK_EQUAL(shashf!=(f32 &)shashf,false);
			shashc=('a');
			CHECK_EQUAL(shashc!=(char &)shashc,false);
			CHECK_EQUAL(shashc!=(char)((char &)shashc-1),true);
		}
		UNITTEST_TEST(hashable_operator_big_equal)
		{
			hashable<u32> shashs;
			hashable<f32> shashf;
			hashable<char> shashc;
			shashs=(891219);
			CHECK_EQUAL(shashs>=(u32)(891219),true);
			shashf=(132.456f);
			CHECK_EQUAL(shashf>=(f32 &)shashf,true);
			CHECK_EQUAL(shashf>=((f32 &)shashf+1),false);
			shashc=('a');
			CHECK_EQUAL(shashc>=(char &)shashc,true);
			CHECK_EQUAL(shashc>=(char)((char &)shashc+1),false);
		}
		UNITTEST_TEST(hashable_operator_big)
		{
			hashable<u32> shashs;
			hashable<f32> shashf;
			hashable<char> shashc;
			shashs=(891219);
			CHECK_EQUAL(shashs>((u32 &)shashs),false);
			shashf=((f32)132.456);
			CHECK_EQUAL(shashf>((f32 &)shashf),false);
			CHECK_EQUAL(shashf>((f32 &)shashf-1),true);
			shashc=('z');
			CHECK_EQUAL(shashc>((char &)shashc),false);
			CHECK_EQUAL(shashc>(char)((char &)shashc-1),true);
		}
		UNITTEST_TEST(hashable_operator_small2)
		{
			hashable<u32> shashs,shashs2;
			shashs=(123);
			shashs2=(65132);
			CHECK_EQUAL(shashs<(shashs2),true);
			shashs2=(123);
			CHECK_EQUAL(shashs<(shashs2),false);
			hashable<f32> shashf,shashf2;
			shashf=((f32)123.456);
			shashf2=((f32)789.456);
			CHECK_EQUAL(shashf<(shashf2),true);
			shashf2=((f32)123.456);
			CHECK_EQUAL(shashf<(shashf2),false);
			hashable<char> shashc,shashc2;
			shashc=('a');
			shashc2=('x');
			CHECK_EQUAL(shashc<(shashc2),true);
			shashc2=('a');
			CHECK_EQUAL(shashc<(shashc2),false);
		}
		UNITTEST_TEST(hashable_operator_small_equal2)
		{
			hashable<u32> shashs,shashs2;
			shashs=(123);
			shashs2=(65132);
			CHECK_EQUAL(shashs<=(shashs2),true);
			shashs2=(123);
			CHECK_EQUAL(shashs<=(shashs2),true);
			hashable<f32> shashf,shashf2;
			shashf=((f32)123.456);
			shashf2=((f32)789.456);
			CHECK_EQUAL(shashf<=(shashf2),true);
			shashf2=((f32)123.456);
			CHECK_EQUAL(shashf<=(shashf2),true);
			hashable<char> shashc,shashc2;
			shashc=('a');
			shashc2=('x');
			CHECK_EQUAL(shashc<=(shashc2),true);
			shashc2=('a');
			CHECK_EQUAL(shashc<=(shashc2),true);
		}
		UNITTEST_TEST(hashable_operator_equal2)
		{
			hashable<u32> shashs,shashs2;
			shashs=(8645);
			shashs2=(65132);
			CHECK_EQUAL(shashs==(shashs2),false);
			shashs2=(8645);
			CHECK_EQUAL(shashs==(shashs2),true);
			hashable<f32> shashf,shashf2;
			shashf=((f32)684513.52);
			shashf2=((f32)789.456);
			CHECK_EQUAL(shashf==(shashf2),false);
			shashf2=((f32)684513.52);
			CHECK_EQUAL(shashf==(shashf2),true);
			hashable<char> shashc,shashc2;
			shashc=('a');
			shashc2=('x');
			CHECK_EQUAL(shashc==(shashc2),false);
			shashc2=('a');
			CHECK_EQUAL(shashc==(shashc2),true);
		}
		UNITTEST_TEST(hashable_operator_not_equal2)
		{
			hashable<u32> shashs,shashs2;
			shashs=(98645132);
			shashs2=(98645132);
			CHECK_EQUAL(shashs!=(shashs2),false);
			shashs2=(8645);
			CHECK_EQUAL(shashs!=(shashs2),true);
			hashable<f32> shashf,shashf2;
			shashf=((f32)9841532.1);
			shashf2=((f32)9841532.1);
			CHECK_EQUAL(shashf!=(shashf2),false);
			shashf2=((f32)684513.52);
			CHECK_EQUAL(shashf!=(shashf2),true);
			hashable<char> shashc,shashc2;
			shashc=('x');
			shashc2=('x');
			CHECK_EQUAL(shashc!=(shashc2),false);
			shashc2=('a');
			CHECK_EQUAL(shashc!=(shashc2),true);
		}
		UNITTEST_TEST(hashable_operator_bige_qual2)
		{
			hashable<u32> shashs,shashs2;
			shashs=(645132);
			shashs2=(645132);
			CHECK_EQUAL(shashs>=(shashs2),true);
			shashs2=(8645);
			CHECK_EQUAL(shashs>=(shashs2),true);
			shashs2=(87451261);
			CHECK_EQUAL(shashs>=(shashs2),false);
			hashable<f32> shashf,shashf2;
			shashf=((f32)9841532.1);
			shashf2=((f32)9841532.1);
			CHECK_EQUAL(shashf>=(shashf2),true);
			shashf2=((f32)684513.52);
			CHECK_EQUAL(shashf>=((f32)shashf2),true);
			hashable<char> shashc,shashc2;
			shashc=('x');
			shashc2=('x');
			CHECK_EQUAL(shashc>=(shashc2),true);
			shashc2=('a');
			CHECK_EQUAL(shashc>=(shashc2),true);
		}
		UNITTEST_TEST(hashable_operator_big2)
		{
			hashable<u32> shashs,shashs2;
			shashs=(645132);
			shashs2=(645132);
			CHECK_EQUAL(shashs>(shashs2),false);
			shashs2=(8645);
			CHECK_EQUAL(shashs>(shashs2),true);
			hashable<f32> shashf,shashf2;
			shashf=((f32)9841532.1);
			shashf2=((f32)9841532.1);
			CHECK_EQUAL(shashf>(shashf2),false);
			shashf2=((f32)684513.52);
			CHECK_EQUAL(shashf>(shashf2),true);
			shashf2=((f32)9841532.123);
			CHECK_EQUAL(shashf>(shashf2),false);
			hashable<char> shashc,shashc2;
			shashc=('x');
			shashc2=('x');
			CHECK_EQUAL(shashc>(shashc2),false);
			shashc2=('a');
			CHECK_EQUAL(shashc>(shashc2),true);
		}
		UNITTEST_TEST(hashable_operator_plus_equal)
		{
			hashable<u32> shashs;
			shashs=(123);
			CHECK_EQUAL(123+456,shashs+=((u32)456));
			hashable<f32> shashf;
			shashf=(123.4f);
			CHECK_EQUAL((123.4f+1.2f),shashf+=(1.2f));
			hashable<char> shashc;
			shashc=('x');
			CHECK_EQUAL((char)('x'+'s'),shashc+=('s'));
		}
		UNITTEST_TEST(hashable_operator_minus_equal)
		{
			hashable<u32> shashs;
			shashs=(123);
			CHECK_EQUAL(123-456,shashs-=((u32)456));
			hashable<f32> shashf;
			shashf=((f32)123.4);
			CHECK_EQUAL((f32)(123.4-1.3),shashf-=(1.3f));
			hashable<char> shashc;
			shashc=('x');
			CHECK_EQUAL((char)('x'-'s'),shashc-=('s'));
		}
		UNITTEST_TEST(hashable_operator_multiply_equal)
		{
			hashable<u32> shashs;
			shashs=(123);
			CHECK_EQUAL(123*456,shashs*=((u32)456));
			hashable<f32> shashf;
			shashf=((f32)123.4);
			CHECK_EQUAL((f32)(123.4*1.2),shashf*=(1.2f));
			hashable<char> shashc;
			shashc=('x');
			CHECK_EQUAL((char)('x'*'s'),shashc*=('s'));
		}
		UNITTEST_TEST(hashable_operator_division_equal)
		{
			hashable<u32> shashs;
			shashs=(123);
			CHECK_EQUAL(123/456,shashs/=((u32)456));
			hashable<f32> shashf;
			shashf=((f32)123.44);
			CHECK_EQUAL((f32)(123.44/1.2f),shashf/=(1.2f));
			hashable<char> shashc;
			shashc=('x');
			CHECK_EQUAL((char)('x'/'s'),shashc/=('s'));
		}
		UNITTEST_TEST(hashable_operator_plus)
		{
			hashable<u32> shashs;
			shashs=(6541);
			CHECK_EQUAL(6541+54,shashs+((u32)54));
			hashable<f32> shashf;
			shashf=(123.4f);
			CHECK_EQUAL((123.4f+1.56f),shashf+(1.56f));
			hashable<char> shashc;
			shashc=('q');
			CHECK_EQUAL((char)('q'+'a'),shashc+('a'));
		}
		UNITTEST_TEST(hashable_operator_minus)
		{
			hashable<u32> shashs;
			shashs=(6541);
			CHECK_EQUAL(6541-321,shashs-((u32)321));
			hashable<f32> shashf;
			shashf=(3213.65f);
			CHECK_EQUAL((3213.65f-1.56f),shashf-(1.56f));
			hashable<char> shashc;
			shashc=('q');
			CHECK_EQUAL((char)('q'-'a'),shashc-('a'));
		}
		UNITTEST_TEST(hashable_operator_multiply)
		{
			hashable<u32> shashs;
			shashs=(6541);
			CHECK_EQUAL(6541*54,shashs*((u32)54));
			hashable<f32> shashf;
			shashf=(123.4f);
			CHECK_EQUAL((123.4f*1.56f),shashf*(1.56f));
			hashable<char> shashc;
			shashc=('q');
			CHECK_EQUAL((char)('q'*'a'),shashc*('a'));
		}
		UNITTEST_TEST(hashable_operator_division)
		{
			hashable<u32> shashs;
			shashs=(6541);
			CHECK_EQUAL(6541/54,shashs/((u32)54));
			hashable<f32> shashf;
			shashf=(123.4f);
			CHECK_EQUAL((123.4f/1.56f),shashf/(1.56f));
			hashable<char> shashc;
			shashc=('q');
			CHECK_EQUAL((char)('q'/'a'),shashc/('a'));
		}
		UNITTEST_TEST(hashable_hash)
		{
			hashable<u32> shashs;
			shashs=(123);
			u32 sum=123;
			for(u32 i=0;i<50;i++)
			{
				shashs+=(450*i);
				sum+=450*i;
				CHECK_EQUAL(xdigest_murmur32::buf(&sum, sizeof(u32)),shashs.hash());
			}
			hashable<f32> shashf;
			shashf=(123.4f);
			shashf+=(1.2f);
			f32 *rf;
			f32 rf2=(f32 &)shashf;
			rf=&rf2;
			CHECK_EQUAL(xdigest_murmur32::buf(rf, sizeof(f32)), shashf.hash());
			hashable<char> shashc;
			shashc=('x');
			shashc+=('s');
			char *rc;
			char rc2=(char &)shashc;
			rc=&rc2;
			CHECK_EQUAL(xdigest_murmur32::buf(rc,sizeof(char)), shashc.hash());
		}
	}
	UNITTEST_FIXTURE(hashed_ptr)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}
		UNITTEST_TEST(hashed_ptr_equal)
		{
			hashed_ptr<u32> sptru,sptru2;
			u32 a,b;
			u32 *p,*q;
			p=&a;q=&b;
			sptru = p;
			sptru2 = q;
			bool nEqual=sptru==(sptru2);
			CHECK_EQUAL(false,nEqual);
			hashed_ptr<f32> sptrf,sptrf2;
			f32 a2,b2;
			f32 *p2,*q2;
			p2=&a2;q2=&b2;
			sptrf = p2;
			sptrf2 = q2;
			bool nEqual2=sptrf==(sptrf2);
			CHECK_EQUAL(false,nEqual2);
			sptrf2 = p2;
			bool nEqual3=sptrf==(sptrf2);
			CHECK_EQUAL(true,nEqual3);
			hashed_ptr<char> sptrc,sptrc2;
			char a3,b3;
			char *p3,*q3;
			p3=&a3;q3=&b3;
			sptrc = p3;
			sptrc2 = q3;
			bool nEqual4=sptrc==(sptrc2);
			CHECK_EQUAL(false,nEqual4);
		}
		UNITTEST_TEST(hashed_ptr_not_equal)
		{
			hashed_ptr<u32> sptru,sptru2;
			u32 a,b;
			u32 *p,*q;
			p=&a;q=&b;
			sptru = p;
			sptru2 = q;
			bool nEqual=sptru!=(sptru2);
			CHECK_EQUAL(true,nEqual);
			hashed_ptr<f32> sptrf,sptrf2;
			f32 a2,b2;
			f32 *p2,*q2;
			p2=&a2;q2=&b2;
			sptrf = p2;
			sptrf2 = q2;
			bool nEqual2=sptrf!=(sptrf2);
			CHECK_EQUAL(true,nEqual2);
			sptrf2 = p2;
			bool nEqual3=sptrf!=(sptrf2);
			CHECK_EQUAL(false,nEqual3);
			hashed_ptr<char> sptrc,sptrc2;
			char a3,b3;
			char *p3,*q3;
			p3=&a3;q3=&b3;
			sptrc = p3;
			sptrc2 = q3;
			bool nEqual4=sptrc!=(sptrc2);
			CHECK_EQUAL(true,nEqual4);
		}
		UNITTEST_TEST(hashed_ptr_assignment_struct)
		{
			hashed_ptr<u32> sptru,sptru2;
			u32 a=46131,*p;
			p=&a;
			sptru = p;
			sptru.hash(327558);
			sptru2=(sptru);
			CHECK_EQUAL(sptru.ptr(),sptru2.ptr());
			hashed_ptr<f32> sptrf,sptrf2=(sptrf);
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrf = p2;
			sptrf.hash(327558);
			sptrf2=(sptrf);
			CHECK_EQUAL(327558,sptrf2.hash());
			CHECK_EQUAL(true,sptrf2==(sptrf));
			hashed_ptr<char> sptrc,sptrc2=(sptrc);
			char a3='s',*p3;
			p3=&a3;
			sptrc = p3;
			sptrc.hash(327558);
			sptrc2=(sptrc);
			CHECK_EQUAL(true,sptrc2==(sptrc));
		}
		UNITTEST_TEST(hashed_ptr_assignment_T)
		{
			hashed_ptr<u32> sptru;
			u32 a=46131,*p;
			p=&a;
			sptru=(p);
			CHECK_EQUAL(sptru.ptr(),p);
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrf=(p2);
			CHECK_EQUAL(sptrf.ptr(),p2);


		}
		UNITTEST_TEST(hashed_ptr_get_mPtr)
		{
			hashed_ptr<u32> sptru;
			u32 a=46131,*p;
			p=&a;
			sptru=(p);
			CHECK_EQUAL(sptru.ptr(),sptru.ptr());
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrf=(p2);
			CHECK_EQUAL(sptrf.ptr(),sptrf.ptr());
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrc=(p3);
			CHECK_EQUAL(sptrc.ptr(),sptrc.ptr());
		}
		UNITTEST_TEST(hashed_ptr_get_mHash)
		{
			hashed_ptr<u32> sptru;
			u32 a=65631651,*p;
			p=&a;
			sptru=(p);
			CHECK_EQUAL(sptru.hash(),sptru.hash());
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrf=(p2);
			CHECK_EQUAL(sptrf.hash(),sptrf.hash());
			hashed_ptr<char> sptrc;
			char a3='d',*p3;
			p3=&a3;
			sptrc=(p3);
			CHECK_EQUAL(sptrc.hash(),sptrc.hash());
		}
		UNITTEST_TEST(hashed_ptr_vector)
		{
			hashed_ptr<u32> sptru;
			u32 a=46131;
			u32 *p;
			p=&a;
			sptru=(p);
			u32 *q=(&sptru)->ptr();
			CHECK_EQUAL(q,p);
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrf=(p2);
			f32 *q2=(&sptrf)->ptr();
			CHECK_EQUAL(q2,p2);
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrc=(p3);
			char *q3=(&sptrc)->ptr();
			CHECK_EQUAL(q3,p3);
		}
		UNITTEST_TEST(hashed_ptr_const_vector)
		{
			hashed_ptr<u32> sptru;
			u32 a=46131;
			u32 *p;
			p=&a;
			sptru=(p);
			const u32 *q=(&sptru)->ptr();
			CHECK_EQUAL(q,p);
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrf=(p2);
			const f32 *q2=(&sptrf)->ptr();
			CHECK_EQUAL(q2,p2);
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrc=(p3);
			const char *q3=(&sptrc)->ptr();
			CHECK_EQUAL(q3,p3);
		}
		UNITTEST_TEST(hashed_ptr_point)
		{
			hashed_ptr<u32> sptru;
			hashed_ptr<u32> *sptru2=&sptru;
			u32 a=46131,*p;
			p=&a;
			sptru=(p);
			u32 *q;
			q=*sptru2;
			CHECK_EQUAL(q,sptru);
			hashed_ptr<f32> sptrf;
			hashed_ptr<f32> *sptrf2=&sptrf;
			f32 a2=46131.654f;
			f32 *p2,*q2;
			p2=&a2;
			sptrf=(p2);
			q2=*sptrf2;
			CHECK_EQUAL(q2,sptrf);
			hashed_ptr<char> sptrc;
			hashed_ptr<char> *sptrc2=&sptrc;
			char a3='s',*p3,*q3;
			p3=&a3;
			sptrc=(p3);
			q3=*sptrc2;
			CHECK_EQUAL(q3,sptrc);
		}
		UNITTEST_TEST(hashed_ptr_const_point)
		{
			hashed_ptr<u32> sptru;
			const hashed_ptr<u32> *sptru2=&sptru;
			u32 a=46131,*p;
			p=&a;
			sptru=(p);
			const u32 *q;
			q=*sptru2;
			CHECK_EQUAL(q,sptru);
			hashed_ptr<f32> sptrf;
			const hashed_ptr<f32> *sptrf2=&sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			const f32 *q2;
			p2=&a2;
			sptrf=(p2);
			q2=*sptrf2;
			CHECK_EQUAL(q2,sptrf);
			hashed_ptr<char> sptrc;
			hashed_ptr<char> *sptrc2=&sptrc;
			char a3='s',*p3;
			const char *q3;
			p3=&a3;
			sptrc=(p3);
			q3=*sptrc2;
			CHECK_EQUAL(q3,sptrc)
		}
		UNITTEST_TEST(hashed_ptr_T_point)
		{
			hashed_ptr<u32> sptru;
			u32 a=46131,*p;
			p=&a;
			sptru=(p);
			u32 *q=(u32 *)sptru;
			CHECK_EQUAL(q,sptru.ptr());
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrf=(p2);
			f32 *q2=(f32 *)sptrf;
			CHECK_EQUAL(q2,sptrf.ptr());
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrc=(p3);
			char *q3=(char *)sptrc;
			CHECK_EQUAL(q3,sptrc.ptr());
		}
		UNITTEST_TEST(hashed_ptr_const_T_point)
		{
			hashed_ptr<u32> sptru;
			u32 a=46131,*p;
			p=&a;
			sptru=(p);
			const u32 *q=(const u32 *)sptru;
			CHECK_EQUAL(q,sptru.ptr());
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrf=(p2);
			f32 const *q2=(const f32 *)sptrf;
			CHECK_EQUAL(q2,sptrf.ptr());
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrc=(p3);
			char const *q3=(const char *)sptrc;
			CHECK_EQUAL(q3,sptrc.ptr());
		}
	}
	UNITTEST_FIXTURE(hashed_void_ptr)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(hashed_void_ptr_assignment_struct)
		{
			hashed_void_ptr sptru,sptru2;
			sptru.hash(46131);
			s32 *ps,as=351;
			f32 *pf,af=10.0f;
			char *pc,ac='a';
			sptru.hash(327558);
			ps=&as;
			sptru = ps;
			sptru2=(sptru);
			CHECK_EQUAL(sptru.ptr(),sptru2.ptr());
			CHECK_EQUAL(sptru.hash(),sptru2.hash());
			pf=&af;
			sptru = pf;
			sptru2=(sptru);
			CHECK_EQUAL(sptru.ptr(),sptru2.ptr());
			pc=&ac;
			sptru = pc;
			sptru2=(sptru);
			CHECK_EQUAL(sptru.ptr(),sptru2.ptr());
		}
		UNITTEST_TEST(hashed_void_ptr_assignment_index)
		{
			hashed_void_ptr sptru;
			sptru.hash(46131);
			s32 *ps,as=351;
			f32 *pf,af=10.0f;
			char *pc,ac='a';
			sptru.hash(327558);
			ps=&as;
			sptru=(ps);
			CHECK_EQUAL(sptru.ptr(),ps);
			pf=&af;
			sptru=(pf);
			CHECK_EQUAL(sptru.ptr(),pf);
			pc=&ac;
			sptru=(pc);
			CHECK_EQUAL(sptru.ptr(),pc);
		}
		UNITTEST_TEST(hashed_void_ptr_equal)
		{
			hashed_void_ptr sptru,sptru2;
			sptru.hash(46131);
			s32 *ps,as=351;
			f32 *pf,af=10.0f;
			char *pc,ac='z',*pc2,ac2='a';
			sptru.hash(327558);
			ps=&as;
			sptru=(ps);
			sptru2=(sptru);
			bool sSmallEqual = (sptru==sptru2);
			CHECK_TRUE(sSmallEqual);
			pf=&af;
			sptru=(pf);
			sptru2=(sptru);
			bool sSmallEqual2 = sptru==sptru2;
			CHECK_TRUE(sSmallEqual2);
			pc=&ac;
			pc2=&ac2;
			sptru=(pc);
			sptru2=(pc2);
			bool sSmallEqual3=sptru==sptru2;
			CHECK_FALSE(sSmallEqual3);
		}
		UNITTEST_TEST(hashed_void_ptr_not_equal)
		{
			hashed_void_ptr sptru,sptru2;
			sptru.hash(46131);
			s32 *ps,as=5413551;
			f32 *pf,af=10.0f;
			char *pc,ac='z',*pc2,ac2='a';
			sptru.hash(327558);
			ps=&as;
			sptru=(ps);
			sptru2=(sptru);
			bool sSmallEqual=sptru!=sptru2,SS=sptru.ptr()!=sptru2.ptr();
			CHECK_EQUAL(sSmallEqual,SS);
			pf=&af;
			sptru=(pf);
			sptru2=(sptru);
			bool sSmallEqual2=sptru!=sptru2,SS2=sptru.ptr()!=sptru2.ptr();
			CHECK_EQUAL(sSmallEqual2,SS2);
			pc=&ac;
			pc2=&ac2;
			sptru=(pc);
			sptru2=(pc2);
			bool sSmallEqual3=sptru!=sptru2,SS3=sptru.ptr()!=sptru2.ptr();
			CHECK_EQUAL(sSmallEqual3,SS3);
		}
		UNITTEST_TEST(hashed_void_ptr_small)
		{
			hashed_void_ptr sptru,sptru2;
			sptru.hash(46131);
			s32 *ps,as=351;
			f32 *pf,af=10.0f;
			char *pc,ac='z',*pc2,ac2='a';
			sptru.hash(327558);
			ps=&as;
			sptru=(ps);
			sptru2=(sptru);
			bool sSmall=sptru<sptru2,SS=sptru.ptr()<sptru2.ptr();
			CHECK_EQUAL(sSmall,SS);
			pf=&af;
			sptru=(pf);
			sptru2=(sptru);
			bool sSmall2=sptru<=sptru2,SS2=sptru.ptr()<=sptru2.ptr();
			CHECK_EQUAL(sSmall2,SS2);
			pc=&ac;
			pc2=&ac2;
			sptru=(pc);
			sptru2=(pc2);
			bool sSmall3=sptru<=sptru2,SS3=sptru.ptr()<=sptru2.ptr();
			CHECK_EQUAL(sSmall3,SS3);
		}
		UNITTEST_TEST(hashed_void_ptr_small_equal)
		{
			hashed_void_ptr sptru,sptru2;
			sptru.hash(46131);
			s32 *ps,as=351;
			f32 *pf,af=10.0f;
			char *pc,ac='z',*pc2,ac2='a';
			sptru.hash(327558);
			ps=&as;
			sptru=(ps);
			sptru2=(sptru);
			bool sSmallEqual=sptru<=sptru2,SS=sptru.ptr()<=sptru2.ptr();
			CHECK_EQUAL(sSmallEqual,SS);
			pf=&af;
			sptru=(pf);
			sptru2=(sptru);
			bool sSmallEqual2=sptru<=sptru2,SS2=sptru.ptr()<=sptru2.ptr();
			CHECK_EQUAL(sSmallEqual2,SS2);
			pc=&ac;
			pc2=&ac2;
			sptru=(pc);
			sptru2=(pc2);
			bool sSmallEqual3=sptru<=sptru2,SS3=sptru.ptr()<=sptru2.ptr();
			CHECK_EQUAL(sSmallEqual3,SS3);
		}
		UNITTEST_TEST(hashed_void_ptr_big_equal)
		{
			hashed_void_ptr sptru,sptru2;
			sptru.hash(46131);
			s32 *ps,as=351;
			f32 *pf,af=10.0f;
			char *pc,ac='z',*pc2,ac2='a';
			sptru.hash(327558);
			ps=&as;
			sptru=(ps);
			sptru2=(sptru);
			bool sSmallEqual=sptru>=sptru2,SS=sptru.ptr()>=sptru2.ptr();
			CHECK_EQUAL(sSmallEqual,SS);
			pf=&af;
			sptru=(pf);
			sptru2=(sptru);
			bool sSmallEqual2=sptru>=sptru2,SS2=sptru.ptr()>=sptru2.ptr(),sss=sptru.ptr()<sptru2.ptr();
			CHECK_EQUAL(sSmallEqual2,SS2);
			CHECK_NOT_EQUAL(sSmallEqual2,sss);
			pc=&ac;
			pc2=&ac2;
			sptru=(pc);
			sptru2=(pc2);
			bool sSmallEqual3=sptru>=sptru2,SS3=sptru.ptr()>=sptru2.ptr();
			CHECK_EQUAL(sSmallEqual3,SS3);
		}
		UNITTEST_TEST(hashed_void_ptr_get_mHash)
		{
			hashed_void_ptr sptru;
			sptru.hash(46131);
			s32  *ps = (s32  *)0x351;
			f32  *pf = (f32  *)0x586;
			char *pc = (char *)0xa13;

			sptru = ps;
			u32 uhash=sptru.hash();

			CHECK_EQUAL(0x0ace4dc6,uhash);

			sptru = pf;
			u32 uhash2=sptru.hash();

			CHECK_EQUAL(0xd2335159,uhash2);

			sptru = pc;
			u32 uhash3=sptru.hash();

			CHECK_EQUAL(0x19f02a74,uhash3);
		}
		UNITTEST_TEST(hashed_void_ptr_vector)
		{
			hashed_void_ptr sptrVoid;
			u32 a=46131;
			u32 *p;
			p=&a;
			sptrVoid=(p);
			void *q=(& sptrVoid)->ptr();
			CHECK_EQUAL(q,sptrVoid.ptr());
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrVoid=(p2);
			void *q2=(& sptrVoid)->ptr();
			CHECK_EQUAL(q2,sptrVoid.ptr());
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrVoid=(p3);
			void *q3=(& sptrVoid)->ptr();
			CHECK_EQUAL(q3,sptrVoid.ptr());
		}
		UNITTEST_TEST(hashed_void_ptr_const_vector)
		{
			hashed_void_ptr sptrVoid;
			u32 a=46131;
			u32 *p;
			p=&a;
			sptrVoid=(p);
			void const *q=(& sptrVoid)->ptr();
			CHECK_EQUAL(q,sptrVoid.ptr());
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrVoid=(p2);
			void const *q2=(& sptrVoid)->ptr();
			CHECK_EQUAL(q2,sptrVoid.ptr());
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrVoid=(p3);
			void const *q3=(& sptrVoid)->ptr();
			CHECK_EQUAL(q3,sptrVoid.ptr());
		}
		UNITTEST_TEST(hashed_void_ptr_void_point)
		{
			hashed_void_ptr sptrVoid;
			u32 a=46131;
			u32 *p;
			p=&a;			
			sptrVoid=(p);
			void *q= (void*)sptrVoid;
			CHECK_EQUAL(q,sptrVoid.ptr());
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrVoid=(p2);
			void *q2=(void *)sptrVoid;
			CHECK_EQUAL(q2,sptrVoid.ptr());
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrVoid=(p3);
			void *q3=(void *)sptrVoid;
			CHECK_EQUAL(q3,sptrVoid.ptr());
		}
		UNITTEST_TEST(hashed_void_ptr_const_void_point)
		{
			hashed_void_ptr sptrVoid;
			u32 a=46131;
			u32 *p;
			p=&a;			
			sptrVoid=(p);
			const void *q= (const void*)sptrVoid;
			CHECK_EQUAL(q,sptrVoid.ptr());
			hashed_ptr<f32> sptrf;
			f32 a2=46131.654f;
			f32 *p2;
			p2=&a2;
			sptrVoid=(p2);
			const void *q2=(const void *)sptrVoid;
			CHECK_EQUAL(q2,sptrVoid.ptr());
			hashed_ptr<char> sptrc;
			char a3='s',*p3;
			p3=&a3;
			sptrVoid=(p3);
			const void *q3=(const void *)sptrVoid;
			CHECK_EQUAL(q3,sptrVoid.ptr());
		}
	}
}
UNITTEST_SUITE_END