#include <lib/CCircularBuffer.h>
#include <gtest/gtest.h>

TEST(CCircularBufferTestSuite, EmptyTest) {
    CCircularBuffer<int> test_buffer(5);
    ASSERT_TRUE(test_buffer.empty());
}

TEST(CCircularBufferTestSuite, FullTest) {
    CCircularBuffer<int> test_buffer(5);
    for (int i = 0; i < 5; ++i) test_buffer.push_back(i);
    ASSERT_TRUE(test_buffer.full());
    ASSERT_FALSE(test_buffer.empty());
}

TEST(CCircularBufferTestSuite, CapacityTest) {
    CCircularBuffer<int> test_buffer(5);
    ASSERT_EQ(test_buffer.capacity(), 5);
}

TEST(CCircularBufferTestSuite, SizeTest) {
    CCircularBuffer<int> test_buffer(5);

    test_buffer.push_back(144);
    test_buffer.push_back(239);
    test_buffer.push_back(420);
    test_buffer.push_back(1337);

    ASSERT_EQ(test_buffer.size(), 4);
}

TEST(CCircularBufferTestSuite, PeekTest) {
    CCircularBuffer<int> test_buffer(5);

    test_buffer.push_back(144);
    test_buffer.push_back(239);
    test_buffer.push_back(420);
    test_buffer.push_back(1337);

    ASSERT_EQ(test_buffer.front(), 144);
    ASSERT_EQ(test_buffer.back(), 1337);
}

TEST(CCircularBufferTestSuite, DeletionTest) {
    CCircularBuffer<int> test_buffer(5);

    test_buffer.push_back(144);
    test_buffer.push_back(239);
    test_buffer.push_back(420);
    test_buffer.push_back(1337);

    test_buffer.pop_front();
    test_buffer.pop_front();

    ASSERT_EQ(test_buffer.front(), 420);
}

TEST(CCircularBufferTestSuite, ClearTest) {
    CCircularBuffer<int> test_buffer(5);

    test_buffer.push_back(144);
    test_buffer.push_back(239);
    test_buffer.push_back(420);
    test_buffer.push_back(1337);

    test_buffer.clear();

    ASSERT_EQ(test_buffer.size(), 0);
}

TEST(CCircularBufferTestSuite, IteratorTest) {
    CCircularBuffer<int> test_buffer(5);

    test_buffer.push_back(144);
    test_buffer.push_back(239);
    test_buffer.push_back(420);
    test_buffer.push_back(1337);

    CCircularBuffer<int>::iterator ptr = test_buffer.begin();
    ASSERT_EQ(*ptr, 144);
    ptr++;
    ASSERT_EQ(*ptr, 239);
    ++ptr;
    ASSERT_EQ(*ptr, 420);
    ptr += 1;
    ASSERT_EQ(*ptr, 1337);
    ptr--;
    ASSERT_EQ(*ptr, 420);
    --ptr;
    ASSERT_EQ(*ptr, 239);
    ptr -= 1;
    ASSERT_EQ(*ptr, 144);
}
