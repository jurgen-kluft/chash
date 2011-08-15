// x_md5.cpp - Core MD5 hash value 
#include "xbase\x_target.h"
#include "xbase\x_va_list.h"
#include "xbase\x_integer.h"
#include "xbase\x_string_std.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_endian.h"

#include "xhash\x_sha1.h"

namespace xcore
{
	// URL:
	//     http://en.wikipedia.org/wiki/SHA-1
	//
	// Description:
	//     This class implements the Secure Hashing Standard as defined
	//     in FIPS PUB 180-1 published April 17, 1995.
	//
	//     The Secure Hashing Standard, which uses the Secure Hashing
	//     Algorithm (SHA), produces a 160-bit message digest for a
	//     given data stream.  In theory, it is highly improbable that
	//     two messages will produce the same message digest.  Therefore,
	//     this algorithm can serve as a means of providing a "fingerprint"
	//     for a message.
	//
	// Portability Issues:
	//     SHA-1 is defined in terms of 32-bit "words".  This code was
	//     written with the expectation that the processor has at least
	//     a 32-bit machine word size.  If the machine word size is larger,
	//     the code should still function properly.  One caveat to that
	//     is that the input functions taking characters and character arrays
	//     assume that only 8 bits of information are stored in each character.
	//
	// Caveats:
	//     SHA-1 is designed to work with messages less than 2^64 bits long.
	//     Although SHA-1 allows a message digest to be generated for
	//     messages of any number of bits less than 2^64, this implementation
	//     only works with messages with a length that is a multiple of 8
	//     bits.
	//
	inline const u32 sha1_rol(const u32 num, const u32 cnt)
	{
		return((num << cnt) | (num >> (32-cnt)));
	}

	static void sha1_compute(u32 *result, u32 *w)
	{
#ifdef X_LITTLE_ENDIAN
		u8* cw = (u8*)w;
		for (s32 i=0; i<64; i+=4, cw+=4)
		{
			u8 t = cw[0];
			cw[0] = cw[3];
			cw[3] = t;
			t = cw[1];
			cw[1] = cw[2];
			cw[2] = t;
		}
#endif

		u32 a = result[0];
		u32 b = result[1];
		u32 c = result[2];
		u32 d = result[3];
		u32 e = result[4];

		#define sha1macro(func,val)			\
				{							\
					const u32 t = sha1_rol(a, 5)+(func)+e+val+w[j]; \
					e = d; d = c;			\
					c = sha1_rol(b, 30);	\
					b = a; a = t;			\
				}

		int j=0;
		while(j<16)
		{
			sha1macro((b&c)|(~b&d),0x5A827999)
			j++;
		}
		while(j<20)
		{
			w[j] = sha1_rol((w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16]), 1);
			sha1macro((b&c)|(~b&d),0x5A827999)
			j++;
		}
		while(j<40)
		{
			w[j] = sha1_rol((w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16]), 1);
			sha1macro(b^c^d,0x6ED9EBA1)
			j++;
		}
		while(j<60)
		{
			w[j] = sha1_rol((w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16]), 1);
			sha1macro((b&c)|(b&d)|(c&d),0x8F1BBCDC)
			j++;
		}
		while(j<80)
		{
			w[j] = sha1_rol((w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16]), 1);
			sha1macro(b^c^d,0xCA62C1D6)
			j++;
		}

		#undef sha1macro

		result[0] += a;
		result[1] += b;
		result[2] += c;
		result[3] += d;
		result[4] += e;
	}



	xsha1_generator::xsha1_generator()
	{
		
	}

	/*  
	 *  Reset
	 *
	 *  Description:
	 *      This function will initialize the sha1 class member variables
	 *      in preparation for computing a new message digest.
	 *
	 *  Parameters:
	 *      None.
	 *
	 *  Returns:
	 *      Nothing.
	 *
	 *  Comments:
	 *
	 */
	void xsha1_generator::open()
	{
		mMessage_Length      = 0;
		mMessage_Block_Index = 0;

		mH[0] = 0x67452301;
		mH[1] = 0xEFCDAB89;
		mH[2] = 0x98BADCFE;
		mH[3] = 0x10325476;
		mH[4] = 0xC3D2E1F0;

		mComputed    = false;
		mCorrupted   = false;
	}

	/*  
	 *  Result
	 *
	 *  Description:
	 *      This function will return the 160-bit message digest into the
	 *      array provided.
	 *
	 *  Parameters:
	 *      message_digest_array: [out]
	 *          This is an array of five unsigned integers which will be filled
	 *          with the message digest that has been computed.
	 *
	 *  Returns:
	 *      True if successful, false if it failed.
	 *
	 *  Comments:
	 *
	 */
	bool xsha1_generator::close(xhash160& hash)
	{
		if (mCorrupted)
		{
			return false;
		}

		if (!mComputed)
		{
			// We need to add 1 '1' bit to the end of the message 
			// Plus the length of the message in bits as a 64 bits value
			u64 const final_message_length = ((mMessage_Length + 1 + 64 + 511) & X_CONSTANT_U64(0xfffffffffffffe00));

			// How many bytes do we have to add ?
			u64 bytes_to_add = (final_message_length - mMessage_Length) / 8;

			// The remainder
			u8 remainder[128];
			x_memset(remainder, 0, 128);
			remainder[0] = 0x80;	// The '1' bit
			xmem_utils::writeunaligned64((u64*)&remainder[bytes_to_add - 8], mMessage_Length);

			u8 const* premainder = remainder;
			while (bytes_to_add!=0)
			{
				while (mMessage_Block_Index!=64)
				{
					mMessage_Block[mMessage_Block_Index++] = *premainder++;
					--bytes_to_add;
				}
				mMessage_Block_Index = 0;
				sha1_compute(mH,(u32*)mMessage_Block);
			}

			mComputed = true;
		}

		// On little endian we need to convert the hash
#ifdef X_LITTLE_ENDIAN
		u8* cw = (u8*)mH;
		for (s32 i=0; i<80; i+=4, cw+=4)
		{
			u8 t = cw[0];
			cw[0] = cw[3];
			cw[3] = t;
			t = cw[1];
			cw[1] = cw[2];
			cw[2] = t;
		}
#endif

		hash.set(mH[0], mH[1], mH[2], mH[3], mH[4]);
		return true;
	}

	/*  
	 *  Input
	 *
	 *  Description:
	 *      This function accepts an array of octets as the next portion of
	 *      the message.
	 *
	 *  Parameters:
	 *      message_array: [in]
	 *          An array of characters representing the next portion of the
	 *          message.
	 *
	 *  Returns:
	 *      Nothing.
	 *
	 *  Comments:
	 *
	 */
	void xsha1_generator::compute(void const* inBuffer, u32 inLength)
	{
		const u8 *src = (const u8*)inBuffer;

		u32 len = inLength;
		while (len!=0)
		{
			while (mMessage_Block_Index<64 && len!=0)
			{
				// This line will swap endian on big endian and keep endian on little endian.
				mMessage_Block[mMessage_Block_Index++]=*src++;
				--len;
			}

			if (mMessage_Block_Index == 64)
			{
				sha1_compute(mH, (u32*)mMessage_Block);
				mMessage_Block_Index = 0;
			}
		}

		mMessage_Length += inLength * 8;
	}




	xsha1	x_Sha1Hash(void const* inBuffer, s32 inLength)
	{
		xsha1_generator g;
		g.open();
		g.compute(inBuffer, inLength);
		xsha1 h;
		if (g.close(h))
			return h;

		h.clear();
		return h;
	}

}
