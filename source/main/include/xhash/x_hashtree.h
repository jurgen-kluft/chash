// x_hashtree.h - Core Hash Tree 
#ifndef __XHASH_HASHTREE_H__
#define __XHASH_HASHTREE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
	/**
	 * @group		xhash
	 * @brief		Hash Tree implementation useful for doing content validation
	 * @URL         Reference -> http://www.merkle.com/‎
	 * 
	 * @required    @_hash is the hash of the root of the hash tree
	 *				@_count is the number of hashed items (number of leafs = (_count+31)/32)
	 *				@_hash_combiner can combine 2 hashes into 1
	 *				@_allocator allocator for allocating nodes and hashes
	 *
	 * @behavior	Submitting hashes is going to allocate nodes, leafs and hashes and if submitted
	 *              in the worst case random order (010101....010101) then this data structure will
	 *              use the most amount of memory.
	 *				So the aim is to submit hashes in a continues order (increasing or decreasing index).
	 *              The hash tree will collapse sub-trees when it can validate the node hash. The user
	 *              can submit a trusted hash into a node with @submit_trusted_hash().
	 *              The range is aligned to 8 since every leaf can hold the hashes of 4 items while a
	 *              node can have 2 child leafs.
	 * @example		You want to set a trusted hash on node X of a hash tree with item count = 1024:
	 *              
	 *                hash_tree->submit_trusted_hash(352, trusted_hash);
	 *              
	 *  1024                       (root,0)
	 *                            /        \
	 *                           /          \
	 *                          /            \
	 *  512                (256)              (768)
	 *                    /     \
	 *                   /       \
	 *  128         (128)         (384)
	 *                           /     \
	 *                          /       \
	 *  64                 (320)         (448)
	 *                    /     \
	 *                   /       \
	 *  32          (288)         (352)      <------   X
	 *              
	 */
	class xhashtree
	{
	public:
		struct hash_t
		{
			inline			hash_t() : digest(0), length(0) {}
			xbyte*			digest;
			u32				length;
		};

		struct node_t
		{
			xbyte				data[16];
		};

		class iallocator
		{
		public:
			virtual void		initialize(u32 _sizeof_node, u32 _sizeof_hash) = 0;

			virtual node_t*		node_allocate() = 0;
			virtual void		node_deallocate(u32) = 0;

			virtual hash_t		hash_allocate() = 0;
			virtual void		hash_deallocate(void*) = 0;
		};

		typedef void (*combiner_f)(hash_t const& _left, hash_t const& _right, hash_t& _result);

		inline				xhashtree(iallocator* _allocator, combiner_f _hash_combiner);
							~xhashtree();

		void				open(hash_t const& _roothash, u32 _count);
		void				close();

		bool				verify() const;

		void				submit_content_hash(u32 _index, hash_t const& _hash);				// item hash
		void				submit_trusted_hash(u32 _index, u32 _level, hash_t const& _hash);	// node hash
		
		s32					find_item_with_no_hash(u32& _index) const;							// return: 0=did not find item, 1=item was found
		s32					find_items_with_no_hash(u32* _index, u32 _max) const;				// return: 0=did not find any item, >0=number of items found

		s32					find_node_to_trust(u32& _index) const;								// return: 0=did not find item, 1=item was found
		s32					find_nodes_to_trust(u32* _index, u32 _max) const;					// return: 0=did not find any node, >0=number of nodes found

	private:
		iallocator*			allocator;
		combiner_f			combiner;
		u32					numNodes;
		u32					maxNodes;
		hash_t				rootHash;
		struct xnode_t*		rootNode;
	};


}
#endif	// __XHASH_HASHTREE_H__
