#include "xbase\x_target.h"
#include "xbase\x_string_std.h"
#include "xhash\x_skein.h"

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xskein)
{
	UNITTEST_FIXTURE(type)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}
	}

	UNITTEST_FIXTURE(generator)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Empty)
		{
			
		}

		UNITTEST_TEST(0xFF)
		{
			u8 message[] = { 0xFF };
			xskein256 hash = x_skein256Hash(message, 1);

			u8 vhash[] =
			{ 
				0xA4, 0x7B, 0xE7, 0x1A, 0x18, 0x5B, 0xA0, 0xAF, 0x82, 0x0B, 0x3C, 0xE8, 0x45, 0xA3, 0xD3, 0x5A,
				0x80, 0xEC, 0x64, 0xF9, 0x6A, 0x0D, 0x6A, 0x36, 0xE3, 0xF5, 0x36, 0x36, 0x24, 0xD8, 0xA0, 0x91
			};

			for (s32 i=0; i<hash.size(); ++i)
				CHECK_EQUAL(vhash[i], hash.data()[i]);
		}


		UNITTEST_TEST(Fox2)
		{
			
		}

	}
}
UNITTEST_SUITE_END
