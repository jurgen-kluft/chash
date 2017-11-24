/**
 * @file x_digest_murmur32.cpp
 *
 * xCore Hash functions
 */

// x_digest_murmur32.cpp - xCore Hash functions 
#include "xbase/x_target.h"
#include "xbase/x_endian.h"
#include "xbase/x_memory_std.h"
#include "xbase/x_string_ascii.h"

#include "xhash/private/x_digest_murmur32.h"

namespace xcore
{
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

	static void reverseEndianIfNeeded(const char* inData, char* outData, u32 len)
	{
		#ifdef X_BIG_ENDIAN
		
		for(xcore::u32 i = 0; i < len; i++)
		{
			outData[len - i - 1] = inData[i];
		}

		#else

		x_memcopy(outData, inData, len);
		#endif
	}


	xdigest32			xdigest_murmur32::buf(void const* inData, u32 inLength)
	{

		return gGetMurmurHash32((const char*)inData, inLength, 0);
	}

	xdigest32			xdigest_murmur32::buf(void const* inData, u32 inLength, xdigest32 inPrevious)
	{

		return gGetMurmurHash32((const char*)inData, inLength, inPrevious);
	}

	xdigest32			xdigest_murmur32::str(char const* inStr)
	{
		return gGetMurmurHash32((const char*)inStr, ascii::size(inStr), 0);
	}

	xdigest32			xdigest_murmur32::str(char const* inStr, xdigest32 inPrevious)
	{
		return gGetMurmurHash32((const char*)inStr, ascii::size(inStr), inPrevious);
	}

}
