/**
 * @file x_uuid.h
 *
 * Universally Unique Identifier implementation
 */

// x_uuid.h - Universally Unique Identifier implementation - 
#ifndef __XHASH_UUID_H__
#define __XHASH_UUID_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
	// Forward declares
	class xcstring;


	/**
	 * @ingroup	xhash
	 * @brief		Universally Unique Identifier

	 * 			The UUID class is the Core implementation of the distributed computing environment (DCE) universally
	 * 			unique identifier (UUID). A UID is a 128-bit number which can be used to identify classes, versions,
	 * 			interfaces and so on. UIDs are often used for safe-casting (asking a class if it can be casted to
	 * 			a certain other class, if the other class's interface has been tagged with a UID). This is the
	 * 			standard way in which COM's QueryInterface method works, but it doesn't only apply to COM, you can
	 * 			also use it in your own classes.

	 * 			More information on UUIDs, DCE and RPC (UUIDs are extensively used in distributed computing and
	 * 			remote procedure calls) can be found at:

	 * 	- http: * www.opengroup.org/publications/catalog/c706.htm 
	 * 	- http: * www.opengroup.org/dce/info/draft-leach-uuids-guids-01.txt		
	 */
	class xuuid
	{
	public:
		/// Construction/Destruction
		inline				xuuid()													{ *this=sNil; } ///< Construct an empty UUID (sets UUID to UUID::sNil)
		inline				xuuid(xuuid const& inOther)								: mData1(inOther.mData1), mData2(inOther.mData2), mData3(inOther.mData3), mData4(inOther.mData4), mHash(inOther.mHash) { } ///< Copy a UUID from another UUID
							xuuid(u32 d1, u16 w1, u16 w2, u8 b1, u8 b2, u8 b3, u8 b4, u8 b5, u8 b6, u8 b7, u8 b8); ///< Construct UUID in form: u32 - u16 - u16 - u16 - 8 x u8
							xuuid(u32 data_1, u32 data_2, u32 data_3, u32 data_4); ///< Construct UUID from 4 raw 32bit data values
							xuuid(u32 data_1, u32 data_2, u32 data_3, u32 data_4, u32 hash); ///< Construct UUID from 4 raw 32bit data values and their resulting hash value

		/// Hashing
		f_inline u32		hash() const											{ return mHash; } ///< Return the hash value of this UID

		/// Operators
		inline bool			operator==(xuuid const& inRHS) const					{ return mData1==inRHS.mData1 && mData2==inRHS.mData2 && mData3==inRHS.mData3 && mData4==inRHS.mData4; } ///< Check if two UUIDs are equal
		inline bool			operator!=(xuuid const& inRHS) const					{ return mHash!=inRHS.mHash; } ///< Check if two UUIDs are inequal

		/// UUID -> String
		bool				toString(xcstring& outString);							///< Convert UUID to string representation "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"

		/// Standard UIDs
		static xuuid		sNil;													///< The special Nil UID (all bits set to 0)

	private:
		u32					mData1, mData2, mData3, mData4;							///< 128-bit UID data
		u32					mHash;													///<  32-bit UID hash value (always cached, because UIDs are often used for lookup and/or comparison purposes)
	};


	/**
	@ingroup	xhash
	@brief		UUID generator (data-time, mac-addresse, process-id based UUID)
	// TODO
	//extern xuuid			gGenerateUUID(u16 year, u8 day_of_year, u8 hour, u8 minute, u8 second, u8 milli_second, u16 process_id, u64 mac_addresse);
	**/

	/**
	 * @ingroup	xhash
	 * @brief		UUID generator (name based UUID, see RFC 4122(http: * www.ietf.org/rfc/rfc4122.txt))

	 * 			Generates the UUID from a domain and name.

	 * 			The requirements for these types of UUIDs are as follows:

	 * 				The UUIDs generated at different times from the same name in the same namespace MUST be equal.
	 * 				The UUIDs generated from two different names in the same namespace should be different (with very high probability).
	 * 				The UUIDs generated from the same name in two different namespaces should be different with (very high probability).
	 * 				If two UUIDs that were generated from names are equal, then they were generated from the same name in the same namespace (with very high probability).

	 */
	// TODO
	//extern xuuid			gGenerateUUID(const char* domain, const char* name);
}

#endif	///< __XHASH_UUID_H__
