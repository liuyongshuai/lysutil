/*************************************************************************
 * File:	test_gtest1.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-08-14 13:07
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

TEST(arithmetic, add_test) {
    int a = 2;
    int b = 3;
    int sum = add(a, b);
    EXPECT_EQ(a + b, sum);
}

TEST(arithmetic, sub_test) {
    int a = 2;
    int b = 3;
    int sum = sub(a, b);
    EXPECT_EQ(a - b, sum);
}


int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}