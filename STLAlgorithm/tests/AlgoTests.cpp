#include "lib/my_algorithms.h"
#include <vector>
#include <list>
#include <set>

#include <gtest/gtest.h>

using namespace algo;

TEST(algo, all_of) {

    auto predicate = [](int a) {return a == 1;};

    std::vector<int> a = {1, 1, 1, 1};
    std::vector<int> b = {1, 2 , 3, 4};

    ASSERT_TRUE(algo::all_of(a.begin(), a.end(), predicate));
    ASSERT_FALSE(algo::all_of(b.begin(), b.end(), predicate));
}

TEST(algo, any_of) {
    auto predicate = [](char a) {return a == 'a';};

    std::list b = {'b', 'b', 'b', 'b'};
    std::list a = {'b', 'a', 'b', 'b'};

    ASSERT_TRUE(algo::any_of(a.begin(), a.end(), predicate));
    ASSERT_FALSE(algo::any_of(b.begin(), b.end(), predicate));
}

TEST(algo, none_of) {
    auto predicate = [](int x) {return x == 3;};

    std::vector<int> a = {1, 3, 3, 3};
    std::set<double> b = {4, 4, 1, 1};

    ASSERT_FALSE(algo::none_of(a.begin(), a.end(), predicate));
    ASSERT_TRUE(algo::none_of(b.begin(), b.end(), predicate));
}

TEST(algo, one_of) {
    auto predicate = [](char c) {return c == 'x';};

    std::string s1 = "absx";
    std::string s2 = "xxxx";
    std::string s3 = "aaaa";

    ASSERT_TRUE(algo::one_of(s1.begin(), s1.end(), predicate));
    ASSERT_FALSE(algo::one_of(s2.begin(), s2.end(), predicate));
    ASSERT_FALSE(algo::one_of(s3.begin(), s3.end(), predicate));
}

TEST(algo, is_sorted) {
    auto predicate = [](int a, int b) {
        return (a < b);
    };

    std::vector<int> vec = {1, 3 , 4, 42, 1};
    std::set<int> st = {1,1,3,4,5,6};

    ASSERT_TRUE(algo::is_sorted(st.begin(), st.end(), predicate));
    ASSERT_FALSE(algo::is_sorted(vec.begin(), vec.end(), predicate));
}

TEST(algo, is_partitioned) {
    auto predicate = [] (int a) {
        return (a%2);
    };

    std::vector<int> v= {1, 3, 3, 4, 6, 6, 8};
    std::list<int> l = {1, 1, 2, 3, 2};

    ASSERT_TRUE(algo::is_partitioned(v.begin(), v.end(), predicate));
    ASSERT_FALSE(algo::is_partitioned(l.begin(), l.end(), predicate));
}

TEST(algo, find_not) {
    int target = 10;

    std::vector<int> v1 = {10, 10, 10};
    std::vector<int> v2 = {1, 23, 10, 31};

    ASSERT_EQ(algo::find_not(v1.begin(), v1.end(), target), v1.end());
    ASSERT_EQ(algo::find_not(v2.begin(), v2.end(), target), v2.begin());
}

TEST(algo, find_backward) {
    char target = 'x';

    std::string s1 = "gxgxfgx";
    std::string s2 = "aaaaaa";

    ASSERT_EQ(algo::find_backward(s1.begin(), s1.end(), target), s1.end() - 1);
    ASSERT_EQ(algo::find_backward(s2.begin(), s2.end(), target), s2.end());
}

TEST(algo, is_palindrome) {
    std::string s = "abacaba";
    std::vector<int> v = {1,3,3,3};

    ASSERT_TRUE(algo::is_palindrome(s.begin(), s.end()));
    ASSERT_FALSE(algo::is_palindrome(v.begin(), v.end()));
}

TEST(algo,  xrange) {
    auto x = algo::xrange(1.5, 5.5);

    std::vector<int> v1 = {x.begin(), x.end()};
    std::vector<int> v2 = {1, 2, 3, 4};

    auto xx = algo::xrange(4);
    std::vector<int> vv1{xx.begin(), xx.end()};
    std::vector<int> vv2{0, 1, 2, 3};

    std::vector<int> vvv1;
    std::vector<int> vvv2{1, 3, 5};
    for (auto i : algo::xrange(1, 6, 2)) {
        vvv1.push_back(i);
    }

    ASSERT_EQ(v1, v2);
    ASSERT_EQ(vv1, vv2);
    ASSERT_EQ(vvv1, vvv2);

}

TEST(algo, zip) {
    std::list<int> l = {1, 2, 3, 4, 5};
    std::vector<char> v = {'a', 'b', 'c', 'd'};

    auto z = zip(l, v);

    std::vector<std::pair<int, char>> v1{z.begin(), z.end()};
    std::vector<std::pair<int, char>> v2 = {{1, 'a'},
                                            {2, 'b'},
                                            {3, 'c'},
                                            {4, 'd'}};

    ASSERT_EQ(v1, v2);
    }