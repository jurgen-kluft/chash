#include "ccore/c_target.h"
#include "cbase/c_buffer.h"
#include "chash/c_hash.h"
#include "chash/private/c_internal_hash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(murmur64_t)
{
	UNITTEST_FIXTURE(xdigest_murmur64)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static u64 murmur64_hash(cbuffer_t const& b)
		{
			nhash_private::murmur64_t murmur64;

			nhash::murmur64 h1;
			murmur64.reset();
			murmur64.hash(b.m_begin, b.m_end);
			murmur64.end(h1.m_data);
			binary_reader_t reader(h1.m_data, h1.m_data + h1.SIZE);
			u64 h2;
			reader.read(h2);
			return h2;
		}

		UNITTEST_TEST(test)
		{
			u8 indata[]={1,2,3,4,5,6,7,8,9,13};
            u8 indata2[]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10;
			u64 ruhash  = murmur64_hash(cbuffer_t(indata, indata + len));
			u64 ruhash2 = murmur64_hash(cbuffer_t(indata2, indata2 + len));
			CHECK_EQUAL(ruhash>0,true);
			CHECK_NOT_EQUAL(ruhash, ruhash2);
		}

	}
}
UNITTEST_SUITE_END
