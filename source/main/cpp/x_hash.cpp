// x_hash.cpp - Core Hash functions 
#include "xbase\x_types.h"
#include "xbase\x_string_std.h"
#include "xhash\x_hash.h"

namespace xcore
{
	/*
	* 32 bit magic FNV-1a prime
	*/
	#define FNV_32_PRIME ((xhash32)0x01000193)


	/*
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
	xhash32 xhash_generator_fnv::buf(void const* inData, u32 inLength)
	{
		return buf(inData, inLength, FNV_32_PRIME);
	}

	xhash32	xhash_generator_fnv::buf(void const *buf, u32 len, xhash32 hval)
	{
		unsigned char *bp = (unsigned char *)buf;	/* start of buffer */
		unsigned char *be = bp + len;		/* beyond end of buffer */

		/*
		* FNV-1a hash each octet in the buffer
		*/
		while (bp < be) 
		{

			/* xor the bottom with the current octet */
			hval ^= (xhash32)*bp++;

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


	/*
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
	xhash32	xhash_generator_fnv::str(char const *inStr)
	{
		return str(inStr, FNV_32_PRIME);
	}

	xhash32	xhash_generator_fnv::str(char const *inStr, xhash32 hval)
	{
		unsigned char *s = (unsigned char *)inStr;	/* unsigned string */

		/*
		* FNV-1a hash each octet in the buffer
		*/
		while (*s) 
		{

			/* xor the bottom with the current octet */
			hval ^= (xhash32)*s++;

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



	xcore::u32 gGetMurmurHash32(const char* str, u32 len, u32 seed)
	{
		// 'm' and 'r' are mixing constants generated offline.
		// They're not really 'magic', they just happen to work well.

		const xcore::u32 m = 0x5bd1e995;
		const xcore::s32 r = 24;
		xcore::u32 length = len;

		// Initialize the hash to a 'random' value

		xcore::u32 h = seed ^ length;

		// Mix 4 bytes at a time into the hash

		const xcore::u8* data = (const xcore::u8*)str;
		while( length >= 4 )
		{
			xcore::u32 k;

			k  = xcore::u32( data[0] );
			k |= xcore::u32( data[1] ) << 8;
			k |= xcore::u32( data[2] ) << 16;
			k |= xcore::u32( data[3] ) << 24;

			k *= m; 
			k ^= k >> r; 
			k *= m; 

			h *= m; 
			h ^= k;

			data += 4;
			length -= 4;

		}

		switch( length )
		{
		case 3: h ^= xcore::u32( data[2] ) << 16;
		case 2: h ^= xcore::u32( data[1] ) << 8;
		case 1: h ^= xcore::u32( data[0] );
			h *= m;
		};

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}


	xhash32			xhash_generator_murmur::buf(void const* inData, u32 inLength)
	{
		return gGetMurmurHash32((const char*)inData, inLength, 0);
	}

	xhash32			xhash_generator_murmur::buf(void const* inData, u32 inLength, xhash32 inPrevious)
	{
		return gGetMurmurHash32((const char*)inData, inLength, inPrevious);
	}

	xhash32			xhash_generator_murmur::str(char const* inStr)
	{
		return gGetMurmurHash32((const char*)inStr, x_strlen(inStr), 0);
	}

	xhash32			xhash_generator_murmur::str(char const* inStr, xhash32 inPrevious)
	{
		return gGetMurmurHash32((const char*)inStr, x_strlen(inStr), inPrevious);
	}

}
