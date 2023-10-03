#pragma once

#include <gtest/gtest.h>

#include "node_pool.h"

namespace detail
{

class AvlNodePoolFixture : public ::testing::Test {
public:
    AvlNodePoolFixture() {
        m_size = detail::CHUNK_SIZE;
    }

    void SetUp() override {
        addr = malloc(m_size * 2);
        np = new NodePool((size_t)addr);
    }

    void TearDown() override {
        delete np;
        free(addr);
    }

    ~AvlNodePoolFixture() override {
    }

    void* addr = nullptr;
    size_t count = POOL_SIZE;
    NodePool* np;

private:
    size_t m_size;
};

TEST_F(AvlNodePoolFixture, create) {
    auto node_1 = np->get();
    ASSERT_EQ((Node*)addr, node_1) << "first node should have the first addr of memory";

    auto node_2 = np->get();
    ASSERT_EQ((Node*)addr + 1, node_2) << "second node should have start the second addr of memory";

    auto node_3 = np->get();
    ASSERT_EQ((Node*)addr + 2, node_3) << "second node should have start the second addr of memory";
}

TEST_F(AvlNodePoolFixture, remove) {
    auto node_1 = np->get();
    auto node_2 = np->get();

    np->free(node_1);
    auto node_3 = np->get();

    ASSERT_EQ((Node*)addr, node_3) << "third node should have the first  addr of memory";
}

TEST_F(AvlNodePoolFixture, out_of_memory) {
    for (size_t i = 0; i < count; ++i) {
        np->get();
    }

    ASSERT_EQ(np->get(), nullptr);
}


} // namespace detail