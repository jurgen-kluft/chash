/**
 * @file x_murmur64.cpp
 *
 * xCore Hash functions
 */

// x_murmur64.cpp - xCore Hash functions 
#include "xbase/x_target.h"
#include "xbase/x_endian.h"
#include "xbase/x_string_ascii.h"
#include "xhash/x_murmur64.h"
#include "xhash/private/x_digest_murmur64.h"

namespace xcore
{
	void		xdigest_engine_murmur64::update(xcbuffer const& buffer)
	{
		mHash = xdigest_murmur64::buf(buffer, mHash);
	}
}
