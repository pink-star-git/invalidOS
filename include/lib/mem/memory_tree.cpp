#include <cassert>

#include "memory_tree.h"


namespace detail
{

int MemoryTree::setBalance(Node* node) {
    if (!node)
        return 0; // node is null?!
    int left_height = getHeight(node->left);
    int right_height = getHeight(node->right);
    node->balance = right_height - left_height;
    return node->balance;
}

int MemoryTree::getHeight(Node* node) const {
    if (!node)
        return -1;

    auto max = getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right);
    return 1 + max;
}

void MemoryTree::rebalance(Node* node) {
    while (node) {
        setBalance(node);
        if (node->balance >= 2 || node->balance <= -2)
            rotate(node);
        node = node->parent;
    }
}

void MemoryTree::rotate(Node* node) {
    Node* child;
    if (node->balance < -1) {
        child = node->left;
        setBalance(child);
        if (child->balance == 1)
            rotateLRR(node, child, child->right);
        else
            rotateLLR(node, child);
    }
    else {
        child = node->right;
        setBalance(child);
        if (child->balance == -1)
            rotateRLR(node, child, child->left);
        else
            rotateRRR(node, child);
    }
}

void MemoryTree::rotateLLR(Node* parent, Node* node) {
    Node* grand_parent = parent->parent;
    node->parent = grand_parent;
    Node* node_right = node->right;
    if (node_right)
        node_right->parent = parent;
    node->right = parent;
    parent->parent = node;
    parent->left = node_right;
    if (!grand_parent)
        m_root = node;
    else if (grand_parent->left == parent)
        grand_parent->left = node;
    else
        grand_parent->right = node;
}

void MemoryTree::rotateRRR(Node* parent, Node* node) {
    Node* grand_parent = parent->parent;
    node->parent = grand_parent;
    Node* node_left = node->left;
    if (node_left)
        node_left->parent = parent;
    node->left = parent;
    parent->parent = node;
    parent->right = node_left;
    if (!grand_parent)
        m_root = node;
    else if (grand_parent->left == parent)
        grand_parent->left = node;
    else
        grand_parent->right = node;
}

void MemoryTree::rotateLRR(Node* parent, Node* node, Node* child) {
    rotateRRR(node, child);
    rotateLLR(parent, child);
}

void MemoryTree::rotateRLR(Node* parent, Node* node, Node* child) {
    rotateLLR(node, child);
    rotateRRR(parent, child);
}

void MemoryTree::removeNode(Node* node) {
    if (node == nullptr)
        return;
    if (m_root == nullptr)
        return;

    Node* parent = node->parent;
    if (node->left == nullptr && node->right == nullptr) {
        if (parent == nullptr)
            m_root = nullptr;
        else if (parent->left == node)
            parent->left = nullptr;
        else
            parent->right = nullptr;
        m_pool.free(node);
        rebalance(parent);
        m_size--;
    }
    else if (node->right == nullptr) {
        if (parent == nullptr) {
            m_root = node->left;
            m_root->parent = nullptr;
        }
        else if (parent->left == node) {
            parent->left = node->left;
            node->left->parent = parent;
        }
        else {
            parent->right = node->left;
            node->left->parent = parent;
        }
        m_pool.free(node);
        rebalance(parent);
        m_size--;
    }
    else if (node->left == nullptr) {
        if (parent == nullptr) {
            m_root = node->right;
            m_root->parent = nullptr;
        }
        else if (parent->left == node) {
            parent->left = node->right;
            node->right->parent = parent;
        }
        else {
            parent->right = node->right;
            node->right->parent = parent;
        }
        m_pool.free(node);
        rebalance(parent);
        m_size--;
    }
    else {
        Node* temp = node->right;
        while (temp->left != nullptr)
            temp = temp->left;
        node->key = temp->key;
        node->size = temp->size;
        removeNode(temp);
    }
}

Node* MemoryTree::findNode(size_t key) {
    Node* temp = m_root;
    while (temp) {
        if (temp->key == key)
            return temp;
        temp = temp->key < key ? temp->right : temp->left;
    }
    return nullptr;
}

MemoryTree::MemoryTree() : m_pool(NodePool(START_ADDR)), m_root(nullptr) {
    assign(START_ADDR, CHUNK_SIZE); // start node
    assign(0x01FFFFFF, 0);          // end node //TODO: get max address
}


Node* MemoryTree::get(size_t key) {
    return findNode(key);
};

void MemoryTree::free(size_t key) {
    removeNode(findNode((size_t)key));
};

size_t MemoryTree::alloc(size_t size) {
    size_t check_size = 1; // for assertion check. remove later
    size_t current = 0;

    // LNR algrorithm to search node (prev<next)
    auto next = [&] {
        Node* result = nullptr;
        size_t counter = 0;

        auto recursive = [&](const auto& f, Node* node) {
            if (!node || result)
                return;
            f(f, node->left);
            if (counter++ == current) {
                result = node;
                return;
            }
            f(f, node->right);
        };
        recursive(recursive, m_root);
        current++;
        return result;
    };

    Node* first = next();
    while (Node* second = next()) {
        assert(first->key + first->size <= second->key);
        check_size++;
        if (second->key - (first->key + first->size) >= size) {
            auto key = first->key + first->size;
            assign(key, size);
            return key;
        }
        first = second;
    }
    assert(check_size == m_size);
    return 0;
}

void MemoryTree::assign(size_t key, size_t size) {
    if (m_root == nullptr) {
        m_size++;
        m_root = m_pool.get();
        m_root->key = key;
        m_root->size = size;
        return;
    }
    Node* curr = m_root;
    Node* prev = curr;
    while (curr != nullptr) {
        prev = curr;
        if (key < curr->key)
            curr = curr->left;
        else if (key > curr->key)
            curr = curr->right;
        else
            return;
    }
    Node* new_node = m_pool.get();
    if (!new_node)
        return;

    m_size++;
    new_node->parent = prev;
    new_node->key = key;
    new_node->size = size;
    if (key < prev->key)
        prev->left = new_node;
    else
        prev->right = new_node;

    rebalance(new_node);
}


} // namespace detail