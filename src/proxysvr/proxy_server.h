/*************************************************************************
 * File:	proxy_server.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2024-03-16 18:44
 ************************************************************************/
#ifndef _LYSUTIL_PROXY_SERVER_H_202403161844_
#define _LYSUTIL_PROXY_SERVER_H_202403161844_

#include<string>
#include<vector>
#include<map>

namespace lysutil {
    /**
     * 代理服务，转发请求
     * 1、工作机制是用户端 -->  proxysvr -->  client  --> 具体的服务。
     * 2、每个client都要有一个唯一的名称name、服务类型type(ssh/http)。
     * 3、proxysvr监听时每个端口号只能对应一个client的服务，要不然都不知道要请求哪。
     * 4、在底层实现时以name、服务类型区分。
     * 5、代理服务要是可动态配置的，服务可提供接口增删监听的端口号。
     */
    class proxyServer {

    };
} //namespace lysutil


#endif //_LYSUTIL_PROXY_SERVER_H_202403161844_
