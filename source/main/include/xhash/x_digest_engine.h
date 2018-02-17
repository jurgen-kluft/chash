/**
 * @file x_digest_engine.h
 *
 * Core Digest Engine
 */

// x_digest_engine.h - Core Digest Engine - 
#ifndef __XHASH_DIGEST_ENGINE_H__
#define __XHASH_DIGEST_ENGINE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_buffer.h"

namespace xcore
{
	class xdigest_engine
	{
	public:
		virtual u32			length() const = 0;
		virtual void		reset() = 0;
		virtual void		update(xcbuffer const&) = 0;
		virtual void		digest(xbuffer&) = 0;
	};
}

#endif	///< __XHASH_DIGEST_ENGINE_H__
