#include <gtest/gtest.h>

#include "Set.hpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Set, Insert) {
    Set<int> set;

    set.Insert(0);
    set.Insert(2);
    set.Insert(4);
    set.Insert(1);
    set.Insert(3);

    size_t counter = 0;

    for (auto iter = set.Begin(); iter != set.End(); ++iter) {
        EXPECT_TRUE(counter == *iter);
        ++counter;
    }
}

TEST(Set, Erase) {
    Set<int> set;

    set.Insert(0);
    set.Insert(2);
    set.Insert(4);
    set.Insert(1);
    set.Insert(3);

    set.Erase(4);
    set.Erase(3);

    size_t counter = 0;

    for (auto iter = set.Begin(); iter != set.End(); ++iter) {
        EXPECT_TRUE(counter == *iter);
        ++counter;
    }
}

TEST(Set, IncrimentIterators) {
    Set<int> set;

    set.Insert(0);
    set.Insert(2);
    set.Insert(4);
    set.Insert(1);
    set.Insert(3);
    set.Insert(6);
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);
    set.Insert(5);

    size_t counter = 0;

    for (auto iter = set.Begin(); iter != set.End(); ++iter) {
        EXPECT_TRUE(counter == *iter);
        ++counter;
    }
}

TEST(Set, DecremntIterators) {
    Set<int> set;

    set.Insert(0);
    set.Insert(2);
    set.Insert(4);
    set.Insert(1);
    set.Insert(3);
    set.Insert(6);
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);
    set.Insert(5);

    size_t counter = 0;

    for (auto iter = set.Rend(); iter != set.Rbegin(); --iter) {
        EXPECT_TRUE(9 - counter == *iter);
        ++counter;
    }
}

TEST(Set, Size) {
    Set<int> set;

    set.Insert(0);
    set.Insert(2);
    set.Insert(4);
    set.Insert(1);
    set.Insert(3);
    set.Insert(6);
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);
    set.Insert(5);

    int counter = 10;

    for (int i = 0; i < counter; ++i) {
        set.Erase(i);
        EXPECT_TRUE(set.Size() == counter - i - 1);
    }
}

TEST(Set, EmptyAndClear) {
    Set<int> set;

    set.Insert(0);
    set.Insert(2);
    set.Insert(4);
    set.Insert(1);
    set.Insert(3);
    set.Insert(6);
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);
    set.Insert(5);


    EXPECT_TRUE(set.Empty() == false);

    set.Clear();

    EXPECT_TRUE(set.Empty() == true);
}


TEST(Set, Constructors) {
    Set<int> set;

    set.Insert(0);
    set.Insert(2);
    set.Insert(4);
    set.Insert(1);
    set.Insert(3);
    set.Insert(6);
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);
    set.Insert(5);

    EXPECT_TRUE(set.Size() == 10);

    Set<int> set_;

    EXPECT_TRUE(set.Size() == 10);
    EXPECT_TRUE(set_.Size() == 0);
}

TEST(Set, Find) {
    Set<int> set;

    set.Insert(0);
    set.Insert(1);
    set.Insert(2);
    set.Insert(3);
    set.Insert(5);
    set.Insert(4);
    set.Insert(6);
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);

    EXPECT_TRUE(*set.Find(5) == 5);
    EXPECT_TRUE(*set.Find(0) == 0);
    EXPECT_TRUE(set.Find(99) == Set<int>::Iterator(nullptr));
}

TEST(Set, FindAndConstruct) {
    Set<int> set;

    set.Insert(0);
    set.Insert(1);
    set.Insert(2);
    set.Insert(3);
    set.Insert(5);
    set.Insert(4);
    set.Insert(6);
    set.Insert(7);
    set.Insert(8);
    set.Insert(9);

    Set<int> set1(set.Find(0), set.Find(5));

    EXPECT_TRUE(set1.Size() == 5);
}