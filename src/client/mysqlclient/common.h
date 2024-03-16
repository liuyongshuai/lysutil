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
            uint32_t conn_timeout;
            //读取超时时间，单位秒，默认5秒
            uint32_t read_timeout;
            //写入超时时间，单位秒，默认5秒
            uint32_t write_timeout;
        } MySQLConnConf;
    }
}
#endif //_LYSUTIL_COMMON_H_202403161206_
