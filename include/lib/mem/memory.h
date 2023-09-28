#pragma once

#include "memory_tree.h"

namespace detail
{
#ifdef SIL_MEM_TEST
MemoryTree* tree;
#else
MemoryTree tree{};
#endif

} // namespace detail

namespace sil
{
[[nodiscard("You have to assign ptr")]] void* malloc(size_t size) {
#ifdef SIL_MEM_TEST
    auto& mem = *detail::tree;
#else
    auto& mem = detail::tree;
#endif

    auto ptr = mem.alloc(size);

    if (mem.isExpandRequired()) {
        auto new_chunk = mem.alloc(detail::CHUNK_SIZE);
        if (new_chunk)
            mem.expand(new_chunk);
    }

    return (void*)ptr;
}

void free(void* ptr) {
#ifdef SIL_MEM_TEST
    auto& mem = *detail::tree;
#else
    auto& mem = detail::tree;
#endif

    mem.free((size_t)ptr);
}
} // namespace sil
