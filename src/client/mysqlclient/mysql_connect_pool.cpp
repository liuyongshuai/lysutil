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

        std::shared_ptr<MySQLConnectPool> MySQLConnectPool::getConnectPool() {
            if (nullptr != instance_) {
                return instance_;
            }
            instance_ = std::shared_ptr<MySQLConnectPool>(new MySQLConnectPool);
            //最多允许打开多少个连接
            instance_->max_open_conns_ = 32;
            //最少打开的链接数
            instance_->min_open_conns_ = 2;
            //单个连接允许的空闲时长
            instance_->max_conn_idle_time_ = 30;
            //获取连接时最长等待时间
            instance_->max_wait_conn_milliseconds_ = 5000;
            return instance_;
        }

        /**
         * 构造函数
         */
        MySQLConnectPool::MySQLConnectPool() {
            //创建最少连接数
            for (uint64_t i = 0; i < this->min_open_conns_; ++i) {
                this->addConnect();
            }
            //创建子线程用于检测并创建新的连接
            std::thread producer(&MySQLConnectPool::checkConnection, this);
            //销毁连接,检测并销毁连接
            std::thread recycler(&MySQLConnectPool::destoryConnection, this);
            //设置线程分离
            producer.detach();
            recycler.detach();
        }

        /**
         * 新增一条连接
         */
        void MySQLConnectPool::addConnect() {
            MySQLClient *conn = new MySQLClient(&(this->conf_));
            conn->conn();
            conn->refreshAliveTime();
            this->conn_queue_.push(conn);
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

        //获取线程池中的连接
        std::shared_ptr<MySQLClient> MySQLConnectPool::getConnection() {
            //需要操作共享资源
            std::unique_lock<std::mutex> locker(this->mutex_);
            //判断连接池队列为空
            while (this->conn_queue_.empty()) {
                if (std::cv_status::timeout == this->cond_.wait_for(locker, std::chrono::milliseconds(this->max_wait_conn_milliseconds_))) {
                    if (this->conn_queue_.empty()) {
                        continue;
                    }
                }
            }
            //自定义shared_ptr析构方法,重新将连接放回到连接池中,而不是销毁
            std::shared_ptr<MySQLClient> connptr(this->conn_queue_.front(), [this](MySQLClient *conn) {
                std::unique_lock<std::mutex> locker(this->mutex_);
                conn->refreshAliveTime();
                this->conn_queue_.push(conn);
            });
            //弹出,放到了队尾
            this->conn_queue_.pop();
            this->cond_.notify_all();
            return connptr;
        }

        void MySQLConnectPool::checkConnection() {
            while (true) {
                std::unique_lock<std::mutex> locker(this->mutex_);
                //判断连接数是否达到容量,如果大于等于容量则需要阻塞一段时间
                while (this->conn_queue_.size() >= this->max_open_conns_) {
                    this->cond_.wait(locker);
                }
                this->addConnect();
                this->cond_.notify_all();
            }
        }

        void MySQLConnectPool::destoryConnection() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::unique_lock<std::mutex> locker(this->mutex_);
                while (this->conn_queue_.size() > this->min_open_conns_) {
                    MySQLClient *recyConn = this->conn_queue_.front();
                    //如果超时则销毁
                    if (recyConn->getAliveTime() >= this->max_conn_idle_time_) {
                        this->conn_queue_.pop();
                        delete recyConn;
                    } else {
                        break;
                    }
                }
            }
        }

        MySQLConnectPool::~MySQLConnectPool() {
            while (!this->conn_queue_.empty()) {
                MySQLClient *conn = this->conn_queue_.front();
                this->conn_queue_.pop();
                delete conn;
            }
        }

    }
} //namespace lysutil
