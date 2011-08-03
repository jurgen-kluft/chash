// x_hash128_generator.h - Core Hash128 Generator - 
#ifndef __XHASH_HASH128_GENERATOR_H__
#define __XHASH_HASH128_GENERATOR_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

namespace xcore
{
	class xihash128_generator
	{
	public:
		///@name Updating
		virtual void		open() = 0;
		virtual void		compute(void const* inBuffer, s32 inLength) = 0;
		virtual bool		close(xhash128& hash) = 0;
	};

	
	/**
	@group		xhash
	@brief		hash128 generator interface
	@desc		This class represents the hash128 generator.
	**/
	class xhash128_generator
	{
	public:
		///@name Construction/Destruction
							xhash128_generator(xihash128_generator*);

		///@name Updating
		void				open();
		void				compute(void const* inBuffer, s32 inLength);
		bool				close(xhash128& hash);

	private:
		xihash128_generator*	mGenerator;
	};

}
#endif	///< __XHASH_HASH128_GENERATOR_H__
