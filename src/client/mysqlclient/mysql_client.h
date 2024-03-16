/*************************************************************************
 * File:	mysql_client.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2019年03月23日 星期六 18时08分17秒
 ************************************************************************/

#ifndef _CLIENT_MYSQL_CLIENT_H_
#define _CLIENT_MYSQL_CLIENT_H_

#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <unistd.h>
#include <memory.h>
#include <chrono>
#include <mysql/mysql.h>
#include "client/mysqlclient/common.h"

namespace lysutil {
    namespace client {
        class MySQLClient {
        public:
            //创建一个MYSQL实例对象并设置字符集
            MySQLClient(const MySQLConnConf &conf) {
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

            //释放资源
            ~MySQLClient();

            //连接
            bool conn();

            //提取单行的单个字段
            bool fetchOne();

            //提取所有行的第一个字段的列表
            bool fetchCols();

            //提取一行数据
            bool fetchRow();

            //提取多行数据
            bool fetchRows();

            //执行一条更新语句
            bool execute();

            //删除一条数据，返回lastAffectedRows
            bool deleteData();

            //写入一条数据，返回lastInsertId
            bool insertData();

            //批量写入数据，返回影响行数
            bool insertBatchData();

            //执行一条：INSERT INTO table (a,b,c) VALUES (1,2,3) ON DUPLICATE KEY UPDATE c=c+1 语句
            bool insertUpdateData();

            //更新一条数据，返回lastAffectedRows
            bool updateData();

            //执行一条select ... for update语句
            bool fetchForUpdate();

            //开启事务
            bool beginTransaction();

            //提交事务
            bool commitTransaction();

            //回滚事务
            bool rollBackTransaction();

            //执行ping命令
            bool ping();

            //更新空闲时间点
            void refreshAliveTime();

            //计算连接空闲时长
            uint64_t getAliveTime();
        private:
            //释放上次的结果
            void freeResult();

            MYSQL *mysql_ = nullptr;
            MYSQL_RES *result_ = nullptr;
            MYSQL_ROW row_;
            //连接信息
            MySQLConnConf conf_;
            std::chrono::steady_clock::time_point alive_time_;
        };
    }
} //namespacecpputils

#endif // _CLIENT_MYSQL_CLIENT_H_
