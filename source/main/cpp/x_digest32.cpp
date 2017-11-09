/**
 * @file x_digest32.cpp
 *
 * Core Hash32
 */

// x_digest32.cpp - Core Hash 32
#include "xbase/x_target.h"
#include "xhash/private/x_digest_murmur32.h"

namespace xcore
{
	xdigest32		x_Hash32(const char* inBuffer, u32 inLength)
	{
		return xdigest_murmur32::buf(inBuffer, inLength, 0);
	}

}
