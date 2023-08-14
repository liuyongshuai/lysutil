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
#include "snappy.h"

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

    std::string input{"123456789asdfghhjsssssssssssssssssssssssssssssssssswwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"}, output;
    snappy::Compress(input.data(), input.size(), &output);
    std::cout << "original size: " << input.size() << " Compressed size: " << output.size() << std::endl;
    std::string original;
    snappy::Uncompress(output.data(), output.size(), &original);
    std::cout << "original str: " << original << std::endl;


    return RUN_ALL_TESTS();
}