/*************************************************************************
 * File:	test_mysql.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2024-03-16 16:27
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<iostream>
#include<string>
#include <mysql/mysql.h>

int main(int argc, char *argv[]) {
    MYSQL *mysql_ = mysql_init(nullptr);
    //设置字符集
    mysql_set_character_set(mysql_, "utf8");
    mysql_options(mysql_, MYSQL_SET_CHARSET_NAME, "utf8");
    //自动重连
    my_bool is_reconnect = true;
    mysql_options(mysql_, MYSQL_OPT_RECONNECT, &is_reconnect);
    //连接上后初始命令
    mysql_options(mysql_, MYSQL_INIT_COMMAND, "set autocommit=1");
    //连接超时
    uint32_t conn_timeout = 5;
    mysql_options(mysql_, MYSQL_OPT_CONNECT_TIMEOUT, &conn_timeout);
    uint32_t read_timeout = 5;
    mysql_options(mysql_, MYSQL_OPT_READ_TIMEOUT, &read_timeout);
    uint32_t write_timeout = 5;
    mysql_options(mysql_, MYSQL_OPT_WRITE_TIMEOUT, &write_timeout);
    MYSQL *p = mysql_real_connect(mysql_, "192.168.1.5", "liuyongshuai", "123456", "db_data", 3306, nullptr, 0);
    if (p == nullptr) {
        std::cout << "connect mysql failed" << std::endl;
        return 1;
    }
    return 0;
}