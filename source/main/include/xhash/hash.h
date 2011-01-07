// Hash.h - Core Hash functions - 
#ifndef __CORE_HASHING_H__
#define __CORE_HASHING_H__
#include "CCore\CCore.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "CSystem\Types.h"
#include "CSystem\Defines.h"

BEGIN_CORE_NAMESPACE


//---------------------------------------------------------------------------------------------------------------------
//	Hash Utility functions
//---------------------------------------------------------------------------------------------------------------------
class Hashing
{
public:
	// Hashing functions, uses
	static uint32			Hash(pcVoid inData, uint32 inLength, uint32 pc = 0);

	static void				Hash2(pcVoid inData, uint32 inLength, uint32& pc, uint32& pb);
	static uint32			Hash2(pcVoid inData, uint32 inLength, uint32 pc = 0);

	static void				Hash2(pcUInt32 inData, uint32 inLength, uint32& pc, uint32& pb);
	static uint32			Hash2(pcUInt32 inData, uint32 inLength, uint32 pc = 0);

	/**
	@group		CHash
	@brief		Fast general hash function for uint32
	@desc		This is a fast general hash function for uint32 data types.
				http://www.concentric.net/~ttwang/tech/inthash.htm
	**/
	static inline uint32	Hash(const uint32 inUInt32)
	{
		uint32 key = inUInt32;
		key += ~(key << 16);
		key ^=  (key >>  5);
		key +=  (key <<  3);
		key ^=  (key >> 13);
		key += ~(key <<  9);
		key ^=  (key >> 17);
  		return key;
	}

	static inline uint32	Hash(int32 inInt32)									{ return Hash((uint32)inInt32); }

	/**
	@group		CHash
	@brief		Fast general hash function for uint64
	@desc		This is a fast general hash function for uint64 data types.
				General hash function for uint64
	**/
	static inline uint32	Hash(const uint64 inData)
	{
		uint32 a = 0x9e3779b9;
		uint32 b = 0x9e3779b9;
		uint32 c = 8;
		uint8  *p_in = (uint8 *)&inData;
		
		// Handle 8 bytes
		b+=((uint32)p_in[7]<<24);
		b+=((uint32)p_in[6]<<16);
		b+=((uint32)p_in[5]<<8);
		b+=p_in[4];

		a+=((uint32)p_in[3]<<24);
		a+=((uint32)p_in[2]<<16);
		a+=((uint32)p_in[1]<<8);
		a+=p_in[0];
		
		a -= b; a -= c; a ^= (c>>13);
		b -= c; b -= a; b ^= (a<<8);
		c -= a; c -= b; c ^= (b>>13);
		a -= b; a -= c; a ^= (c>>12);
		b -= c; b -= a; b ^= (a<<16);
		c -= a; c -= b; c ^= (b>>5);
		a -= b; a -= c; a ^= (c>>3);
		b -= c; b -= a; b ^= (a<<10);
		c -= a; c -= b; c ^= (b>>15);
		return c;
	}

	static inline uint32	Hash(int64 inInt64)									{ return Hash((uint64)inInt64); }
};


//---------------------------------------------------------------------------------------------------------------------
// Template class to make types hashable
//---------------------------------------------------------------------------------------------------------------------



/**
@group		CHash
@brief		hashable template class

			The hashable<T> template creates a hashable version of <T>,
			by using the standard Hashing::Hash() function on the memory block occupied by
			<mData>. taTYPEhis is a simple - though not so efficient - solution to
			make most structs and internal types hashable.

			You should not try to use the hashable class on structs and classes that
			have pointer members, because the hash value is calculated on the memory
			block occupied by the type and will most likely not return valid hash
			values for the data pointer to by members.

			Because the hashable template stores the hash value of the type it embeds,
			changing the value also requires updating the update() function.
**/
template <typename T>
class hashable
{
public:
	///@name Constructor/Assignment
	inline					hashable()											: mCachedHash(0) { }
	inline					hashable(const hashable<T>& inOther)				: mData(inOther.mData), mCachedHash(inOther.mCachedHash) { }
	inline explicit			hashable(const T& inData)							: mData(inData) { update(mData); }
	inline hashable<T>& 	operator=(const T& inRHS)							{ mData = inRHS; update(mData); return *this; }

	///@name Conversion to type <T>
							operator const T&() const							{ return mData; }
	
	///@name Relational operators
	inline bool				operator< (const T& inRHS) const					{ return mData <  inRHS; }
	inline bool				operator<=(const T& inRHS) const					{ return mData <= inRHS; }
	inline bool				operator==(const T& inRHS) const					{ return mData == inRHS; }
	inline bool				operator!=(const T& inRHS) const					{ return mData != inRHS; }
	inline bool				operator>=(const T& inRHS) const					{ return mData >= inRHS; }
	inline bool				operator> (const T& inRHS) const					{ return mData >  inRHS; }
	inline bool				operator< (const hashable<T>& inRHS) const			{ return mData <  inRHS.mData; }
	inline bool				operator<=(const hashable<T>& inRHS) const			{ return mData <= inRHS.mData; }
	inline bool				operator==(const hashable<T>& inRHS) const			{ return mData == inRHS.mData; }
	inline bool				operator!=(const hashable<T>& inRHS) const			{ return mData != inRHS.mData; }
	inline bool				operator>=(const hashable<T>& inRHS) const			{ return mData >= inRHS.mData; }
	inline bool				operator> (const hashable<T>& inRHS) const			{ return mData >  inRHS.mData; }

	///@name Arithmetic operators
	inline const hashable<T>& operator+=(const T& inRHS)						{ mData+=inRHS; update(mData); return *this; }
	inline const hashable<T>& operator-=(const T& inRHS)						{ mData-=inRHS; update(mData); return *this; }
	inline const hashable<T>& operator/=(const T& inRHS)						{ mData/=inRHS; update(mData); return *this; }
	inline const hashable<T>& operator*=(const T& inRHS)						{ mData*=inRHS; update(mData); return *this; }
	inline hashable<T>		operator+(const T& inRHS) const						{ return hashable<T>(mData + inRHS); }
	inline hashable<T>		operator-(const T& inRHS) const						{ return hashable<T>(mData - inRHS); }
	inline hashable<T>		operator/(const T& inRHS) const						{ return hashable<T>(mData / inRHS); }
	inline hashable<T>		operator*(const T& inRHS) const						{ return hashable<T>(mData * inRHS); }

	///@name Properties
	inline uint				hash() const										{ return mCachedHash; } // Return the hash value of this object

private:
	///@name Actions
	template<typename T>
	inline void				update(const T& data)								{ mCachedHash = Hashing::Hash(&data, sizeof(T)); } // Update the hash value from the value of <T>

	inline void 			update(const int32& data)							{ mCachedHash = Hashing::Hash((uint32)data); }
	inline void 			update(const uint32& data)							{ mCachedHash = Hashing::Hash((uint32)data); }
	inline void 			update(const int64& data)							{ mCachedHash = Hashing::Hash((uint64)mData); }
	inline void 			update(const uint64& data)							{ mCachedHash = Hashing::Hash((uint64)mData); }

	T						mData;
	uint					mCachedHash;
};



/**
@group		CHash
@brief		Hashed pointer template
**/
template <typename T>
struct hashed_ptr
{
	///@name Construction/Destruction
	inline				hashed_ptr()											: mPtr(NULL),  mHash(0) { }
	inline				hashed_ptr(T *inPtr)									: mPtr(inPtr), mHash(Hashing::Hash((uint32)inPtr)) { }

	///@name Operators
	inline bool			operator==(const hashed_ptr<T>& inRHS) const			{ return mPtr == inRHS.mPtr; }
	inline bool			operator!=(const hashed_ptr<T>& inRHS) const			{ return mPtr != inRHS.mPtr; }
	inline void			operator=(const hashed_ptr<T>& inRHS)					{ mPtr=inRHS.mPtr; mHash=inRHS.mHash; }
	inline void			operator=(T *inPtr)										{ mPtr=inPtr; mHash=Hashing::Hash((uint32)inPtr); }

	///@name Hashing
	inline T*			ptr() const												{ return mPtr; }
	inline uint32		hash() const											{ return mHash; }

	///@name Accessors
	inline T*			operator->()											{ return mPtr; }
	inline const T*		operator->() const										{ return mPtr; }
	inline T&			operator*()												{ return *mPtr; }
	inline const T&		operator*() const										{ return *mPtr; }
	inline				operator T *()											{ return mPtr; }
	inline				operator const T *() const								{ return mPtr; }

	///@name Data
	T*					mPtr;
	uint32				mHash;
};



/**
@group		CHash
@brief		Hashed void pointer

			Hashed void pointer. This is a concrete implementation of hashed_ptr, it's specified
			because hashed_ptr<void> cannot be instantiated due to the dereference on void.
**/
struct hashed_void_ptr
{
	///@name Construction/Destruction
	inline				hashed_void_ptr()										: mPtr(NULL),  mHash(0) { }
	inline				hashed_void_ptr(pVoid inPtr)							: mPtr(inPtr), mHash(Hashing::Hash((uint32)inPtr)) { }

	///@name Assignment
	inline void			operator=(const hashed_void_ptr& inRHS)					{ mPtr=inRHS.mPtr; mHash=inRHS.mHash; }
	inline void			operator=(pVoid inPtr)									{ mPtr=inPtr; mHash=Hashing::Hash((uint32)inPtr); }

	///@name Comparing operators
	inline bool			operator==(const hashed_void_ptr& inRHS) const			{ return mPtr == inRHS.mPtr; }
	inline bool			operator!=(const hashed_void_ptr& inRHS) const			{ return mPtr != inRHS.mPtr; }
	inline bool			operator< (const hashed_void_ptr& inRHS) const			{ return mPtr <  inRHS.mPtr; }
	inline bool			operator<=(const hashed_void_ptr& inRHS) const			{ return mPtr <= inRHS.mPtr; }
	inline bool			operator> (const hashed_void_ptr& inRHS) const			{ return mPtr >  inRHS.mPtr; }
	inline bool			operator>=(const hashed_void_ptr& inRHS) const			{ return mPtr >= inRHS.mPtr; }

	///@name Hashing
	inline uint32		hash() const											{ return mHash; }

	///@name Accessors
	inline pVoid		operator->()											{ return mPtr; }
	inline pcVoid		operator->() const										{ return mPtr; }
	inline				operator pVoid()										{ return mPtr; }
	inline				operator pcVoid() const									{ return mPtr; }

	///@name Data
	pVoid				mPtr;
	uint32				mHash;
};


END_CORE_NAMESPACE
#endif	///< __CORE_HASHING_H__