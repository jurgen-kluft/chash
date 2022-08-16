#include "xbase/x_target.h"
#include "xhash/x_crc.h"

#include "xunittest/xunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(xcrc)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(CRC_CRC32)
		{
			u8 buffer[100]={1,2,3,4,5,6,7,8,9};
			u8 buff2[100]={1,1,2,3,5,8,13,21};
			s32 len=100;
			CHECK_NOT_EQUAL(xcrc::crc32(cbuffer_t(len, buffer),1321),xcrc::crc32(cbuffer_t(len, buffer),654321));
			CHECK_NOT_EQUAL(xcrc::crc32(cbuffer_t(254, buffer),110),xcrc::crc32(cbuffer_t(25,buffer),110));
			CHECK_EQUAL(xcrc::crc32(cbuffer_t(len, buffer),0),xcrc::crc32(cbuffer_t(len, buffer)));
			CHECK_NOT_EQUAL(xcrc::crc32(cbuffer_t(len, buffer),1321),xcrc::crc32(cbuffer_t(len,buff2),654321));
		}
		UNITTEST_TEST(CRC_Adler16)
		{
			u8 buffer[100]={1,2,3,4,5,6,7,8,9};
			u8 buff2[100]={1,1,2,3,5,8,13,21};
			s32 len=100;
			CHECK_EQUAL(xcrc::adler16(cbuffer_t(len, buffer),1),xcrc::adler16(cbuffer_t(len, buffer)));
			CHECK_NOT_EQUAL(xcrc::adler16(cbuffer_t(len, buffer),110),xcrc::adler16(cbuffer_t(len, buffer),5));
			CHECK_NOT_EQUAL(xcrc::adler16(cbuffer_t(len, buffer),110),xcrc::adler16(cbuffer_t(len,buff2),110));
			CHECK_NOT_EQUAL(xcrc::adler16(cbuffer_t(len, buffer),110),xcrc::adler16(cbuffer_t(50,buffer),110));
			CHECK_EQUAL(xcrc::adler16(cbuffer_t(0, buffer),358),((358& 0xFF)|((358>> 8)<<8)));
		}
		UNITTEST_TEST(CRC_Adler32)
		{
			u8 buffer[100]={1,2,3,51,54,65,98};
			u8 buff2[100]={1,1,2,3,5,8,13,21};
			s32 len=100;
			CHECK_EQUAL(xcrc::adler32(cbuffer_t(len, buffer),1),xcrc::adler32(cbuffer_t(len, buffer)));
			CHECK_NOT_EQUAL(xcrc::adler32(cbuffer_t(len, buffer),110),xcrc::adler32(cbuffer_t(len, buffer),5));
			CHECK_NOT_EQUAL(xcrc::adler32(cbuffer_t(len, buffer),123456),xcrc::adler32(cbuffer_t(len, buff2),123456));
			CHECK_NOT_EQUAL(xcrc::adler32(cbuffer_t(len, buffer),110),xcrc::adler32(cbuffer_t(50, buffer),110));
			CHECK_EQUAL(xcrc::adler32(cbuffer_t(0, buffer),112358),((112358& 0xFFFF)|((112358>> 16)<<16)));
		}
	}
}
UNITTEST_SUITE_END