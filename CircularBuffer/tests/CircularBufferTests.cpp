#include <algorithm>

#include "lib/CircularBuffer.h"
#include <gtest/gtest.h>

TEST(Buffer, sort) {
    CircularBuffer<std::string> buff = {"b", "asd", "c", "da"};

    sort(buff.begin(), buff.end());
    CircularBuffer<std::string> sorted_buff = {"asd", "b", "c", "da"};

    ASSERT_EQ(buff, sorted_buff);
}

TEST(Buffer, push_back) {
    CircularBuffer<int> buff;
    buff.reserve(3);
    buff.push_back(1);
    buff.push_back(2);
    buff.push_back(3);

    CircularBuffer<int> buff1;
    buff1 = {1, 2, 3};

    ASSERT_TRUE(buff == buff1);
}

TEST(Buffer, push_front_and_reserve) {
    CircularBuffer<int> buff;
    buff.reserve(3);
    buff.push_front(1);
    std::cout <<*buff.begin() << '\n';
    ASSERT_TRUE(*buff.begin() == 1);
    buff.push_front(2);
    ASSERT_TRUE(*buff.begin() == 2);
    buff.push_front(3);
    ASSERT_TRUE(*buff.begin() == 3);
}

TEST(Buffer, empty) {
    CircularBuffer<int> buff;
    ASSERT_TRUE(buff.empty() == true);
}


TEST(Buffer, pop_back) {
    CircularBuffer<int> buff(2, 1);
    buff.pop_back();
    ASSERT_TRUE(buff.size() == 1);
    buff.pop_back();
    ASSERT_TRUE(buff.empty() == true);
}

TEST(Buffer, pop_front) {
    CircularBuffer<int> buff(2, 1);
    buff.pop_front();
    ASSERT_TRUE(buff.size() == 1);
    buff.pop_front();
    ASSERT_TRUE(buff.empty() == true);
}

TEST(Buffer, BeginEnd) {
    CircularBuffer<int> buff(3);
    buff = {1, 2, 3};

    std::cout << *buff.begin() << " " << buff.back() << '\n';

    ASSERT_TRUE(buff.front() == 1);
    ASSERT_TRUE(buff.back() == 3);
}

TEST(Buffer, size) {
    CircularBuffer<int> buff;
    buff.reserve(3);
    ASSERT_TRUE(buff.size() == 0);
    buff.push_back(1);
    ASSERT_TRUE(buff.size() == 1);
    buff.push_front(2);
    ASSERT_TRUE(buff.size() == 2);
}

TEST(Buffer, capacity) {
    CircularBuffer<int> buff(3);
    ASSERT_TRUE(buff.capacity() == 3);
}

TEST(Buffer, clear) {
    CircularBuffer<int> buff(5);
    buff = {1, 2, 3, 4, 5};
    ASSERT_FALSE(buff.empty());
    buff.clear();
    ASSERT_TRUE(buff.empty());
}

TEST(Buffer, access_operator) {
    CircularBuffer<int> buff(5);
    buff = {1, 2, 3, 4, 5};
    ASSERT_TRUE(buff[3] == 4);
    ASSERT_TRUE(buff[1] == 2);
}

TEST(Buffer, insert) {
    CircularBuffer<int> buff;
    buff.reserve(5);
    buff = {1, 2};
    buff.insert(buff.begin() + 2, 5);
    ASSERT_TRUE(buff[2] == 5);
    buff.insert(buff.begin(), 2, 7);
    ASSERT_TRUE(buff[0] == 7 && buff[1] == 7);
}


TEST(Buffer, equal) {
    CircularBuffer<int> buff1;
    CircularBuffer<int> buff2;
    ASSERT_TRUE(buff1 == buff2);
    buff1 = {1, 2, 3, 4};
    buff2 = {1, 2, 3, 4};
    ASSERT_TRUE(buff1 == buff2);
    buff1.push_back(5);
    ASSERT_FALSE(buff1 == buff2);
}

TEST(Buffer, not_equal) {
    CircularBuffer<int> buff1(4);
    CircularBuffer<int> buff2(4);
    ASSERT_FALSE(buff1 != buff2);
    buff1 = {1, 2, 3, 4};
    buff2 = {1, 2, 3, 4};
    ASSERT_FALSE(buff1 != buff2);
    buff1.push_back(5);
    ASSERT_TRUE(buff1 != buff2);
}

TEST(Iterator, increment) {
    CircularBuffer<int> buff(5);
    buff = {1, 2, 3, 4, 5};
    CircularBuffer<int>::iterator it;
    it = buff.begin();
    ASSERT_TRUE(*it == 1);
    it++;
    ASSERT_TRUE(*it == 2);
}

TEST(Iterator, decrement) {
    CircularBuffer<int> buff(5);
    buff = {1, 2, 3, 4, 5};
    CircularBuffer<int>::iterator it;
    it = buff.end() - 1;
    ASSERT_TRUE(*it == 5);
    it--;
    ASSERT_TRUE(*it == 4);
}

TEST(Iterator, plus) {
    CircularBuffer<int> buff(5);
    buff = {1, 2, 3, 4, 5};
    CircularBuffer<int>::iterator it;
    it = buff.begin();
    ASSERT_TRUE(*it == 1);
    it += 3;
    ASSERT_TRUE(*it == 4);
}

TEST(Iterator, minus) {
    CircularBuffer<int> buff(5);
    buff = {1, 2, 3, 4, 5};
    CircularBuffer<int>::iterator it;
    it = buff.end() - 1;
    ASSERT_TRUE(*it == 5);
    it -= 2;
    ASSERT_TRUE(*it == 3);
}

TEST(Iterator, access_operator) {
    CircularBuffer<int> buff(5);
    buff = {1, 2, 3, 4, 5};
    CircularBuffer<int>::iterator it;
    it = buff.begin();
    ASSERT_TRUE(it[0] == 1);
    ASSERT_TRUE(it[3] == 4);
    it++;
    ASSERT_TRUE(it[3] = 5);
}

TEST(Iterator, equal) {
    CircularBuffer<int> buff(5);
    buff = {1, 2, 3, 4, 5};
    CircularBuffer<int>::iterator it1;
    it1 = buff.begin();
    CircularBuffer<int>::iterator it2;
    it2 = buff.begin();
    ASSERT_TRUE(it1 == it2);
    it1 += 2;
    it2 += 2;
    ASSERT_TRUE(it1 == it2);
    it1++;
    ASSERT_TRUE(it1 != it2);
}

TEST(Iterator, compare) {
    CircularBuffer<int> buff;
    buff = {1, 2, 3, 4, 5};
    CircularBuffer<int>::iterator it1;
    it1 = buff.begin();
    CircularBuffer<int>::iterator it2;
    it2 = buff.begin() + 1;
    ASSERT_LE(*it1, *it2);
    it1 += 2;
    ASSERT_GT(*it1, *it2);
}

TEST(BufferExt, expansion) {
    CCircularBufferExt<int> buff = {1, 2, 3};

    ASSERT_EQ(buff.size(), 3);
    buff.push_back(4);
    std::cout << buff.size() << '\n';
    ASSERT_EQ(buff.size(), 4);
}