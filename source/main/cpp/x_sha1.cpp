// x_md5.cpp - Core MD5 hash value 
#include "xbase\x_target.h"
#include "xbase\x_va_list.h"
#include "xbase\x_string_std.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_endian.h"

#include "xhash\x_sha1.h"

namespace xcore
{

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
	//

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
		mLength_Low          = 0;
		mLength_High         = 0;
		mMessage_Block_Index = 0;

		mH[0]        = 0x67452301;
		mH[1]        = 0xEFCDAB89;
		mH[2]        = 0x98BADCFE;
		mH[3]        = 0x10325476;
		mH[4]        = 0xC3D2E1F0;

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
			padMessage();
			mComputed = true;
		}

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
	void xsha1_generator::compute(void const* inBuffer, s32 inLength)
	{
		if (!inLength)
		{
			return;
		}

		if (mComputed || mCorrupted)
		{
			mCorrupted = true;
			return;
		}

		xbyte const* buffer = (xbyte const*)inBuffer;
		while(inLength-- && !mCorrupted)
		{
			mMessage_Block[mMessage_Block_Index++] = (*buffer & 0xFF);

			mLength_Low += 8;
			mLength_Low &= 0xFFFFFFFF;               // Force it to 32 bits
			if (mLength_Low == 0)
			{
				mLength_High++;
				mLength_High &= 0xFFFFFFFF;          // Force it to 32 bits
				if (mLength_High == 0)
				{
					mCorrupted = true;               // Message is too long
				}
			}

			if (mMessage_Block_Index == 64)
			{
				processMessageBlock();
			}

			buffer++;
		}
	}

	/*  
	 *  sCircularShift
	 *
	 *  Description:
	 *      This member function will perform a circular shifting operation.
	 *
	 *  Parameters:
	 *      bits: [in]
	 *          The number of bits to shift (1-31)
	 *      word: [in]
	 *          The value to shift (assumes a 32-bit integer)
	 *
	 *  Returns:
	 *      The shifted value.
	 *
	 *  Comments:
	 *
	 */
	inline static 
	u32 sCircularShift(s32 bits, u32 word) 
	{
		return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32-bits));
	}

	/*  
	 *  ProcessMessageBlock
	 *
	 *  Description:
	 *      This function will process the next 512 bits of the message
	 *      stored in the Message_Block array.
	 *
	 *  Parameters:
	 *      None.
	 *
	 *  Returns:
	 *      Nothing.
	 *
	 *  Comments:
	 *      Many of the variable names in this function, especially the single
	 *      character names, were used because those were the names used
	 *      in the publication.
	 *
	 */
	void xsha1_generator::processMessageBlock()
	{
		const unsigned K[] =    {               // Constants defined for SHA-1
									0x5A827999,
									0x6ED9EBA1,
									0x8F1BBCDC,
									0xCA62C1D6
								};
		int         t;                          // Loop counter
		unsigned    temp;                       // Temporary word value
		unsigned    W[80];                      // Word sequence
		unsigned    A, B, C, D, E;              // Word buffers

		/*
		 *  Initialize the first 16 words in the array W
		 */
		for(t = 0; t < 16; t++)
		{
			W[t]  = ((u32) mMessage_Block[t * 4]) << 24;
			W[t] |= ((u32) mMessage_Block[t * 4 + 1]) << 16;
			W[t] |= ((u32) mMessage_Block[t * 4 + 2]) << 8;
			W[t] |= ((u32) mMessage_Block[t * 4 + 3]);
		}

		for(t = 16; t < 80; t++)
		{
		   W[t] = sCircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
		}

		A = mH[0];
		B = mH[1];
		C = mH[2];
		D = mH[3];
		E = mH[4];

		for(t = 0; t < 20; t++)
		{
			temp = sCircularShift(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
			temp &= 0xFFFFFFFF;
			E = D;
			D = C;
			C = sCircularShift(30,B);
			B = A;
			A = temp;
		}

		for(t = 20; t < 40; t++)
		{
			temp = sCircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
			temp &= 0xFFFFFFFF;
			E = D;
			D = C;
			C = sCircularShift(30,B);
			B = A;
			A = temp;
		}

		for(t = 40; t < 60; t++)
		{
			temp = sCircularShift(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
			temp &= 0xFFFFFFFF;
			E = D;
			D = C;
			C = sCircularShift(30,B);
			B = A;
			A = temp;
		}

		for(t = 60; t < 80; t++)
		{
			temp = sCircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
			temp &= 0xFFFFFFFF;
			E = D;
			D = C;
			C = sCircularShift(30,B);
			B = A;
			A = temp;
		}

		mH[0] = (mH[0] + A) & 0xFFFFFFFF;
		mH[1] = (mH[1] + B) & 0xFFFFFFFF;
		mH[2] = (mH[2] + C) & 0xFFFFFFFF;
		mH[3] = (mH[3] + D) & 0xFFFFFFFF;
		mH[4] = (mH[4] + E) & 0xFFFFFFFF;

		mMessage_Block_Index = 0;
	}

	/*  
	 *  PadMessage
	 *
	 *  Description:
	 *      According to the standard, the message must be padded to an even
	 *      512 bits.  The first padding bit must be a '1'.  The last 64 bits
	 *      represent the length of the original message.  All bits in between
	 *      should be 0.  This function will pad the message according to those
	 *      rules by filling the message_block array accordingly.  It will also
	 *      call ProcessMessageBlock() appropriately.  When it returns, it
	 *      can be assumed that the message digest has been computed.
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
	void xsha1_generator::padMessage()
	{
		/*
		 *  Check to see if the current message block is too small to hold
		 *  the initial padding bits and length.  If so, we will pad the
		 *  block, process it, and then continue padding into a second block.
		 */
		if (mMessage_Block_Index > 55)
		{
			mMessage_Block[mMessage_Block_Index++] = 0x80;
			while(mMessage_Block_Index < 64)
			{
				mMessage_Block[mMessage_Block_Index++] = 0;
			}

			processMessageBlock();

			while(mMessage_Block_Index < 56)
			{
				mMessage_Block[mMessage_Block_Index++] = 0;
			}
		}
		else
		{
			mMessage_Block[mMessage_Block_Index++] = 0x80;
			while(mMessage_Block_Index < 56)
			{
				mMessage_Block[mMessage_Block_Index++] = 0;
			}

		}

		/*
		 *  Store the message length as the last 8 octets
		 */
		mMessage_Block[56] = (mLength_High >> 24) & 0xFF;
		mMessage_Block[57] = (mLength_High >> 16) & 0xFF;
		mMessage_Block[58] = (mLength_High >> 8) & 0xFF;
		mMessage_Block[59] = (mLength_High) & 0xFF;
		mMessage_Block[60] = (mLength_Low >> 24) & 0xFF;
		mMessage_Block[61] = (mLength_Low >> 16) & 0xFF;
		mMessage_Block[62] = (mLength_Low >> 8) & 0xFF;
		mMessage_Block[63] = (mLength_Low) & 0xFF;

		processMessageBlock();
	}
}