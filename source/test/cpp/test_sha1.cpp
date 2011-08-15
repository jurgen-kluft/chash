#include "xbase\x_target.h"
#include "xbase\x_string_std.h"
#include "xhash\x_sha1.h"

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(xsha1)
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
			xsha1_generator g;
			g.open();
			xsha1 h;
			CHECK_TRUE(g.close(h));

			char sha1Str[128];
			u32 sha1StrLen = sizeof(sha1Str) - 1;
			CHECK_TRUE(h.toString(sha1Str, sha1StrLen));
			CHECK_EQUAL(40, sha1StrLen);

			CHECK_EQUAL(0, x_strCompareNoCase("da39a3ee5e6b4b0d3255bfef95601890afd80709", sha1StrLen, sha1Str));
		}

		UNITTEST_TEST(Fox1)
		{
			xsha1_generator g;
			g.open();

			char const* data = "The quick brown fox jumps over the lazy dog";
			u32 data_len = x_strlen(data);
			g.compute(data, data_len);

			xsha1 h;
			CHECK_TRUE(g.close(h));

			char sha1Str[128];
			u32 sha1StrLen = sizeof(sha1Str) - 1;
			CHECK_TRUE(h.toString(sha1Str, sha1StrLen));
			CHECK_EQUAL(40, sha1StrLen);

			CHECK_EQUAL(0, x_strCompareNoCase("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12", sha1StrLen, sha1Str));
		}


		UNITTEST_TEST(Fox2)
		{
			xsha1_generator g;
			g.open();

			char const* data = "The quick brown fox jumps over the lazy cog";
			u32 data_len = x_strlen(data);
			g.compute(data, data_len);

			xsha1 h;
			CHECK_TRUE(g.close(h));

			char sha1Str[128];
			u32 sha1StrLen = sizeof(sha1Str) - 1;
			CHECK_TRUE(h.toString(sha1Str, sha1StrLen));
			CHECK_EQUAL(40, sha1StrLen);

			CHECK_EQUAL(0, x_strCompareNoCase("de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3", sha1StrLen, sha1Str));
		}

	}
}
UNITTEST_SUITE_END