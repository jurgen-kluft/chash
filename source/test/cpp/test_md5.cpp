#include "ccore/c_target.h"
#include "ccore/c_memory.h"
#include "chash/c_hash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

namespace MD5TestVectors
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
		{"", "D41D8CD98F00B204E9800998ECF8427E"},
		{"616263", "900150983CD24FB0D6963F7D28E17F72"},
		{"6D65737361676520646967657374", "F96B697D7CB7938D525A2F31AAF161D0"},
		{"6162636465666768696A6B6C6D6E6F707172737475767778797A", "C3FCD3D76192E4007DFB496CCA67E13B"},
		{"4142434445464748494A4B4C4D4E4F505152535455565758595A6162636465666768696A6B6C6D6E6F707172737475767778797A30313233343536373839", "D174AB98D277D9F5A5611C2C9F419D9F"},
	};
}; // namespace MD5TestVectors

UNITTEST_SUITE_BEGIN(md5_t)
{
	UNITTEST_FIXTURE(generator)
	{
		UNITTEST_ALLOCATOR;

		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(known_answer_vectors)
		{
			u8 message[128];
			u8 expected[16];
			u8 digest[16];
			CHECK_EQUAL(16, nhash::digest_size(nhash::MD5));

			for (u32 index = 0; index < sizeof(MD5TestVectors::Tests) / sizeof(MD5TestVectors::Tests[0]); ++index)
			{
				MD5TestVectors::Vector const& test = MD5TestVectors::Tests[index];
				s32 const message_size = MD5TestVectors::TextToBytes(test.Msg, message);
				s32 const expected_size = MD5TestVectors::TextToBytes(test.Digest, expected);
				void* const context_memory = Allocator->allocate(nhash::context_size(nhash::MD5));

				CHECK_EQUAL(16, expected_size);

				hash_instance_t hash = create_hash(context_memory, nhash::MD5);
				CHECK_EQUAL(16, hash_size(hash));

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