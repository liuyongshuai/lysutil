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
#include <leveldb/db.h>
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

    std::string input{
            "123456789asdfghhjsssssssssssssssssssssssssssssssssswwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"}, output;
    snappy::Compress(input.data(), input.size(), &output);
    std::cout << "original size: " << input.size() << " Compressed size: " << output.size() << std::endl;
    std::string original;
    snappy::Uncompress(output.data(), output.size(), &original);
    std::cout << "original str: " << original << std::endl;

    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());
    //write key1,value1
    std::string key = "key";
    std::string value = "value";
    status = db->Put(leveldb::WriteOptions(), key, value);
    assert(status.ok());
    status = db->Put(leveldb::WriteOptions(), key, "hello");
    assert(status.ok());
    status = db->Get(leveldb::ReadOptions(), key, &value);
    assert(status.ok());
    std::cout << value << std::endl;
    std::string key2 = "key2";
    //move the value under key to key2
    status = db->Put(leveldb::WriteOptions(), key2, value);
    assert(status.ok());
    status = db->Delete(leveldb::WriteOptions(), key);
    assert(status.ok());
    status = db->Get(leveldb::ReadOptions(), key2, &value);
    assert(status.ok());
    std::cout << key2 << "===" << value << std::endl;
    status = db->Get(leveldb::ReadOptions(), key, &value);
    if (!status.ok()) std::cerr << key << "  " << status.ToString() << std::endl;
    else std::cout << key << "===" << value << std::endl;
    delete db;

    return RUN_ALL_TESTS();
}