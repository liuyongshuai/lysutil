/*************************************************************************
 * File:	http_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:23
 ************************************************************************/
#ifndef _TRANSTOPIC_HTTP_UTILS_H_202306271123_
#define _TRANSTOPIC_HTTP_UTILS_H_202306271123_

#include<string>
#include<vector>
#include<map>
#include "comutils/str_utils.h"

namespace lysutil{
    namespace comutils{
        class httpUtils{
        public:
            //规范化header的key
            static void canonicalHeaderKey(std::string &key);

            //切割cookie信息
            static void splitRawCookie(const std::string &rawCookie, std::map <std::string, std::string> &cookie);

            //合并cookie
            static void joinRawCookie(const std::map <std::string, std::string> &cookie, std::string &rawCookie);

            //切割原始的头信息
            static void splitRawHeaders(const std::string &header, std::map <std::string, std::string> &rspHeader);

            //http响应中用的日期格式
            static std::string genRespDate(uint32_t maxAgeTime);
            static std::string genFixedRespDate(time_t time);
        private:
            httpUtils() = delete;
            httpUtils(const httpUtils &) = delete;
            httpUtils &operator=(const httpUtils &) = delete;

            //规范化header的key
            static void formatHeaderKey(std::string &key);
            static bool validHeaderFieldByte(uint8_t c);
            static std::unordered_set <uint8_t> isTokenTable;
        };
    }
} //namespace Project


#endif //_TRANSTOPIC_HTTP_UTILS_H_202306271123_
