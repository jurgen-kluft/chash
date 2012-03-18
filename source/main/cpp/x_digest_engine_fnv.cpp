/**
 * @file x_hash.cpp
 *
 * Core Hash functions
 */

// x_hash.cpp - Core Hash functions 
#include "xbase\x_types.h"
#include "xbase\x_string_std.h"
#include "xhash\private\x_digest_engine_fnv.h"

namespace xcore
{
	/**
	 * 32 bit magic FNV-1a prime
	 */
	#define FNV_32_PRIME ((xdigest32)0x01000193)


	/**
	 * fnv_32a_buf - perform a 32 bit Fowler/Noll/Vo FNV-1a hash on a buffer
	 *
	 * input:
	 *	buf	- start of buffer to hash
	 *	len	- length of buffer in octets
	 *	hval	- previous hash value or 0 if first call
	 *
	 * returns:
	 *	32 bit hash as a static hash type
	 *
	 * NOTE: To use the recommended 32 bit FNV-1a hash, use FNV1_32A_INIT as the
	 * 	 hval arg on the first call to either fnv_32a_buf() or fnv_32a_str().
	 */
	xdigest32 xdigest_fnv::buf(void const* inData, u32 inLength)
	{
		return buf(inData, inLength, FNV_32_PRIME);
	}

	xdigest32	xdigest_fnv::buf(void const *buf, u32 len, xdigest32 hval)
	{
		unsigned char *bp = (unsigned char *)buf;	/* start of buffer */
		unsigned char *be = bp + len;		/* beyond end of buffer */

		/*
		* FNV-1a hash each octet in the buffer
		*/
		while (bp < be) 
		{

			/* xor the bottom with the current octet */
			hval ^= (xdigest32)*bp++;

			/* multiply by the 32 bit FNV magic prime mod 2^32 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
			hval *= FNV_32_PRIME;
#else
			hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
#endif
		}

		/* return our new hash value */
		return hval;
	}


	/**
	 * fnv_32a_str - perform a 32 bit Fowler/Noll/Vo FNV-1a hash on a string
	 *
	 * input:
	 *	str	- string to hash
	 *	hval	- previous hash value or 0 if first call
	 *
	 * returns:
	 *	32 bit hash as a static hash type
	 *
	 * NOTE: To use the recommended 32 bit FNV-1a hash, use FNV1_32A_INIT as the
	 *  	 hval arg on the first call to either fnv_32a_buf() or fnv_32a_str().
	 */
	xdigest32	xdigest_fnv::str(char const *inStr)
	{
		return str(inStr, FNV_32_PRIME);
	}

	xdigest32	xdigest_fnv::str(char const *inStr, xdigest32 hval)
	{
		unsigned char *s = (unsigned char *)inStr;	/* unsigned string */

		/*
		* FNV-1a hash each octet in the buffer
		*/
		while (*s) 
		{

			/* xor the bottom with the current octet */
			hval ^= (xdigest32)*s++;

			/* multiply by the 32 bit FNV magic prime mod 2^32 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
			hval *= FNV_32_PRIME;
#else
			hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
#endif
		}

		/* return our new hash value */
		return hval;
	}

}
