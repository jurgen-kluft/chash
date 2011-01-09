// UUID.h - Core Universally Unique Identifier implementation - 
#ifndef __XHASH_UUID_H__
#define __XHASH_UUID_H__
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif
namespace xcore {

// Local declares
DECLARE_CLASS(UUID)

// Forward declares
class String;


/**
@ingroup xhash
@brief Universally Unique Identifier

The UUID class is the Core implementation of the distributed computing environment (DCE) universally
unique identifier (UUID). A UID is a 128-bit number which can be used to identify classes, versions,
interfaces and so on. UIDs are often used for safe-casting (asking a class if it can be casted to
a certain other class, if the other class's interface has been tagged with a UID). This is the
standard way in which COM's QueryInterface method works, but it doesn't only apply to COM, you can
also use it in your own classes.

More information on UUIDs, DCE and RPC (UUIDs are extensively used in distributed computing and
remote procudure calls) can be found at:

	- http://www.opengroup.org/publications/catalog/c706.htm 
	- http://www.opengroup.org/dce/info/draft-leach-uuids-guids-01.txt		
**/
class UUID
{
public:
	/// Construction/Destruction
	inline				UUID()													{ *this=sNil; } ///< Construct an empty UUID (sets UUID to UUID::sNil)
	inline				UUID(rcUUID inOther)									: mData1(inOther.mData1), mData2(inOther.mData2), mData3(inOther.mData3), mData4(inOther.mData4), mHash(inOther.mHash) { } ///< Copy a UUID from another UUID
						UUID(u32 d1, u16 w1, u16 w2, u8 b1, u8 b2, u8 b3, u8 b4, u8 b5, u8 b6, u8 b7, u8 b8); ///< Construct UUID in form: u32 - u16 - u16 - u16 - 8 x u8
						UUID(u32 data_1, u32 data_2, u32 data_3, u32 data_4); ///< Construct UUID from 4 raw 32bit data values
						UUID(u32 data_1, u32 data_2, u32 data_3, u32 data_4, u32 hash); ///< Construct UUID from 4 raw 32bit data values and their resulting hash value

	/// Hashing
	f_inline u32		Hash() const											{ return mHash; } ///< Return the hash value of this UID

	/// Operators
	inline bool			operator==(rcUUID inRHS) const							{ return gMemCompare(this, &inRHS, sizeof(UUID))==0; } ///< Check if two UUIDs are equal
	inline bool			operator!=(rcUUID inRHS) const							{ return gMemCompare(this, &inRHS, sizeof(UUID))!=0; } ///< Check if two UUIDs are inequal

	/// UUID -> String
	String				ToString();												///< Convert UUID to string representation "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"

	/// Standard UIDs
	static UUID			sNil;													///< The special Nil UID (all bits set to 0)

private:
	u32				mData1, mData2, mData3, mData4;							///< 128-bit UID data
	u32				mHash;													///<  32-bit UID hash value (always cached, because UIDs are often used for lookup and/or comparison purposes)
};


}
#endif
