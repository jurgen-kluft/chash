/**
 * @file x_hash64_generator.h
 *
 * Core Hash64 Generator
 */

// x_hash64_generator.h - Core Hash64 Generator - 
#ifndef __XHASH_HASH64_GENERATOR_H__
#define __XHASH_HASH64_GENERATOR_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

namespace xcore
{
	typedef		u64			xdigest64;

	class xihash64_generator
	{
	public:
		///@name Updating
		virtual void		open() = 0;
		virtual void		compute(void const* inBuffer, s32 inLength) = 0;
		virtual bool		close(xdigest64& hash) = 0;
	};

	
	/**
	 * @group		xhash
	 * @brief		hash64 generator interface
	 * @desc		This class represents the hash64 generator.
	 */
	class xhash64_generator
	{
	public:
		///@name Construction/Destruction
							xhash64_generator(xihash64_generator*);

		///@name Updating
		void				open();
		void				compute(void const* inBuffer, s32 inLength);
		bool				close(xdigest64& hash);

	private:
		xihash64_generator*	mGenerator;
	};

}
#endif	///< __XHASH_HASH64_GENERATOR_H__
