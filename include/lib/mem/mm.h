// include/lib/mem/mem.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"

#include "mem.h"

#define MEM_BLOCK_USE 0x01

namespace sil { // standart invalid library
    struct mem_block_header {
        u_int32 signature;
        u_int32 size_prev;
        u_int32 size;
        u_int32 status;
        mem_block_header (u_int32 size, u_int32 size_pr) {
            signature = 0x1AFDCFA1;
            size = size;
            size_prev = size_pr;
            status = 0;
        };
    }  __attribute__((packed));

    static size_t mem_block_size = sizeof(mem_block_header);
    static u_char8 *mem_alloc = (size_t)(0x100000 + mem_block_size);
    void 
    mem_map_init () {
        mem_block_header *main_block = (mem_block_header *)mem_alloc - mem_block_size;
        main_block->size = 0x200000 - mem_block_size;
        main_block->size_prev = 0;
    }

    u_char8 *
    malloc (size_t size) {
        mem_block_header *block = (mem_block_header *)mem_alloc - mem_block_size;
        if (block->signature != 0x1AFDCFA1) return NULL;
        while (1)
        {
            if (!(block->status & MEM_BLOCK_USE) && (size < block->size)) {
                // if (size > 32) {
                    mem_block_header *new_block = block + mem_block_size + size;
                    new_block->size = block->size - size - mem_block_size;
                    new_block->size_prev = size;
                    block->size = size;
                // }
                block->status |= MEM_BLOCK_USE;
                return (u_char8 *)(block + mem_block_size);
            }
            else {
                block += block->size + mem_block_size;
            }
        }
    }

    void free(u_char8 *mem_ptr) {
        mem_block_header *block = (mem_block_header *)mem_ptr - mem_block_size;
        if (block->signature != 0x1AFDCFA1) return;
        memset(mem_ptr, block->size);
        block->status &= ~MEM_BLOCK_USE;
    }

    void count_memory(void)
    {
        register u_int32 *mem;
        u_int32 mem_count, a;
        u_short16 memkb;
        u_char8 irq1, irq2;
        u_int32 cr0;
    
        /* save IRQ's */
        irq1=inb(0x21);
        irq2=inb(0xA1);
    
        /* kill all irq's */
        outb(0x21, 0xFF);
        outb(0xA1, 0xFF);
    
        mem_count=0;
        memkb=0;
    
        // store a copy of CR0
        __asm__ __volatile("movl %%cr0, %%eax":"=a"(cr0))::"eax");
    
        // invalidate the cache
        // write-back and invalidate the cache
        __asm__ __volatile__ ("wbinvd");
    
        // plug cr0 with just PE/CD/NW
        // cache disable(486+), no-writeback(486+), 32bit mode(386+)
        __asm__ __volatile__("movl %%eax, %%cr0", ::
            "a" (cr0 | 0x00000001 | 0x40000000 | 0x20000000) : "eax");
    
        do {
            memkb++;
            mem_count += 1024*1024;
            mem= (u_int32 *) mem_count;
    
            a= *mem;
            *mem= 0x55AA55AA;
    
            // the empty asm calls tell gcc not to rely on what's in its registers
            // as saved variables (this avoids GCC optimisations)
            asm("":::"memory");
            if (*mem!=0x55AA55AA) mem_count=0;
            else {
                *mem=0xAA55AA55;
                asm("":::"memory");
                if(*mem!=0xAA55AA55)
                mem_count=0;
            }
    
            asm("":::"memory");
            *mem=a;
    
        } while (memkb<4096 && mem_count!=0);
    
        __asm__ __volatile__("movl %%eax, %%cr0", :: "a" (cr0) : "eax");
    
        mem_end = memkb<<20;
        mem = (u_int32 *) 0x413;
        bse_end= (*mem & 0xFFFF) <<6;
    
        outb(0x21, irq1);
        outb(0xA1, irq2);
    }



} // namespace sil


void *operator new(size_t size)
{
    return sil::malloc(size);
}

void *operator new[](size_t size)
{
    return sil::malloc(size);
}

void operator delete(void *p)
{
    sil::free(p);
}

void operator delete[](void *p)
{
    sil::free(p);
}

// avl_tree mem_map;

// u_char8 *
// malloc(u_int32 size) {
// 	if (!size)
// 		return 0;

// 	u_char8 * addr = 0;
// 	u_int32 i;
// 	for (i = 0; i < mem_map.size; )
// 	{
// 		int r;
// 		avl_node *n;
// 		n = &mem_map.find_node(i);
// 		/* couldn't find it */
// 		if (!n->key) {
// 			return NULL;
// 		}
// 		r = allocationAvlTree.cmp(n->key, size);
// 		if (r <= 0)
// 		{
// 			//We're lucky today.
// 			//Get memory block header
// 			s_alloc_t * block = (size_t)n->val - sizeof(s_alloc_t);
// 			//Set to used
// 			block->status = 1;
// 			//Set size
// 			block->size = size;
// 			s_alloc_t * next = (size_t)n->val + size;
// 			next->prev_size = size;
// 			next->status = 0;
// 			next->size = n->key - size - 16;
// 			avltree_remove(&allocationAvlTree, n->key, n->val);
// 			if (n->key - size - 16)
// 				avltree_insert(&allocationAvlTree, next->size, (size_t)next + sizeof(s_alloc_t));
// 			memset((size_t)block + sizeof(s_alloc_t), 0, block->size); 
// 			block->signature = 0xDEADBEEF;
// 			unlockTaskSwitch();
// 			return (size_t)block + sizeof(s_alloc_t);
// 		}
// 		else if (r > 0)
// 			i = __child_r(i);
// 		else
// 			assert(0);
// 	}
// 	return 0;
// }

// void free(void * mem) {
// 	if (!mem)
// 		return;
// 	//Get current alloc
// 	s_alloc_t * alloc = ((unsigned int)mem - sizeof(s_alloc_t));
// 	if (alloc->signature != 0xDEADBEEF)
// 		return;
// 	alloc->status = 0;
// 	s_alloc_t * left = ((unsigned int)alloc - sizeof(s_alloc_t) - alloc->prev_size);
// 	if (left->signature == 0xDEADBEEF&&left->status == 0&&left->size==alloc->prev_size)
// 	{
// 		//Merge blocks
// 		if (avltree_remove(&allocationAvlTree, left->size, (uint)left + sizeof(s_alloc_t))) {
// 			left->size += sizeof(s_alloc_t) + alloc->size;
// 			alloc = left;
// 		}
// 		else assert(0);
// 	}
// 	s_alloc_t * right = (uint)alloc + sizeof(s_alloc_t) + alloc->size;
// 	if (right->prev_size&&right->status == 0&&right->signature == 0xDEADBEEF)
// 	{
// 		if (avltree_remove(&allocationAvlTree, right->size, (uint)right + sizeof(s_alloc_t)))
// 			alloc->size += sizeof(s_alloc_t) + right->size;
// 		else assert(0);
// 	}
// 	avltree_insert(&allocationAvlTree, alloc->size, (uint)alloc + sizeof(s_alloc_t));

// }