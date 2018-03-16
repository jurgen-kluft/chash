#include "xbase/x_target.h"
#include "xbase/x_string_ascii.h"
#include "xhash/x_sha1.h"

#include "xunittest/xunittest.h"

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

			xuchars128 sha1Str;
			u32 len = h.toString(sha1Str.chars());
			CHECK_TRUE(len!=0);
			CHECK_EQUAL(40, len);

			CHECK_EQUAL(0, ascii::compare(ascii::crunes("da39a3ee5e6b4b0d3255bfef95601890afd80709"), sha1Str.str()));
		}

		UNITTEST_TEST(Fox1)
		{
			xdigest_engine_sha1 g;
			g.reset();

			xcuchars data("The quick brown fox jumps over the lazy dog");
			g.update(data.buffer());

			xsha1 h;
			CHECK_TRUE(g.digest(h));

			xuchars128 sha1Str;
			s32 len = h.toString(sha1Str.chars());
			CHECK_TRUE(len!=0);
			CHECK_EQUAL(40, len);

			CHECK_EQUAL(0, ascii::compare(ascii::crunes("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"), sha1Str.str()));
		}


		UNITTEST_TEST(Fox2)
		{
			xdigest_engine_sha1 g;
			g.reset();

			xcuchars data("The quick brown fox jumps over the lazy cog");
			g.update(data.buffer());

			xsha1 h;
			CHECK_TRUE(g.digest(h));

			xuchars128 sha1Str;
			s32 len = h.toString(sha1Str.chars());
			CHECK_TRUE(len!=0);
			CHECK_EQUAL(40, len);

			CHECK_EQUAL(0, ascii::compare(ascii::crunes("de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3"), sha1Str.cchars()));
		}

	}
}
UNITTEST_SUITE_END
