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
			xdigest_engine_sha1 g;
			g.reset();
			xsha1 h;
			CHECK_TRUE(g.digest(h));

			char sha1Str[128];
			u32 sha1StrLen = sizeof(sha1Str) - 1;
			u32 len = h.toString(sha1Str, sha1StrLen);
			CHECK_TRUE(len!=0);
			CHECK_EQUAL(40, len);

			CHECK_EQUAL(0, x_strCompareNoCase("da39a3ee5e6b4b0d3255bfef95601890afd80709", len, sha1Str));
		}

		UNITTEST_TEST(Fox1)
		{
			xdigest_engine_sha1 g;
			g.reset();

			char const* data = "The quick brown fox jumps over the lazy dog";
			u32 data_len = x_strlen(data);
			g.update(data, data_len);

			xsha1 h;
			CHECK_TRUE(g.digest(h));

			char sha1Str[128];
			u32 sha1StrLen = sizeof(sha1Str) - 1;
			s32 len = h.toString(sha1Str, sha1StrLen);
			CHECK_TRUE(len!=0);
			CHECK_EQUAL(40, len);

			CHECK_EQUAL(0, x_strCompareNoCase("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12", len, sha1Str));
		}


		UNITTEST_TEST(Fox2)
		{
			xdigest_engine_sha1 g;
			g.reset();

			char const* data = "The quick brown fox jumps over the lazy cog";
			u32 data_len = x_strlen(data);
			g.update(data, data_len);

			xsha1 h;
			CHECK_TRUE(g.digest(h));

			char sha1Str[128];
			u32 sha1StrLen = sizeof(sha1Str) - 1;
			s32 len = h.toString(sha1Str, sha1StrLen);
			CHECK_TRUE(len!=0);
			CHECK_EQUAL(40, len);

			CHECK_EQUAL(0, x_strCompareNoCase("de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3", len, sha1Str));
		}

	}
}
UNITTEST_SUITE_END
