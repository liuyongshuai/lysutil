/*************************************************************************
 * File:	mysql_connect_pool.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2024-03-16 13:03
 ************************************************************************/
#include<iostream>
#include "client/mysqlclient/mysql_connect_pool.h"

namespace lysutil {
    namespace client {
        std::shared_ptr<MySQLConnectPool> MySQLConnectPool::instance_ = nullptr;
        std::shared_ptr<MySQLConnectPool> MySQLConnectPool::getConnectPool(){
            if (nullptr != instance_){
                return instance_;
            }
            instance_ = std::shared_ptr< MySQLConnectPool >(new MySQLConnectPool);
            //最多允许打开多少个连接
            instance_->max_open_conns_ = 32;
            //最少打开的链接数
            instance_->min_open_conns_ = 2;
            //单个连接允许的空闲时长
            instance_->max_conn_idle_time_ = 30;
            return instance_;
        }

        /**
        * 设置mysql连接信息
        */
        void MySQLConnectPool::setMySQLConf(const MySQLConnConf &conf) {
            this->conf_.host = conf.host;
            this->conf_.port = conf.port;
            this->conf_.user = conf.user;
            this->conf_.dbName = conf.dbName;
            this->conf_.charset = conf.charset;
            this->conf_.password = conf.password;
            this->conf_.conn_timeout = conf.conn_timeout;
            this->conf_.read_timeout = conf.read_timeout;
            this->conf_.write_timeout = conf.write_timeout;
            this->conf_.autoCommit = conf.autoCommit;

            //设置默认值
            if (this->conf_.charset.empty()) {
                this->conf_.charset = "utf8";
            }
            if (this->conf_.conn_timeout == 0) {
                this->conf_.conn_timeout = 5;
            }
            if (this->conf_.read_timeout == 0) {
                this->conf_.read_timeout = 5;
            }
            if (this->conf_.write_timeout == 0) {
                this->conf_.write_timeout = 5;
            }
            if (this->conf_.port == 0) {
                this->conf_.port = 3306;
            }
        }
    }
} //namespace lysutil
