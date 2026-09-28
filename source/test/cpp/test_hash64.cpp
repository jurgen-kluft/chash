#include "ccore/c_target.h"
#include "ccore/c_buffer.h"
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

		static u64 murmur64_hash(const u8* begin, const u8* end, u64 seed = 0, s32 split = -1)
		{
			nhash::murmur64_t murmur64;

			nhash::murmur64 h1;
			murmur64.reset(seed);
			if (split >= 0)
			{
				murmur64.hash(begin, begin + split);
				murmur64.hash(begin + split, end);
			}
			else
				murmur64.hash(begin, end);
			murmur64.end(h1.m_data);
			binary_reader_t reader(h1.m_data, h1.m_data + h1.SIZE);
			u64 h2;
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

			CHECK_EQUAL(murmur64_hash(empty, empty), 0x0000000000000000ULL);
			CHECK_EQUAL(murmur64_hash(a, a + sizeof(a)), 0x85555565f6597889ULL);
			CHECK_EQUAL(murmur64_hash(abc, abc + sizeof(abc)), 0xb4963f3f3fad7867ULL);
			CHECK_EQUAL(murmur64_hash(hello, hello + sizeof(hello)), 0xcbd8a7b341bd9b02ULL);
			CHECK_EQUAL(murmur64_hash(foo, foo + sizeof(foo), 42), 0xf4569d51637053f2ULL);
			CHECK_EQUAL(murmur64_hash(multi, multi + sizeof(multi)), 0x7564747f88bda657ULL);
		}

		UNITTEST_TEST(streaming_and_seed_folding)
		{
			static const u8 multi[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q'};
			u64 const expected = 0x7564747f88bda657ULL;
			CHECK_EQUAL(murmur64_hash(multi, multi + sizeof(multi), 0, 1), expected);
			CHECK_EQUAL(murmur64_hash(multi, multi + sizeof(multi), 0, 8), expected);
			CHECK_EQUAL(murmur64_hash(multi, multi + sizeof(multi), 0, 16), expected);
			CHECK_EQUAL(murmur64_hash(multi, multi + sizeof(multi), 0x0102030405060708ULL),
			            murmur64_hash(multi, multi + sizeof(multi), 0x000000000404040cULL));
		}

	}
}
UNITTEST_SUITE_END
