/**
 * @file x_hash32_generator.h
 *
 * Core Hash32 Generator
 */

// x_hash32_generator.h - Core Hash32 Generator - 
#ifndef __XHASH_HASH32_GENERATOR_H__
#define __XHASH_HASH32_GENERATOR_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

namespace xcore
{
	typedef		u32			xhash32;

	class xihash32_generator
	{
	public:
		///@name Updating
		virtual void		open() = 0;
		virtual void		compute(void const* inBuffer, s32 inLength) = 0;
		virtual bool		close(xhash32& hash) = 0;
	};

	
	/**
	 * @group		xhash
	 * @brief		hash32 generator interface
	 * @desc		This class represents the hash32 generator.
	 */
	class xhash32_generator
	{
	public:
		///@name Construction/Destruction
							xhash32_generator(xihash32_generator*);

		///@name Updating
		void				open();
		void				compute(void const* inBuffer, s32 inLength);
		bool				close(xhash32& hash);

	private:
		xihash32_generator*	mGenerator;
	};

}
#endif	///< __XHASH_HASH32_GENERATOR_H__
