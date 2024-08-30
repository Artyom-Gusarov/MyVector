#include <gtest/gtest.h>

#ifdef TEST_MY
#include "MyVector.hpp"
template <typename T>
using vector = MyVector::vector<T>;
#endif

namespace {
class VectorTest : public testing::Test {
protected:
    vector<int> v;
};

TEST_F(VectorTest, push_back_pass_by_const_ref) {
    const int num = 12345;
    v.push_back(num);
    EXPECT_EQ(v[0], num);
}

TEST_F(VectorTest, push_back_pass_by_rvalue_ref) {
    const int num = 12345;
    int copy = num;
    v.push_back(std::move(copy));
    EXPECT_EQ(v[0], num);
}

TEST_F(VectorTest, push_back_multiple_elements) {
    const int a = 1;
    const int b = 2;
    const int c = 3;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    EXPECT_EQ(v[0], a);
    EXPECT_EQ(v[1], b);
    EXPECT_EQ(v[2], c);
    EXPECT_EQ(v.size(), 3);
}

TEST_F(VectorTest, pop_back) {
    const int a = 1;
    const int b = 2;
    const int c = 3;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    v.pop_back();
    EXPECT_EQ(v[0], a);
    EXPECT_EQ(v[1], b);
    EXPECT_EQ(v.size(), 2);
}

TEST_F(VectorTest, change_element) {
    const int a = 1;
    const int b = 2;
    const int c = 3;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    v[1] = 4;
    EXPECT_EQ(v[0], a);
    EXPECT_EQ(v[1], 4);
    EXPECT_EQ(v[2], c);
}

TEST_F(VectorTest, many_push_back) {
    const int n = 1000;
    for (int i = 0; i < n; ++i) {
        v.push_back(i);
    }
    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(v[i], i);
    }
    const int m = 10000;
    for (int i = n; i < m; ++i) {
        v.push_back(i);
    }
    for (int i = n; i < m; ++i) {
        EXPECT_EQ(v[i], i);
    }
}
}  // namespace

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}