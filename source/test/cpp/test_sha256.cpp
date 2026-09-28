#include "ccore/c_target.h"
#include "ccore/c_memory.h"
#include "chash/c_hash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

namespace SHA256TestVectors
{
	struct Vector
	{
		char const* Msg;
		char const* Digest;
	};

	static inline u8 CharToByte(char ch)
	{
		if ((ch >= '0') && (ch <= '9'))
			return (u8)(ch - '0');
		if ((ch >= 'A') && (ch <= 'F'))
			return (u8)(ch - 'A' + 10);
		return (u8)(ch - 'a' + 10);
	}

	static s32 TextToBytes(char const* text, u8* bytes)
	{
		s32 length = 0;
		while (text[0] != '\0')
		{
			bytes[length++] = (u8)((CharToByte(text[0]) << 4) | CharToByte(text[1]));
			text += 2;
		}
		return length;
	}

	static Vector const Tests[] = {
		{"", "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855"},
		{"616263", "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD"},
		{"6162636462636465636465666465666765666768666768696768696A68696A6B696A6B6C6A6B6C6D6B6C6D6E6C6D6E6F6D6E6F706E6F7071", "248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1"},
	};
}; // namespace SHA256TestVectors

UNITTEST_SUITE_BEGIN(sha256)
{
	UNITTEST_FIXTURE(generator)
	{
		UNITTEST_ALLOCATOR;

		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(known_answer_vectors)
		{
			u8 message[128];
			u8 expected[32];
			u8 digest[32];
			CHECK_EQUAL(32, nhash::digest_size(nhash::SHA256));

			for (u32 index = 0; index < sizeof(SHA256TestVectors::Tests) / sizeof(SHA256TestVectors::Tests[0]); ++index)
			{
				SHA256TestVectors::Vector const& test = SHA256TestVectors::Tests[index];
				s32 const message_size = SHA256TestVectors::TextToBytes(test.Msg, message);
				s32 const expected_size = SHA256TestVectors::TextToBytes(test.Digest, expected);
				void* const context_memory = Allocator->allocate(nhash::context_size(nhash::SHA256));

				CHECK_EQUAL(32, expected_size);

				hash_instance_t hash = create_hash(context_memory, nhash::SHA256);
				CHECK_EQUAL(32, hash_size(hash));

				hash_begin(hash);
				if (message_size > 1)
				{
					s32 const midpoint = message_size / 2;
					hash_update(hash, message, message + midpoint);
					hash_update(hash, message + midpoint, message + message_size);
				}
				else if (message_size == 1)
				{
					hash_update(hash, message, message + message_size);
				}
				hash_end(hash, digest, sizeof(digest));

				CHECK_TRUE(nmem::memcmp(expected, digest, sizeof(digest)) == 0);
				destroy_hash(context_memory, hash);
				Allocator->deallocate(context_memory);
			}
		}
	}
}
UNITTEST_SUITE_END
