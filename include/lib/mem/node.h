#pragma once

namespace detail
{

struct Node final {
    Node() = delete;
    Node(const Node&) = delete;
    Node(Node&&) = delete;
    Node operator=(const Node&) = delete;
    Node operator=(Node&&) = delete;

    Node* left = nullptr;
    Node* right = nullptr;
    union {
        Node* parent = nullptr;
        Node* next;
        size_t next_chunk;
    };

    size_t key = 0;
    size_t size = 0;
    int balance = 0;

private:
    friend class NodePool;
    Node* reset(Node* n = nullptr) {
        left = nullptr;
        right = nullptr;
        next = n;
        key = 0;
        size = 0;
        balance = 0;
        return this;
    }
};


} // namespace detail