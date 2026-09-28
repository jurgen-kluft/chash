#include "ccore/c_target.h"
#include "ccore/c_memory.h"
#include "chash/c_hash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

namespace SHA1TestVectors
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
		{"", "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709"},
		{"616263", "A9993E364706816ABA3E25717850C26C9CD0D89D"},
		{"6162636462636465636465666465666765666768666768696768696A68696A6B696A6B6C6A6B6C6D6B6C6D6E6C6D6E6F6D6E6F706E6F7071", "84983E441C3BD26EBAAE4AA1F95129E5E54670F1"},
	};
}; // namespace SHA1TestVectors

UNITTEST_SUITE_BEGIN(sha1)
{
	UNITTEST_FIXTURE(generator)
	{
		UNITTEST_ALLOCATOR;

		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(known_answer_vectors)
		{
			u8 message[128];
			u8 expected[20];
			u8 digest[20];
			CHECK_EQUAL(20, nhash::digest_size(nhash::SHA1));

			for (u32 index = 0; index < sizeof(SHA1TestVectors::Tests) / sizeof(SHA1TestVectors::Tests[0]); ++index)
			{
				SHA1TestVectors::Vector const& test = SHA1TestVectors::Tests[index];
				s32 const message_size = SHA1TestVectors::TextToBytes(test.Msg, message);
				s32 const expected_size = SHA1TestVectors::TextToBytes(test.Digest, expected);
				void* const context_memory = Allocator->allocate(nhash::context_size(nhash::SHA1));

				CHECK_EQUAL(20, expected_size);

				hash_instance_t hash = create_hash(context_memory, nhash::SHA1);
				CHECK_EQUAL(20, hash_size(hash));

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
