// x_hash160_generator.h - Core Hash160 Generator - 
#ifndef __XHASH_HASH160_GENERATOR_H__
#define __XHASH_HASH160_GENERATOR_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

namespace xcore
{
	struct xhash160;

	class xihash160_generator
	{
	public:
		///@name Updating
		virtual void		open() = 0;
		virtual void		compute(void const* inBuffer, u32 inLength) = 0;
		virtual bool		close(xhash160& hash) = 0;
	};

	
	/**
	@group		xhash
	@brief		hash160 generator interface
	@desc		This class represents the hash160 generator.
	**/
	class xhash160_generator
	{
	public:
		///@name Construction/Destruction
							xhash160_generator(xihash160_generator*);

		///@name Updating
		void				open();
		void				compute(void const* inBuffer, u32 inLength);
		bool				close(xhash160& hash);

	private:
		xihash160_generator*	mGenerator;
	};

}
#endif	///< __XHASH_HASH160_GENERATOR_H__
