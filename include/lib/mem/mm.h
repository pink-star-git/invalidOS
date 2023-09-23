// include/lib/mem/mem.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "define/color.h"

#include "avl_tree.h"

struct s_alloc_t {
	u_int32 signature;
	u_int32 prev_size;
	u_int32 size;
	u_int32 tree_id;
	u_int32 status;
};

avl_tree mem_map;

u_char8 *
malloc(u_int32 size) {
	if (!size)
		return 0;

	u_char8 * addr = 0;
	u_int32 i;
	for (i = 0; i < mem_map.size; )
	{
		int r;
		avl_node *n;
		n = &mem_map.find_node(i);
		/* couldn't find it */
		if (!n->key) {
			return NULL;
		}
		r = allocationAvlTree.cmp(n->key, size);
		if (r <= 0)
		{
			//We're lucky today.
			//Get memory block header
			s_alloc_t * block = (size_t)n->val - sizeof(s_alloc_t);
			//Set to used
			block->status = 1;
			//Set size
			block->size = size;
			s_alloc_t * next = (size_t)n->val + size;
			next->prev_size = size;
			next->status = 0;
			next->size = n->key - size - 16;
			avltree_remove(&allocationAvlTree, n->key, n->val);
			if (n->key - size - 16)
				avltree_insert(&allocationAvlTree, next->size, (size_t)next + sizeof(s_alloc_t));
			memset((size_t)block + sizeof(s_alloc_t), 0, block->size); 
			block->signature = 0xDEADBEEF;
			unlockTaskSwitch();
			return (size_t)block + sizeof(s_alloc_t);
		}
		else if (r > 0)
			i = __child_r(i);
		else
			assert(0);
	}
	return 0;
}

void free(void * mem) {
	if (!mem)
		return;
	//Get current alloc
	s_alloc_t * alloc = ((unsigned int)mem - sizeof(s_alloc_t));
	if (alloc->signature != 0xDEADBEEF)
		return;
	alloc->status = 0;
	s_alloc_t * left = ((unsigned int)alloc - sizeof(s_alloc_t) - alloc->prev_size);
	if (left->signature == 0xDEADBEEF&&left->status == 0&&left->size==alloc->prev_size)
	{
		//Merge blocks
		if (avltree_remove(&allocationAvlTree, left->size, (uint)left + sizeof(s_alloc_t))) {
			left->size += sizeof(s_alloc_t) + alloc->size;
			alloc = left;
		}
		else assert(0);
	}
	s_alloc_t * right = (uint)alloc + sizeof(s_alloc_t) + alloc->size;
	if (right->prev_size&&right->status == 0&&right->signature == 0xDEADBEEF)
	{
		if (avltree_remove(&allocationAvlTree, right->size, (uint)right + sizeof(s_alloc_t)))
			alloc->size += sizeof(s_alloc_t) + right->size;
		else assert(0);
	}
	avltree_insert(&allocationAvlTree, alloc->size, (uint)alloc + sizeof(s_alloc_t));

}