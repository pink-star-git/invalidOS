#include "node_pool.h"

namespace detail
{

Node* NodePool::get() {
    auto cur = m_next_free;
    if (cur) {
        m_next_free = m_next_free->next;
        return cur->reset();
    }
    return cur;
}

NodePool::NodePool(size_t ptr) : m_next_void(ptr) {
    initChunk(m_next_void);
}

void NodePool::initChunk(size_t ptr) {
    size_t i;
    Node* mem = (Node*)ptr;
    for (i = 0; i < POOL_SIZE - 1; ++i) {
        mem[i].reset(&mem[i + 1]);
    }
    mem[i].reset(nullptr);
}

void NodePool::free(Node* node) {
    node->reset(m_next_free);
    m_next_free = node;
}

void NodePool::expand(size_t ptr) {
    initChunk(ptr);
    m_next_free->next_chunk = ptr;
}
} // namespace detail