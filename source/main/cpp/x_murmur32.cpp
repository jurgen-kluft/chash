/**
 * @file x_murmur32.cpp
 *
 * xCore Hash functions
 */

// x_murmur32.cpp - xCore Hash functions 
#include "xbase\x_types.h"
#include "xbase\x_endian.h"
#include "xbase\x_string.h"
#include "xbase\x_string_std.h"
#include "xhash\x_murmur32.h"
#include "xhash\private\x_digest_murmur32.h"

namespace xcore
{
	void		xdigest_engine_murmur32::update(void const* inBuffer, s32 inLength)
	{
		mHash = xdigest_murmur32::buf(inBuffer, inLength, mHash);
	}
}
