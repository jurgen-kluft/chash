#include "ccore/c_target.h"
#include "ccore/c_buffer.h"
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

		static u32 murmur32_hash(const u8* begin, const u8* end, u64 seed = 0, s32 split = -1)
		{
			nhash::murmur32_t murmur32;
			murmur32.reset(seed);
			nhash::murmur32 h1;
			if (split >= 0)
			{
				murmur32.hash(begin, begin + split);
				murmur32.hash(begin + split, end);
			}
			else
				murmur32.hash(begin, end);
			murmur32.end(h1.m_data);
			binary_reader_t reader(h1.m_data, h1.m_data + h1.SIZE);
			u32 h2;
			reader.read(h2);
			return h2;
		}

		UNITTEST_TEST(known_vectors)
		{
			static const u8 empty[] = {0};
			static const u8 a[] = {'a'};
			static const u8 abc[] = {'a', 'b', 'c'};
			static const u8 hello[] = {'h', 'e', 'l', 'l', 'o'};
			static const u8 foo[] = {'f', 'o', 'o'};
			static const u8 multi[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q'};

			CHECK_EQUAL(murmur32_hash(empty, empty), 0x00000000U);
			CHECK_EQUAL(murmur32_hash(a, a + sizeof(a)), 0x3c2569b2U);
			CHECK_EQUAL(murmur32_hash(abc, abc + sizeof(abc)), 0xb3dd93faU);
			CHECK_EQUAL(murmur32_hash(hello, hello + sizeof(hello)), 0x248bfa47U);
			CHECK_EQUAL(murmur32_hash(foo, foo + sizeof(foo), 42), 0xb12f489eU);
			CHECK_EQUAL(murmur32_hash(multi, multi + sizeof(multi)), 0xb6655e4aU);
		}

		UNITTEST_TEST(streaming_matches_one_shot)
		{
			static const u8 multi[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q'};
			u32 const expected = 0xb6655e4aU;
			CHECK_EQUAL(murmur32_hash(multi, multi + sizeof(multi), 0, 1), expected);
			CHECK_EQUAL(murmur32_hash(multi, multi + sizeof(multi), 0, 4), expected);
			CHECK_EQUAL(murmur32_hash(multi, multi + sizeof(multi), 0, 15), expected);
		}
	}

}
UNITTEST_SUITE_END
