#pragma once

#include "node.h"

namespace detail
{

const size_t POOL_SIZE = 0x1000;
const size_t CHUNK_SIZE = POOL_SIZE * sizeof(Node);
const size_t START_ADDR = 0x01000000;

struct NodePool final {
    NodePool(size_t);
    Node* get();
    void free(Node* node);

    void expand(size_t);
    bool isExpandRequired() {
        return !m_next_free->next->next_chunk;
    }

private:
    void initChunk(size_t start);
    union {
        Node* m_next_free = nullptr;
        size_t m_next_void;
    };
};

} // namespace detail