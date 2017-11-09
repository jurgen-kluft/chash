/**
 * @file x_digest64.cpp
 *
 * Core Hash64
 */

// x_digest64.cpp - Core Hash 64
#include "xbase/x_target.h"
#include "xhash/private/x_digest_murmur64.h"

namespace xcore
{
	xdigest64		x_Hash64(const char* inBuffer, u32 inLength)
	{
		return xdigest_murmur64::buf(inBuffer, inLength, 0);
	}

}
