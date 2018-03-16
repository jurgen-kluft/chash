/**
 * @file x_hash.cpp
 *
 * Core Hash functions
 */

// x_hash.cpp - Core Hash functions 
#include "xbase/x_target.h"
#include "xhash/private/x_digest_engine_fnv.h"

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
	xdigest32 xdigest_fnv::buf(xcbuffer const& buffer)
	{
		return buf(buffer, FNV_32_PRIME);
	}

	xdigest32	xdigest_fnv::buf(xcbuffer const& buffer, xdigest32 hval)
	{
		unsigned char *bp = (unsigned char *)buffer.m_data;		/* start of buffer */
		unsigned char *be = bp + buffer.size();					/* beyond end of buffer */

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
	xdigest32	xdigest_fnv::str(xcuchars const& _str)
	{
		return str(_str, FNV_32_PRIME);
	}

	xdigest32	xdigest_fnv::str(xcuchars const& _str, xdigest32 _hval)
	{
		uchar const * s = (uchar const *)_str.m_str;	/* unsigned string */

		/*
		* FNV-1a hash each octet in the buffer
		*/
		while (s < _str.m_end)
		{

			/* xor the bottom with the current octet */
			_hval ^= (xdigest32)*s++;

			/* multiply by the 32 bit FNV magic prime mod 2^32 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
			_hval *= FNV_32_PRIME;
#else
			_hval += (_hval <<1) + (_hval <<4) + (_hval <<7) + (_hval <<8) + (_hval <<24);
#endif
		}

		/* return our new hash value */
		return _hval;
	}

}
