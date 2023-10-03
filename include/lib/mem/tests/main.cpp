#include <gtest/gtest.h>


#include "test_memory.h"
#include "test_memory_tree.h"
#include "test_node_pool.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}