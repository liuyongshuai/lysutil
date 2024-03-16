/*************************************************************************
 * File:	mysql_connect_pool.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2024-03-16 13:03
 ************************************************************************/
#ifndef _LYSUTIL_MYSQL_CONNECT_POOL_H_202403161303_
#define _LYSUTIL_MYSQL_CONNECT_POOL_H_202403161303_

#include<string>
#include<vector>
#include<map>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<fstream>
#include<thread>
#include "client/mysqlclient/mysql_client.h"

namespace lysutil {
    namespace client {
        class MySQLConnectPool {
        public:
            //对外接口,获取线程池
            static std::shared_ptr<MySQLConnectPool> getConnectPool();

            /**
             * 设置mysql连接信息
             */
            void setMySQLConf(const MySQLConnConf& conf);

            /**
             * 获取一个连接
             */
            std::shared_ptr<MySQLClient> getConnection();

            ~MySQLConnectPool();

        private:
            MySQLConnectPool();
            MySQLConnectPool(const MySQLConnectPool &obj) = delete;
            MySQLConnectPool &operator=(const MySQLConnectPool &obj) = delete;

            //任务函数
            void checkConnection();
            void destoryConnection();
            void addConnect();

        private:
            static std::shared_ptr< MySQLConnectPool > instance_;
            //mysql的连接信息
            MySQLConnConf conf_;
            //最多允许打开多少个连接，默认0不限制
            int max_open_conns_;
            //最少打开的链接数
            int min_open_conns_;
            //单个连接允许的空闲时长
            int max_conn_idle_time_;
            //线程同步
            std::mutex mutex_;
            std::condition_variable cond_;
            std::queue<MySQLClient*> conn_queue_;
        };
    }
} //namespace lysutil


#endif //_LYSUTIL_MYSQL_CONNECT_POOL_H_202403161303_
