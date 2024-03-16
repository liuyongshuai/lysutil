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
            mysql_set_character_set(this->mysql_, this->conf_->charset.c_str());
            mysql_options(this->mysql_, MYSQL_SET_CHARSET_NAME, this->conf_->charset.c_str());
            //自动重连
            my_bool is_reconnect = true;
            mysql_options(this->mysql_, MYSQL_OPT_RECONNECT, &is_reconnect);
            //连接上后初始命令
            mysql_options(this->mysql_, MYSQL_INIT_COMMAND, "set autocommit=1");
            //连接超时
            mysql_options(this->mysql_, MYSQL_OPT_CONNECT_TIMEOUT, &(this->conf_->conn_timeout));
            mysql_options(this->mysql_, MYSQL_OPT_READ_TIMEOUT, &(this->conf_->read_timeout));
            mysql_options(this->mysql_, MYSQL_OPT_WRITE_TIMEOUT, &(this->conf_->write_timeout));
            MYSQL *p = mysql_real_connect(
                    this->mysql_,
                    this->conf_->host.c_str(),
                    this->conf_->user.c_str(),
                    this->conf_->password.c_str(),
                    this->conf_->dbName.c_str(),
                    this->conf_->port,
                    nullptr,
                    0
            );
            return p != nullptr;
        }

        /**
         * 释放上次的结果
         */
        void MySQLClient::freeResult() {
            if (this->result_) {
                mysql_free_result(this->result_);
                this->result_ = nullptr;
            }
        }

        /**
         * 更新空闲时间点
         */
        void MySQLClient::refreshAliveTime() {
            this->alive_time_ = std::chrono::steady_clock::now();
        }

        /**
         * 计算连接空闲时长
         */
        uint64_t MySQLClient::getAliveTime() {
            std::chrono::nanoseconds res = std::chrono::steady_clock::now() - this->alive_time_;
            std::chrono::milliseconds mil = std::chrono::duration_cast<std::chrono::milliseconds>(res);
            return mil.count();
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
