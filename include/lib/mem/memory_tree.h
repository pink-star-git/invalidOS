#pragma once

#include "node_pool.h"

namespace detail
{

class MemoryTree final { // AvlReference;
#ifdef SIL_MEM_TEST
public:
    MemoryTree(size_t ptr) : m_pool(NodePool(ptr)), m_root(nullptr) {
    }

    MemoryTree(size_t ptr, size_t size) : m_pool(NodePool(ptr)), m_root(nullptr) {
        assign(ptr, CHUNK_SIZE); // start node
        assign(ptr + size, 0);   // end node
    }
    size_t size() {
        return m_size;
    };
    Node* root() {
        return m_root;
    }
    void assign_test(size_t key, size_t size) {
        assign(key, size);
    }

#endif


public:
    MemoryTree();

    Node* get(size_t key);     // get access to node
    void free(size_t key);     // free
    size_t alloc(size_t size); // allocate memory

    // serve memory pool
    void expand(size_t ptr) { // add new chunk to pool
        m_pool.expand(ptr);
    }
    bool isExpandRequired() { // check state
        return m_pool.isExpandRequired();
    }


private:
    void assign(size_t key, size_t size);

    int setBalance(Node* node);
    int getHeight(Node* node) const;
    void rebalance(Node* node);
    void rotate(Node* node);
    void rotateLLR(Node* parent, Node* node);
    void rotateLRR(Node* patent, Node* node, Node* child);
    void rotateRRR(Node* parent, Node* node);
    void rotateRLR(Node* patent, Node* node, Node* child);
    void removeNode(Node* node);
    Node* findNode(size_t key);

    class LNR {
        size_t m_counter = 0;
        size_t m_current = 0;
        Node* m_root = nullptr;
        Node* m_result = nullptr;

        Node* recursiveSearch(Node* node) {
            if (node->left)
                m_result = recursiveSearch(node->left);
            if (m_result)
                return m_result;
            if (m_counter == m_current)
                return node;
            m_counter++;
            if (node->right)
                m_result = recursiveSearch(node->right);
            return m_result;
        }

    public:
        LNR(Node* root) : m_root(root) {
        }
        Node* next() {
            m_result = nullptr;
            m_counter = 0;
            auto result = recursiveSearch(m_root);
            m_current++;

            return result;
        }
    };

    NodePool m_pool;
    Node* m_root = nullptr;
    int m_size = 0;
};

} // namespace detail