/*************************************************************************
 * File:	common.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2024-03-16 12:06
 ************************************************************************/
#ifndef _LYSUTIL_COMMON_H_202403161206_
#define _LYSUTIL_COMMON_H_202403161206_

#include <string>

namespace lysutil {
    namespace client {
        //存储MySQL的连接账号信息
        typedef struct _mysql_conn_conf {
            //连接地址
            std::string host;
            //端口号，默认3306
            uint16_t port;
            //用户名
            std::string user;
            //密码
            std::string password;
            //DB名称
            std::string dbName;
            //设置的字符编码，默认utf8
            std::string charset;
            //连接超时时间，单位秒，默认5秒
            uint32_t timeout;
            //是否自动提交，默认为true
            bool autoCommit;
        } MySQLConnConf;

        typedef struct _mysql_conn_pool_conf {
            int maxIdleConns;           //允许最大空闲连接数，默认为2
            int maxOpenConns;           //最多允许打开多少个连接，默认0不限制
            size_t connMaxLiftTime;     //连接的最大生存时间，默认0不限制，单位秒
        } MySQLConnPoolConf;
    }
}
#endif //_LYSUTIL_COMMON_H_202403161206_
