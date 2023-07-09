/*************************************************************************
 * File:	common.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:24
 ************************************************************************/
#ifndef _TRANSTOPIC_COMMON_H_202306271124_
#define _TRANSTOPIC_COMMON_H_202306271124_

#include<string>
#include<vector>
#include<map>
#include<memory>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<fcntl.h>
#include<deque>
#include<pthread.h>
#include<semaphore.h>
#include<errno.h>
#include<signal.h>
#include<cctype>
#include<sstream>
#include<utility>
#include<stdexcept>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/shm.h>

#define HTTP_HEADER_BUF_SIZE  10240

namespace lysutil{
    namespace httpsvr{
        class httpRequest;

        class httpResponse;

        //要执行的方法
        typedef void (*httpFunc)(httpRequest &req, httpResponse &rsp);

        //POST配置相关
        const static std::string NORMAL_POST_CONTENT_TYPE = "application/x-www-form-urlencoded";
        const static std::string UPLOAD_POST_CONTENT_TYPE = "multipart/form-data";

        //上传的文件类型
        typedef struct _upload_file{
            std::string fileName;
            std::string contentType;
            char *fileBuf;
            size_t fileLen;
        } uploadFile;

        //请求方法类型
        typedef enum{
            GET = 1, POST, PUT, HEAD, DELETE
        } HTTP_METHOD;


        //路由匹配类型
        typedef enum _router_type{
            ROUTER_TYPE_PATH_INFO = 1,//全路径匹配
            ROUTER_TYPE_REGEXP//正则匹配
        } ROUTER_TYPE;

        //单个路由匹配项
        typedef struct _router_item{
            ROUTER_TYPE type;//路由匹配类型
            std::string config;//规则信息
            httpFunc func;//控制层
            std::string extParam;//额外的参数
        } routerItem;

        //http的状态
        typedef enum{
            CONTINUE = 100, //Continue	继续。客户端应继续其请求,
            SWITCHING_PROTOCOLS = 101, //Switching Protocols	切换协议。服务器根据客户端的请求切换协议。只能切换到更高级的协议，例如，切换到HTTP的新版本协议,
            OK = 200, //OK	请求成功。一般用于GET与POST请求,
            CREATED = 201, //Created	已创建。成功请求并创建了新的资源,
            ACCEPTED = 202, //Accepted	已接受。已经接受请求，但未处理完成,
            NON_AUTHORITATIVE_INFORMATION = 203, //Non-Authoritative Information	非授权信息。请求成功。但返回的meta信息不在原始的服务器，而是一个副本,
            NO_CONTENT = 204, //No Content	无内容。服务器成功处理，但未返回内容。在未更新网页的情况下，可确保浏览器继续显示当前文档,
            RESET_CONTENT = 205, //Reset Content	重置内容。服务器处理成功，用户终端（例如：浏览器）应重置文档视图。可通过此返回码清除浏览器的表单域,
            PARTIAL_CONTENT = 206, //Partial Content	部分内容。服务器成功处理了部分GET请求,
            MULTIPLE_CHOICES = 300, //Multiple Choices	多种选择。请求的资源可包括多个位置，相应可返回一个资源特征与地址的列表用于用户终端（例如：浏览器）选择,
            MOVED_PERMANENTLY = 301, //Moved Permanently	永久移动。请求的资源已被永久的移动到新URI，返回信息会包括新的URI，浏览器会自动定向到新URI。今后任何新的请求都应使用新的URI代替,
            FOUND = 302, //Found	临时移动。与类似。但资源只是临时被移动。客户端应继续使用原有URI,
            SEE_OTHER = 303, //See Other	查看其它地址。与类似。使用GET和POST请求查看,
            NOT_MODIFIED = 304, //Not Modified	未修改。所请求的资源未修改，服务器返回此状态码时，不会返回任何资源。客户端通常会缓存访问过的资源，通过提供一个头信息指出客户端希望只返回在指定日期之后修改的资源,
            USE_PROXY = 305, //Use Proxy	使用代理。所请求的资源必须通过代理访问,
            UNUSED = 306, //Unused	已经被废弃的HTTP状态码,
            TEMPORARY_REDIRECT = 307, //Temporary Redirect	临时重定向。与类似。使用GET请求重定向,
            BAD_REQUEST = 400, //Bad Request	客户端请求的语法错误，服务器无法理解,
            UNAUTHORIZED = 401, //Unauthorized	请求要求用户的身份认证,
            PAYMENT_REQUIRED = 402, //Payment Required	保留，将来使用,
            FORBIDDEN = 403, //Forbidden	服务器理解请求客户端的请求，但是拒绝执行此请求,
            NOT_FOUND = 404, //Not Found	服务器无法根据客户端的请求找到资源（网页）。通过此代码，网站设计人员可设置您所请求的资源无法找到的个性页面,
            METHOD_NOT_ALLOWED = 405, //Method Not Allowed	客户端请求中的方法被禁止,
            NOT_ACCEPTABLE = 406, //Not Acceptable	服务器无法根据客户端请求的内容特性完成请求,
            PROXY_AUTHENTICATION_REQUIRED = 407, //Proxy Authentication Required	请求要求代理的身份认证，与类似，但请求者应当使用代理进行授权,
            REQUEST_TIME_OUT = 408, //Request Time-out	服务器等待客户端发送的请求时间过长，超时,
            CONFLICT = 409, //Conflict	服务器完成客户端的PUT请求是可能返回此代码，服务器处理请求时发生了冲突,
            GONE = 410, //Gone	客户端请求的资源已经不存在。不同于，如果资源以前有现在被永久删除了可使用代码，网站设计人员可通过代码指定资源的新位置,
            LENGTH_REQUIRED = 411, //Length Required	服务器无法处理客户端发送的不带Content-Length的请求信息,
            PRECONDITION_FAILED = 412, //Precondition Failed	客户端请求信息的先决条件错误,
            REQUEST_ENTITY_TOO_LARGE = 413, //Request Entity Too Large	由于请求的实体过大，服务器无法处理，因此拒绝请求。为防止客户端的连续请求，服务器可能会关闭连接。如果只是服务器暂时无法处理，则会包含一个Retry-After的响应信息,
            REQUEST_URI_TOO_LARGE = 414, //Request-URI Too Large	请求的URI过长（URI通常为网址），服务器无法处理,
            UNSUPPORTED_MEDIA_TYPE = 415, //Unsupported Media Type	服务器无法处理请求附带的媒体格式,
            REQUESTED_RANGE_NOT_SATISFIABLE = 416, //Requested range not satisfiable	客户端请求的范围无效,
            EXPECTATION_FAILED = 417, //Expectation Failed	服务器无法满足Expect的请求头信息,
            INTERNAL_SERVER_ERROR = 500, //Internal Server Error	服务器内部错误，无法完成请求,
            NOT_IMPLEMENTED = 501, //Not Implemented	服务器不支持请求的功能，无法完成请求,
            BAD_GATEWAY = 502, //Bad Gateway	充当网关或代理的服务器，从远端服务器接收到了一个无效的请求,
            SERVICE_UNAVAILABLE = 503, //Service Unavailable	由于超载或系统维护，服务器暂时的无法处理客户端的请求。延时的长度可包含在服务器的Retry-After头信息中,
            GATEWAY_TIME_OUT = 504, //Gateway Time-out	充当网关或代理的服务器，未及时从远端服务器获取请求,
            VERSION_NOT_SUPPORTED = 505, //HTTP Version not supported	服务器不支持请求的HTTP协议的版本，无法完成处理,
        } HTTP_STATUS;

        //各个状态对应的说明
        const static std::map <HTTP_STATUS, std::string> httpStatusDesc = {
                {CONTINUE,                        "Continue"},
                {SWITCHING_PROTOCOLS,             "Switching Protocols"},
                {OK,                              "OK"},
                {CREATED,                         "Created"},
                {ACCEPTED,                        "Accepted"},
                {NON_AUTHORITATIVE_INFORMATION,   "Non-Authoritative Information"},
                {NO_CONTENT,                      "No Content"},
                {RESET_CONTENT,                   "Reset Content"},
                {PARTIAL_CONTENT,                 "Partial Content"},
                {MULTIPLE_CHOICES,                "Multiple Choices"},
                {MOVED_PERMANENTLY,               "Moved Permanently"},
                {FOUND,                           "Found"},
                {SEE_OTHER,                       "See Other"},
                {NOT_MODIFIED,                    "Not Modified"},
                {USE_PROXY,                       "Use Proxy"},
                {UNUSED,                          "Unused"},
                {TEMPORARY_REDIRECT,              "Temporary Redirect"},
                {BAD_REQUEST,                     "Bad Request"},
                {UNAUTHORIZED,                    "Unauthorized"},
                {PAYMENT_REQUIRED,                "Payment Required"},
                {FORBIDDEN,                       "Forbidden"},
                {NOT_FOUND,                       "Not Found"},
                {METHOD_NOT_ALLOWED,              "Method Not Allowed"},
                {NOT_ACCEPTABLE,                  "Not Acceptable"},
                {PROXY_AUTHENTICATION_REQUIRED,   "Proxy Authentication Required"},
                {REQUEST_TIME_OUT,                "Request Time out"},
                {CONFLICT,                        "Conflict"},
                {GONE,                            "Gone"},
                {LENGTH_REQUIRED,                 "Length Required"},
                {PRECONDITION_FAILED,             "Precondition Failed"},
                {REQUEST_ENTITY_TOO_LARGE,        "Request Entity Too Large"},
                {REQUEST_URI_TOO_LARGE,           "Request-URI Too Large"},
                {UNSUPPORTED_MEDIA_TYPE,          "Unsupported Media Type"},
                {REQUESTED_RANGE_NOT_SATISFIABLE, "Requested range not satisfiable"},
                {EXPECTATION_FAILED,              "Expectation Failed"},
                {INTERNAL_SERVER_ERROR,           "Internal Server Error"},
                {NOT_IMPLEMENTED,                 "Not Implemented"},
                {BAD_GATEWAY,                     "Bad Gateway"},
                {SERVICE_UNAVAILABLE,             "Service Unavailable"},
                {GATEWAY_TIME_OUT,                "Gateway Time out"},
                {VERSION_NOT_SUPPORTED,           "HTTP Version not supported"}
        };
    }
}
#endif //_TRANSTOPIC_COMMON_H_202306271124_
