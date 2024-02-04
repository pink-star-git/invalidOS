// include/lib/mem/mem.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "lib/asm.h"

#include "mem.h"

#define MEM_BLOCK_SIGN      0x1AFDCFA1

#define MEM_BLOCK_USAGE     0x01
#define MEM_BLOCK_USE       0x01
#define MEM_BLOCK_FREE      0x00

namespace memory {
    struct block_header {
        u_int32 sign_;
        u_int32 size_prev_;
        u_int32 status_;
        u_int32 size_;

        void set_use () {
            status_ |= MEM_BLOCK_USE;
        };
        bool is_use () {
            return ((status_ & MEM_BLOCK_USAGE) == MEM_BLOCK_USE);
        };

        void set_free () {
            status_ &= (~MEM_BLOCK_USAGE | MEM_BLOCK_FREE);
        };
        bool is_free () {
            return ((status_ & MEM_BLOCK_USAGE) == MEM_BLOCK_FREE);
        }
    }__attribute__((packed));

    static size_t block_size;// must be 128
    static u_char8 *alloc;
    static bool full_usage;

    void map_init () {
        block_size = sizeof(memory::block_header)*8;
        alloc = (0x100000 + block_size);
        full_usage = false;

        memory::block_header *block = (block_header *)(alloc - block_size);
        block->sign_ = MEM_BLOCK_SIGN;
        block->size_ = 0x200000 - block_size;
        block->size_prev_ = 0;
        block->set_free();

    }
} // namespace memory


namespace sil { // standart invalid library
    namespace mem {
        void init () {
            memory::map_init();
        }

        u_char8 *malloc (size_t size) {
            size += memory::block_size - size % memory::block_size;
            memory::block_header *block = (memory::block_header *)(memory::alloc - memory::block_size);
            
            while (true) {
                if (block->sign_ != MEM_BLOCK_SIGN) return NULL;
                if ((block->is_free()) && (block->size_ > size)) {
                    if (block->size_ > 256 - size) {
                        memory::block_header *new_block = block + memory::block_size + size;
                        new_block->sign_ = MEM_BLOCK_SIGN;
                        new_block->size_ = block->size_ - size - memory::block_size;
                        new_block->size_prev_ = size;
                        block->size_ = size;
                    }
                    block->set_use();
                    return (u_char8 *)(block + memory::block_size);
                }
                else {
                    block += block->size_ + memory::block_size;
                }
            }
        }

        void free(u_char8 *mem_ptr) {
            memory::block_header *block = (memory::block_header *)(mem_ptr - memory::block_size);
            if (block->sign_ != MEM_BLOCK_SIGN) return;
            memset(mem_ptr, block->size_);
            block->set_free();
            
            memory::block_header *prev_block = (memory::block_header *)((u_char8 *)block - block->size_prev_);
            memory::block_header *next_block = (memory::block_header *)(mem_ptr          + block->size_);
            if ((prev_block->sign_ == MEM_BLOCK_SIGN) && (prev_block->is_free())) {
                prev_block->size_ += memory::block_size + block->size_;
                block->sign_ = 0;
            }
            if ((next_block->sign_ == MEM_BLOCK_SIGN) && (next_block->is_free())) {
                block->size_ += memory::block_size + next_block->size_;
                next_block->sign_ = 0;
            }
        }
    } // namespace mem

    u_int32 count_memory(void)
    {
        register u_int32 *mem;
        u_int32 mem_count = 0,
                a,
                mem_end,
                bse_end,
                cr0;
        u_short16 mem_mb = 0;
        u_char8 irq1,
                irq2;

        /* save IRQ's */
        irq1 = asm_::inb(0x21);
        irq2 = asm_::inb(0xA1);

        /* kill all irq's */
        asm_::outb(0x21, 0xFF);
        asm_::outb(0xA1, 0xFF);

        // store a copy of CR0
        asm volatile(
            "movl %%cr0, %%eax"
            : "=a"(cr0)
            :
            :
        );

        // invalidate the cache
        // write-back and invalidate the cache
        asm volatile ("wbinvd");

        // plug cr0 with just PE/CD/NW
        // cache disable(486+), no-writeback(486+), 32bit mode(386+)
        asm volatile(
            "movl %%eax, %%cr0"
            :
            : "a" (cr0 | 0x00000001 | 0x40000000 | 0x20000000)
            :
        );
    
        do {
            mem_mb++;
            mem_count += 1024*1024; // += 1MB
            mem = (u_int32 *) mem_count;
    
            a = *mem;
            *mem = 0x55AA55AA;
    
            // the empty asm calls tell gcc not to rely on what's in its registers
            // as saved variables (this avoids GCC optimisations)
            asm("":::"memory");
            if (*mem != 0x55AA55AA)
                mem_count=0;
            else {
                *mem = 0xAA55AA55;
                asm("":::"memory");
                if (*mem != 0xAA55AA55)
                    mem_count=0;
            }

            asm("":::"memory");
            *mem=a;
    
        } while (mem_mb<4096 && mem_count!=0);
    
        asm volatile(
            "movl %%eax, %%cr0"
            :
            : "a" (cr0)
            :
        );
    
        // mem_end = mem_mb << 20;
        // mem = (u_int32 *)0x413;
        // bse_end = (*mem & 0xFFFF) << 6;
    
        asm_::outb(0x21, irq1);
        asm_::outb(0xA1, irq2);

        return mem_mb;
    }
} // namespace sil


void *operator new(size_t size)
{
    return sil::mem::malloc(size);
}

void *operator new[](size_t size)
{
    return sil::mem::malloc(size);
}

void operator delete(void *p)
{
    sil::mem::free(p);
}

void operator delete[](void *p)
{
    sil::mem::free(p);
}