/*************************************************************************
 * File:	http_request.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:02
 ************************************************************************/
#ifndef _TRANSTOPIC_HTTP_REQUEST_H_202306271102_
#define _TRANSTOPIC_HTTP_REQUEST_H_202306271102_

#include<string>
#include<vector>
#include<map>
#include "http_common.h"
#include "comutils/str_utils.h"
#include "comutils/terminal_table.h"
#include "comutils/pcre_utils.h"

namespace haomo{
    namespace transtopic{
        class httpRequest{
        public:
            httpRequest(const char *body, size_t bodyLen);

            /**
             * 提取header中的信息
             */
            void getHeader(const std::string &key, std::string &val);

            /**
             * 提取单个请求参数
             */
            void getArg(const std::string &key, std::string &val);

            /**
             * 提取请求参数的数组
             */
            void getArg(const std::string &key, std::vector <std::string> &vals);

            /**
             * 是否为ajax请求
             */
            bool isAjax();

            /**
             * 打印请求的信息
             */
            void printReq();

            /**
             * 释放资源
             */
            ~httpRequest();

        public:
            /**
             * 当前请求的唯一标识
             */
            std::string uniqueId;

            /**
             * 请求时的host
             */
            std::string host;

            /**
             * 请求时的uri
             */
            std::string uri;

            /**
             * 请求时的客户端IP
             */
            std::string client_ip;

            /**
             * 请求时用的协议，如"HTTP/1.1"
             */
            std::string protocol;

            /**
             * 请求时的方法
             */
            HTTP_METHOD method;

            /**
             * 请求的头信息
             */
            std::map <std::string, std::string> headers;

            /**
             * 请求时带的KV参数
             */
            std::map <std::string, std::vector< std::string >> args;

            /**
             * 上传的文件列表
             */
            std::map <std::string, uploadFile> uploadFiles;

        private:
            /**
             * 解析http请求的body，会填充请求参数、上传文件等各种信息，基本格式：
             * GET /abc.....
             * Key: Value
             *
             * bodyBin
             */
            int parseBody(const void *body, size_t bodyLen);

            /**
             * 简单的解析参数，请求的Content-Type必须是multipart/form-data，如：
             *      此类请求中，一个参数的开始是 "--" + boundary
             *      然后加上参数的Header信息，格式为字段名和字段值，二者之间使用“：”连接，如：Content-Type：text/plain
             *      加上一个空行（\r\n）
             *      发送的参数值
             *      整个请求的报文数据以 "--" + boundary + "--" 结束
             * 如下，boundary = "----WebKitFormBoundary5cab1a0c5b5f5"
             * ------WebKitFormBoundary5cab1a0c5b5f5
             * Content-Disposition: form-data; name="key"
             * Content-Type: text/plain; charset=UTF-8  //可省略
             *
             * value
             * ------WebKitFormBoundary5cab1a0c5b5f5
             * ..........其他请求参数........
             * ------WebKitFormBoundary5cab1a0c5b5f5--
             *
             * @param   boundary    从header中解析出来的分隔符
             * @param   rawBody     去掉header后的原始的请求body
             * @param   rawBodyLen  原始body的长度
             */
            void parseMultiPartFormDataArgs(const std::string &boundary, const char *rawBody, size_t rawBodyLen);

            /**
             * 简单的解析KV参数
             */
            void parseArgs(const std::string &str);
        };
    }
} //namespace pfsm_logsim


#endif //_TRANSTOPIC_HTTP_REQUEST_H_202306271102_
