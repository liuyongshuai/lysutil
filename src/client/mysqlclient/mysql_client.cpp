/*************************************************************************
 * File:	mysql_client.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2019年03月23日 星期六 18时08分24秒
 ************************************************************************/

#include<iostream>
#include "client/mysqlclient/mysql_client.h"

namespace lysutil {
    namespace client {
        //连接
        bool MySQLClient::conn() {
            this->mysql_ = mysql_init(nullptr);
            //设置字符集
            mysql_set_character_set(this->mysql_, this->conf_.charset.c_str());
            mysql_options(this->mysql_, MYSQL_SET_CHARSET_NAME, this->conf_.charset.c_str());
            //自动重连
            my_bool is_reconnect = true;
            mysql_options(this->mysql_, MYSQL_OPT_RECONNECT, &is_reconnect);
            //连接上后初始命令
            //mysql_options(this->mysql_, MYSQL_INIT_COMMAND, "");
            //连接超时
            mysql_options(this->mysql_, MYSQL_OPT_CONNECT_TIMEOUT, &(this->conf_.conn_timeout));
            mysql_options(this->mysql_, MYSQL_OPT_READ_TIMEOUT, &(this->conf_.read_timeout));
            mysql_options(this->mysql_, MYSQL_OPT_WRITE_TIMEOUT, &(this->conf_.write_timeout));
            MYSQL *p = mysql_real_connect(
                    this->mysql_,
                    this->conf_.host.c_str(),
                    this->conf_.user.c_str(),
                    this->conf_.password.c_str(),
                    this->conf_.dbName.c_str(),
                    this->conf_.port,
                    nullptr,
                    0
            );
            return p != nullptr;
        }

        //释放资源
        MySQLClient::~MySQLClient() {
            this->freeResult();
            if (this->mysql_ != nullptr) {
                mysql_close(this->mysql_);
            }
        }
    }
} //namespacecpputils
