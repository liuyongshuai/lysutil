/*************************************************************************
 * File:	http_response.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:05
 ************************************************************************/
#ifndef _TRANSTOPIC_HTTP_RESPONSE_H_202306271105_
#define _TRANSTOPIC_HTTP_RESPONSE_H_202306271105_

#include<string>
#include<vector>
#include<map>
#include "httpsvr/http_common.h"
#include "comutils/http_utils.h"

namespace lysutil{
    namespace httpsvr{
        class httpResponse{
        public:
            httpResponse();

            //设置响应值
            void setStatus(HTTP_STATUS status);

            //设置头信息
            void setHeader(const std::string &k, const std::string &v);

            void addHeader(const std::string &k, const std::string &v);

            void getHeader(const std::string &k, std::vector< std::string > &vs);

            //设置cookie信息
            void setCookie(
                    const std::string &k,//KV是必需的
                    const std::string &v,
                    uint32_t maxAgeTime,//相对过期时间，单位秒，删除cookie时传负值
                    const std::string &path,//指定的路径信息
                    const std::string &domain,//指定的域名，默认为创建cookie的网页所属域名
                    bool secure,//只对HTTPS请求可见，对HTTP请求不可见
                    bool httponly//对浏览器端的javascript中的document对象不可见
            );

            //设置响应的body信息
            void setBody(const std::string &body);

            //返回json信息
            void setJsonHeader();

            //获取要返回的响应信息
            void getRsp(std::string &rsp);

        private:
            HTTP_STATUS status_;
            std::map< std::string, std::vector< std::string>> headers_;
            std::string body_;

            //生成header信息
            void genHeader(std::string &h);
        };
    }
} //namespace pfsm_logsim


#endif //_TRANSTOPIC_HTTP_RESPONSE_H_202306271105_
