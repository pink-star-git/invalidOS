#pragma once

#include <gtest/gtest.h>

#include "memory.h"
#include "memory_tree.h"

namespace detail
{

class MemoryFixture : public ::testing::Test {
public:
    MemoryFixture() {
        size = CHUNK_SIZE * 5;
    }

    void SetUp() override {
        addr = malloc(size);
        tree = new MemoryTree((size_t)addr, size);
    }

    void TearDown() override {
        delete tree;
        free(addr);
    }

    ~MemoryFixture() override {
    }

    void* addr = nullptr;
    size_t size;

private:
};

TEST_F(MemoryFixture, malloc) {
    {
        auto x = sil::malloc(123);
        auto node = tree->get((size_t)x);
        ASSERT_EQ(x, (void*)((size_t)addr + CHUNK_SIZE));

        ASSERT_EQ((void*)node->key, x);
        ASSERT_EQ(node->size, 123);
    }

    {
        auto x = sil::malloc(456);
        auto node = tree->get((size_t)x);
        ASSERT_EQ(x, (void*)((size_t)addr + CHUNK_SIZE + 123));

        ASSERT_EQ((void*)node->key, x);
        ASSERT_EQ(node->size, 456);
    }
}


TEST_F(MemoryFixture, realloc) {

    auto a1 = sil::malloc(0x100);
    auto a2 = sil::malloc(0x200);
    auto a3 = sil::malloc(0x100);

    sil::free(a2);
    auto a2n = sil::malloc(0x200);

    ASSERT_EQ(a2n, a2);
    ASSERT_EQ(a1, (void*)((size_t)addr + CHUNK_SIZE));
    ASSERT_EQ(a2n, (void*)((size_t)addr + CHUNK_SIZE + 0x100));
    ASSERT_EQ(a3, (void*)((size_t)addr + CHUNK_SIZE + 0x100 + 0x200));
}

TEST_F(MemoryFixture, free) {
    ASSERT_EQ(tree->size(), 2);

    auto x = sil::malloc(123);
    ASSERT_EQ(tree->size(), 3);

    sil::free(x);
    ASSERT_EQ(tree->size(), 2);


    auto node = tree->get((size_t)x);
    ASSERT_EQ(node, nullptr);
}

TEST_F(MemoryFixture, expandMemoryPool) {
    // allocate more than POOL_SIZE
    for (size_t i = 0; i <= POOL_SIZE; ++i) {
        auto _ = sil::malloc(1);
    }
}

} // namespace detail