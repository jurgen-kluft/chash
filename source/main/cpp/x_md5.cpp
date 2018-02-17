/**
 * @file x_md5.cpp
 *
 * Core MD5 hash value 
 */

// x_md5.cpp - Core MD5 hash value 
#include "xbase/x_target.h"
#include "xbase/x_va_list.h"
#include "xbase/x_memory_std.h"
#include "xbase/x_endian.h"
#include "xbase/x_buffer.h"

#include "xhash/x_md5.h"

namespace xcore
{

	//---------------------------------------------------------------------------------------------------------------------
	//	MD5HashValue
	//---------------------------------------------------------------------------------------------------------------------



	//---------------------------------------------------------------------------------------------------------------------
	//	MD5Hash
	//---------------------------------------------------------------------------------------------------------------------


	/**
	 * @group		xhash
	 * @page		MD5 hash algorithm

	 * 			The MD5Hash class implements the MD5 message-digest algorithm. The implementation
	 * 			in Core is based on code written by Colin Plumb in 1993. Copyright on the code
	 * 			was not claimed and the code was placed in the public domain. This reference to
	 * 			the original source is provided out of courtesy reasons. The original source code
	 * 			was part of Nemesis "An operating system with principles".

	 * 			The Nemesis homepage can be found here:

	 * 			http: * nemesis.sourceforge.net/

	 * 			And the original MD5 source code here:

	 * 			http: * nemesis.sourceforge.net/browse/mod/hash/md5/md5.c.html

	 * 			The MD5 algorithm (Message Digest 5) is a cryptographic message digest algorithm.

	 * 			MD5 was designed by Ron Rivest, who is also the `R' in `RSA' in 1991. MD5 is
	 * 			described in RFC 1321. C source code is included with the RFC. It is basically
	 * 			MD4 with "safety-belts" and while it is slightly slower than MD4, it is more
	 * 			secure. The algorithm consists of four distinct rounds, which have a slightly
	 * 			different design from that of MD4. Message-digest size, as well as padding
	 * 			requirements, remains the same. Den Boer and Bosselaers [B. den Boer and A.
	 * 			Bosselaers. Collisions for the compression function of MD5. In Advances
	 * 			in Cryptology - Eurocrypt '93, pages 293-304, Springer-Verlag, 1994.]
	 * 			have found pseudo-collisions for MD5 (see RSA FAQ Question 98), but there
	 * 			are no other known cryptanalytic results. 

	 * 			The MD5 algorithm takes as input a message of arbitrary length and produces
	 * 			as output a 128-bit "fingerprint" or "message digest" of the input. It is
	 * 			conjectured that it is computationally infeasible to produce two messages
	 * 			having the same message digest, or to produce any message having a given
	 * 			prespecified target message digest. The MD5 algorithm is intended for
	 * 			digital signature applications, where a large file must be "compressed"
	 * 			in a secure manner before being encrypted with a private (secret) key
	 * 			under a public-key cryptosystem such as RSA or PGP. 
	 */



	/**
	 * @brief		Byteswap an area of memory

	 * 			This area converts a number of u32's to little endian format to be compatible
	 * 			with the MD5 hash algorithm.

	 * @param		ioBuffer	Pointer to data to swap
	 * @param		inLength	Number of u32's to swap
	 */
	static void sByteSwap(u32* ioBuffer, s32 inLength)
	{
#if !defined(X_LITTLE_ENDIAN)
		for (s32 i=0; i<inLength; i++)
			ioBuffer[i] = x_endian_swap::swap(ioBuffer[i]);
#endif
	}



	/**
	 * @brief		Construct MD5 hash generator

	 * Construct an empty MD5 hash generator. The buffer is initialized and the hash
	 * value is marked as "open" (i.e. the final hash value has not been retrieved
	 * yet).

	 * @see		Update GetHash
	 */
	xdigest_engine_md5::xdigest_engine_md5()
		: mState(xdigest_engine_md5::CLOSED)
		, mLength(0)
	{
	}



	//---------------------------------------------------------------------------------------------------------------------
	//	Algorithm
	//---------------------------------------------------------------------------------------------------------------------


	/**
	 * @brief Update MD5 hash value with new data
     * 
	 * @param inData	Buffer to update hash with
	 * @param inLength	Length of buffer in bytes
	 */
	void					xdigest_engine_md5::update(const xcbuffer& buffer)
	{
		ASSERTS(mState==OPEN, "Can't compute hash value before Open() has been called!");

		// Calculate current offset in buffer and bytes left
		u32 buffer_offset = (mLength & 63);											// Current offset in buffer
		u32 space_left	  = 64 - buffer_offset;										// Space available in mBuffer.mInput (at least 1)

		// Update length
		u32 const len = buffer.size();
		mLength += len;

		// If there's enough space in the buffer, just copy and exit
		if (len  < space_left)
		{
			x_memcopy((u8*)mBuffer.mInput + buffer_offset, buffer.m_data, len);
			return;
		}

		// Fill up current buffer until it's full
		u8 const* data   = (u8 const*)buffer.m_data;
		u32     length = len;
		x_memcopy((u8*)mBuffer.mInput + buffer_offset, data, space_left);
		sByteSwap(mBuffer.mInput, 16);
		transform();
		data   += space_left;
		length -= space_left;

		// Process data in 64-byte chunks
		while (length >= 64)
		{
			x_memcopy((u64*)mBuffer.mInput, (u64*)data, 64);
			sByteSwap(mBuffer.mInput, 16);
			transform();
			data   += 64;
			length -= 64;
		}

		// Handle any remaining bytes of data
		x_memcopy(mBuffer.mInput, data, length);
	}



	/**
	 * @brief Get final hash value
	 */
	void				xdigest_engine_md5::digest(xbuffer& digest)
	{
		// If this is the first time we call GetHash(), finish the last transform
		if (mState == OPEN)
		{
			u32	count	= mLength & 63;											// Number of bytes in mBuffer.mInput
			u8* p		= (u8*)mBuffer.mInput + count;

			// Set the first char of padding to 0x80.  There is always room.
			*p++ = 0x80;

			// Bytes of padding needed to make 56 bytes (-8..55)
			count = 56 - 1 - count;

			// Check if padding causes an extra block
			if (count < 0)
			{
				x_memzero(p, count + 8);
				sByteSwap(mBuffer.mInput, 16);
				transform();
				p = (u8*)mBuffer.mInput;
				count = 56;
			}
			x_memzero(p, count);
			sByteSwap(mBuffer.mInput, 14);

			// Append length of message in bits
			mBuffer.mInput[14] = (u32)(mLength << 3);
			mBuffer.mInput[15] = (u32)(mLength >> 29);

			// Final transform
			transform();

			mState = CLOSED;
		}

		// export digest
		xbyte const* src = (xbyte const*)&mMD5[0];
		for (s32 i=0; i<16; ++i)
			digest[i] = *src++;
	}

	void					xdigest_engine_md5::digest(xmd5& md5)
	{
		xbytes16 e;
		xbuffer b = e.buffer();
		digest(b);
		sByteSwap((u32*)e.m_data, 4);
		for (s32 i=0; i<e.size(); ++i)
			md5.set8(i, e[i]);
	}


	void					xdigest_engine_md5::reset()
	{
		mState  = OPEN;
		mLength = 0;
		mMD5[0] = 0x67452301;
		mMD5[1] = 0xefcdab89;
		mMD5[2] = 0x98badcfe;
		mMD5[3] = 0x10325476;
	}

	// The four core functions - F1 is optimized somewhat
	#define MD5F1(x, y, z) (z ^ (x & (y ^ z)))
	#define MD5F2(x, y, z) MD5F1(z, x, y)
	#define MD5F3(x, y, z) (x ^ y ^ z)
	#define MD5F4(x, y, z) (y ^ (x | ~z))

		// This is the central step in the MD5 algorithm
	#define MD5STEP(f,w,x,y,z,in,s)		(w += f(x,y,z) + in, w = (w<<s | w>>(32-s)) + x)


	/**
	 * @brief MD5 hash transform
	 *
	 * The core of the MD5 algorithm, this alters an existing MD5 hash to
	 * reflect the addition of 16 longwords of new data.  MD5Update blocks
	 * the data and converts bytes into longwords for this routine.
	 */
	void					xdigest_engine_md5::transform()
	{
		u32 a = mMD5[0];
		u32 b = mMD5[1];
		u32 c = mMD5[2];
		u32 d = mMD5[3];

		MD5STEP(MD5F1, a, b, c, d, mBuffer.mInput[0]  + 0xd76aa478,  7);
		MD5STEP(MD5F1, d, a, b, c, mBuffer.mInput[1]  + 0xe8c7b756, 12);
		MD5STEP(MD5F1, c, d, a, b, mBuffer.mInput[2]  + 0x242070db, 17);
		MD5STEP(MD5F1, b, c, d, a, mBuffer.mInput[3]  + 0xc1bdceee, 22);
		MD5STEP(MD5F1, a, b, c, d, mBuffer.mInput[4]  + 0xf57c0faf,  7);
		MD5STEP(MD5F1, d, a, b, c, mBuffer.mInput[5]  + 0x4787c62a, 12);
		MD5STEP(MD5F1, c, d, a, b, mBuffer.mInput[6]  + 0xa8304613, 17);
		MD5STEP(MD5F1, b, c, d, a, mBuffer.mInput[7]  + 0xfd469501, 22);
		MD5STEP(MD5F1, a, b, c, d, mBuffer.mInput[8]  + 0x698098d8,  7);
		MD5STEP(MD5F1, d, a, b, c, mBuffer.mInput[9]  + 0x8b44f7af, 12);
		MD5STEP(MD5F1, c, d, a, b, mBuffer.mInput[10] + 0xffff5bb1, 17);
		MD5STEP(MD5F1, b, c, d, a, mBuffer.mInput[11] + 0x895cd7be, 22);
		MD5STEP(MD5F1, a, b, c, d, mBuffer.mInput[12] + 0x6b901122,  7);
		MD5STEP(MD5F1, d, a, b, c, mBuffer.mInput[13] + 0xfd987193, 12);
		MD5STEP(MD5F1, c, d, a, b, mBuffer.mInput[14] + 0xa679438e, 17);
		MD5STEP(MD5F1, b, c, d, a, mBuffer.mInput[15] + 0x49b40821, 22);

		MD5STEP(MD5F2, a, b, c, d, mBuffer.mInput[1]  + 0xf61e2562,  5);
		MD5STEP(MD5F2, d, a, b, c, mBuffer.mInput[6]  + 0xc040b340,  9);
		MD5STEP(MD5F2, c, d, a, b, mBuffer.mInput[11] + 0x265e5a51, 14);
		MD5STEP(MD5F2, b, c, d, a, mBuffer.mInput[0]  + 0xe9b6c7aa, 20);
		MD5STEP(MD5F2, a, b, c, d, mBuffer.mInput[5]  + 0xd62f105d,  5);
		MD5STEP(MD5F2, d, a, b, c, mBuffer.mInput[10] + 0x02441453,  9);
		MD5STEP(MD5F2, c, d, a, b, mBuffer.mInput[15] + 0xd8a1e681, 14);
		MD5STEP(MD5F2, b, c, d, a, mBuffer.mInput[4]  + 0xe7d3fbc8, 20);
		MD5STEP(MD5F2, a, b, c, d, mBuffer.mInput[9]  + 0x21e1cde6,  5);
		MD5STEP(MD5F2, d, a, b, c, mBuffer.mInput[14] + 0xc33707d6,  9);
		MD5STEP(MD5F2, c, d, a, b, mBuffer.mInput[3]  + 0xf4d50d87, 14);
		MD5STEP(MD5F2, b, c, d, a, mBuffer.mInput[8]  + 0x455a14ed, 20);
		MD5STEP(MD5F2, a, b, c, d, mBuffer.mInput[13] + 0xa9e3e905,  5);
		MD5STEP(MD5F2, d, a, b, c, mBuffer.mInput[2]  + 0xfcefa3f8,  9);
		MD5STEP(MD5F2, c, d, a, b, mBuffer.mInput[7]  + 0x676f02d9, 14);
		MD5STEP(MD5F2, b, c, d, a, mBuffer.mInput[12] + 0x8d2a4c8a, 20);

		MD5STEP(MD5F3, a, b, c, d, mBuffer.mInput[5]  + 0xfffa3942,  4);
		MD5STEP(MD5F3, d, a, b, c, mBuffer.mInput[8]  + 0x8771f681, 11);
		MD5STEP(MD5F3, c, d, a, b, mBuffer.mInput[11] + 0x6d9d6122, 16);
		MD5STEP(MD5F3, b, c, d, a, mBuffer.mInput[14] + 0xfde5380c, 23);
		MD5STEP(MD5F3, a, b, c, d, mBuffer.mInput[1]  + 0xa4beea44,  4);
		MD5STEP(MD5F3, d, a, b, c, mBuffer.mInput[4]  + 0x4bdecfa9, 11);
		MD5STEP(MD5F3, c, d, a, b, mBuffer.mInput[7]  + 0xf6bb4b60, 16);
		MD5STEP(MD5F3, b, c, d, a, mBuffer.mInput[10] + 0xbebfbc70, 23);
		MD5STEP(MD5F3, a, b, c, d, mBuffer.mInput[13] + 0x289b7ec6,  4);
		MD5STEP(MD5F3, d, a, b, c, mBuffer.mInput[0]  + 0xeaa127fa, 11);
		MD5STEP(MD5F3, c, d, a, b, mBuffer.mInput[3]  + 0xd4ef3085, 16);
		MD5STEP(MD5F3, b, c, d, a, mBuffer.mInput[6]  + 0x04881d05, 23);
		MD5STEP(MD5F3, a, b, c, d, mBuffer.mInput[9]  + 0xd9d4d039,  4);
		MD5STEP(MD5F3, d, a, b, c, mBuffer.mInput[12] + 0xe6db99e5, 11);
		MD5STEP(MD5F3, c, d, a, b, mBuffer.mInput[15] + 0x1fa27cf8, 16);
		MD5STEP(MD5F3, b, c, d, a, mBuffer.mInput[2]  + 0xc4ac5665, 23);

		MD5STEP(MD5F4, a, b, c, d, mBuffer.mInput[0]  + 0xf4292244,  6);
		MD5STEP(MD5F4, d, a, b, c, mBuffer.mInput[7]  + 0x432aff97, 10);
		MD5STEP(MD5F4, c, d, a, b, mBuffer.mInput[14] + 0xab9423a7, 15);
		MD5STEP(MD5F4, b, c, d, a, mBuffer.mInput[5]  + 0xfc93a039, 21);
		MD5STEP(MD5F4, a, b, c, d, mBuffer.mInput[12] + 0x655b59c3,  6);
		MD5STEP(MD5F4, d, a, b, c, mBuffer.mInput[3]  + 0x8f0ccc92, 10);
		MD5STEP(MD5F4, c, d, a, b, mBuffer.mInput[10] + 0xffeff47d, 15);
		MD5STEP(MD5F4, b, c, d, a, mBuffer.mInput[1]  + 0x85845dd1, 21);
		MD5STEP(MD5F4, a, b, c, d, mBuffer.mInput[8]  + 0x6fa87e4f,  6);
		MD5STEP(MD5F4, d, a, b, c, mBuffer.mInput[15] + 0xfe2ce6e0, 10);
		MD5STEP(MD5F4, c, d, a, b, mBuffer.mInput[6]  + 0xa3014314, 15);
		MD5STEP(MD5F4, b, c, d, a, mBuffer.mInput[13] + 0x4e0811a1, 21);
		MD5STEP(MD5F4, a, b, c, d, mBuffer.mInput[4]  + 0xf7537e82,  6);
		MD5STEP(MD5F4, d, a, b, c, mBuffer.mInput[11] + 0xbd3af235, 10);
		MD5STEP(MD5F4, c, d, a, b, mBuffer.mInput[2]  + 0x2ad7d2bb, 15);
		MD5STEP(MD5F4, b, c, d, a, mBuffer.mInput[9]  + 0xeb86d391, 21);

		mMD5[0] += a;
		mMD5[1] += b;
		mMD5[2] += c;
		mMD5[3] += d;
	}



	/**
	 * @ingroup xhash
	 * @brief	Get MD5 digest of a block of data

	 * 		This function is a single-shot MD5 digest generator. It takes a block of
	 * 		data bytes and returns its MD5 digest. It uses the MD5 digest engine 
	 *      class internally to produce its result.

	 * @see	xmd5 xdigest_engine_md5
	 */
	xmd5		x_MD5Hash(xcbuffer const& buffer)
	{
		xdigest_engine_md5 md5;
		md5.reset();
		md5.update(buffer);

		xmd5 digest;
		md5.digest(digest);
		return digest;
	}


}
