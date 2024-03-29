#include "ccore/c_target.h"
#include "cbase/c_buffer.h"
#include "chash/c_hash.h"
#include "chash/private/c_internal_hash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(murmur32_t)
{
	UNITTEST_FIXTURE(xdigest_murmur32)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static u32 murmur32_hash(cbuffer_t const& b)
		{
			murmur32_t murmur32;
			murmur32.reset();
			nhash::murmur32 h1;
			murmur32.hash(b.m_begin, b.m_end);
			murmur32.end(h1.m_data);
			binary_reader_t reader(h1.m_data, h1.m_data + h1.SIZE);
			u32 h2;
			reader.read(h2);
			return h2;
		}

		UNITTEST_TEST(test)
		{
			u8 indata[]={1,2,3,4,5,6,7,8,9,13},indata2[]={0xff,0xfe,0xfd,0xfc,0xfb,0,1,2,3,4};
			u32 len=10;
			u32 ruhash = murmur32_hash(cbuffer_t(indata, indata + len));
			u32 ruhash2 = murmur32_hash(cbuffer_t(indata, indata + len));
			CHECK_EQUAL(ruhash>0,true);
			CHECK_EQUAL(ruhash,ruhash2);
		}
	}
	
}
UNITTEST_SUITE_END