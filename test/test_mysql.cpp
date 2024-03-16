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
        std::cout << "connect mysql failed:" << mysql_error(mysql_) << std::endl;
        return 1;
    }

    //开始查询
    std::string sql = "show tables";
    if (mysql_real_query(mysql_, sql.c_str(), sql.size()) != 0) {
        std::cout << "mysql_real_query failed:" << mysql_error(mysql_) << std::endl;
        return 1;
    }
    //得到结果集
    MYSQL_RES *result_ = mysql_store_result(mysql_);
    my_ulonglong field_num = mysql_field_count(mysql_);
    my_ulonglong num_rows = mysql_num_rows(result_);
    MYSQL_FIELD *field_list = mysql_fetch_fields(result_);
    std::cout << "row_count=" << num_rows << "\tfield_count=" << field_num << std::endl;

    //打印字段
    for (uint64_t i = 0; i < field_num; i++) {
        std::cout << "field" << i << ":" << field_list[i].name << std::endl;
    }
    //打印值
    MYSQL_ROW row;
    for (uint64_t j = 0; j < num_rows; j++) {
        row = mysql_fetch_row(result_);
        for (uint64_t i = 0; i < field_num; i++) {
            std::cout << field_list[i].name << "=" << row[i] << std::endl;
        }
    }
    mysql_free_result(result_);
    return 0;
}