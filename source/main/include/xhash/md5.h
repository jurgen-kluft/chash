// HashMD5.h - Core MD5 Hash value 
#ifndef __CORE_MD5HASH_H__
#define __CORE_MD5HASH_H__
#include "CCore\CCore.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif
BEGIN_CORE_NAMESPACE

// Forward declares
DECLARE_CLASS(String)

// Local declares
DECLARE_CLASS(MD5HashValue)
DECLARE_CLASS(MD5Hash)



/**
@group		CHash
@brief		MD5 hash value
@desc		This struct represents the MD5 hash value ("message digest"). It contains a
			16-byte (128 bits) value, returned by the MD5Hash class.
**/
class MD5Hash
{
public:
						MD5Hash()												{ Clear(); }
						MD5Hash(rcString inString)								{ FromString(inString); }

	///@name Equality
	bool				operator==(rcMD5Hash inRHS) const						{ return mData32[0] == inRHS.mData32[0] && mData32[1] == inRHS.mData32[1] && mData32[2] == inRHS.mData32[2] && mData32[3] == inRHS.mData32[3]; }
	bool				operator!=(rcMD5Hash inRHS) const						{ return !(*this == inRHS); }
	bool				operator<(rcMD5Hash inRHS) const;
	bool				operator>(rcMD5Hash inRHS) const;

	///@name The MD5 hash value
	void				SetMD5(uint32 inR1, uint32 inR2, uint32 inR3, uint32 inR4);
	void				GetMD5(rUInt32 outR1, rUInt32 outR2, rUInt32 outR3, rUInt32 outR4) const;

	///@name Hashing
	inline uint32		GetHash() const											{ return mData32[0]; } ///< Get hash value (when MD5Hash is used as a key in a hash set or map)

	///@name To/From String
	bool				ToString(rString outString) const;						///< Convert MD5 hash value to String
	bool				FromString(rcString inString);							///< Set MD5 hash value from String

protected:
	void				Clear()													{ SetMD5(0,0,0,0); }

	///@name Data
	union
	{
		uint8			mData8[16];												///< MD5 Message digest as sixteen bytes
		uint32			mData32[4];												///< MD5 Message digest as four 32-bit integers
	};
};



/**
@group		CHash
@brief		MD5 hash function
@desc		This class represents the MD5 hash function. This code is a modification from
			the implementation by Colin Plumb. He placed this code in the public domain,
			no copyright is claimed.

			The original algorithm was developed by Ron Rivest (the 'R' in RSA Security).

			The MD5 hash algorithm calculates a hash value ("message digest") of a set of
			bytes (the "message"). MD5 hash is performed over chunks of 64 bytes.

			To retrieve the MD5 hash value of an arbitrary set of bytes, construct an
			MD5Hash object, call Update() on all of the data that needs to be hashed
			and call the GetHash() function to retrieve the MD5Hash.
**/
class MD5
{
	enum EState
	{
		OPEN,
		CLOSED,
	};
public:
	///@name Construction/Destruction
						MD5();

	///@name Updating
	void				Open();
	void					Compute(pcVoid inBuffer, int inLength);
	MD5Hash				Close();

private:
	void				Transform();

	EState				mState;
	uint32				mMD5[4];												///< 128 bits MD5 hash value
	uint64				mLength;
	struct
	{
		uint32			mInput[16];												///< 64 byte input buffer
		uint8			mSlack[7];												///< Slack space because internal memcopy copies gready (i.e. up to 7 bytes past mInput)
	} mBuffer ALIGN16;
};



//---------------------------------------------------------------------------------------------------------------------
//	Utilities
//---------------------------------------------------------------------------------------------------------------------

extern MD5Hash			gMD5Hash(pcVoid inBuffer, int inLength);				///< Get MD5 hash value of a block of data


END_CORE_NAMESPACE
#endif
