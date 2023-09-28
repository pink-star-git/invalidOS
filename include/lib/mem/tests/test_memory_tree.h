#pragma once

#include <gtest/gtest.h>

#include "memory_tree.h"

namespace detail
{

class AvlTreeFixture : public ::testing::Test {
public:
    AvlTreeFixture() {
        m_size = CHUNK_SIZE;
    }

    void SetUp() override {
        addr = malloc(m_size);
        tree = new MemoryTree((size_t)addr);
        tree->assign_test(100, 93);
        tree->assign_test(600, 10);
        tree->assign_test(300, 3);
        tree->assign_test(700, 42);
        tree->assign_test(200, 42);
        tree->assign_test(400, 40);
        tree->assign_test(900, 42);
    }

    void TearDown() override {
        delete tree;
        free(addr);
    }

    ~AvlTreeFixture() override {
    }

    void* addr = nullptr;
    MemoryTree* tree;

private:
    size_t m_size;
    size_t m_count = POOL_SIZE;
};

TEST_F(AvlTreeFixture, create) {

    while (tree->root()) {
        printf("%zu\n", tree->root()->key);
        tree->free(tree->root()->key);
    }

    ASSERT_EQ(tree->size(), 0);

    tree->assign_test(1, 93);
    tree->assign_test(2, 10);
    tree->assign_test(3, 3);
    tree->assign_test(4, 42);
    tree->assign_test(5, 42);
    tree->assign_test(6, 40);
    tree->assign_test(7, 42);
    tree->assign_test(8, 8);
    tree->assign_test(9, 23);
    tree->assign_test(10, 235);

    ASSERT_EQ(tree->root()->key, 4);
    ASSERT_EQ(tree->root()->left->key, 2);
    ASSERT_EQ(tree->root()->left->left->key, 1);
    ASSERT_EQ(tree->root()->left->right->key, 3);
    ASSERT_EQ(tree->root()->right->key, 8);
    ASSERT_EQ(tree->root()->right->left->key, 6);
    ASSERT_EQ(tree->root()->right->left->left->key, 5);
    ASSERT_EQ(tree->root()->right->left->right->key, 7);
    ASSERT_EQ(tree->root()->right->right->key, 9);
    ASSERT_EQ(tree->root()->right->right->right->key, 10);
}

TEST_F(AvlTreeFixture, remove) {
    ASSERT_EQ(tree->size(), 7);

    tree->free(600);
    ASSERT_EQ(tree->size(), 6);

    auto root = tree->get(700);
    ASSERT_EQ(root->key, 700);
    ASSERT_EQ(root->size, 42);
    ASSERT_EQ(root->parent, tree->get(300));
    ASSERT_EQ(root->left, tree->get(400));
    ASSERT_EQ(root->right, tree->get(900));

    tree->free(700);
    ASSERT_EQ(tree->size(), 5);
}

} // namespace detail