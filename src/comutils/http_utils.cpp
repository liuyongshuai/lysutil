/*************************************************************************
 * File:	http_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:23
 ************************************************************************/
#include<iostream>
#include "http_utils.h"

namespace haomo{
    namespace transtopic{

        std::unordered_set <uint8_t> httpUtils::isTokenTable = {
                '!', '#', '$', '%', '&', '\'', '*', '+', '-', '.', '0', '1', '2', '3', '4',
                '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
                'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'W', 'V', 'X', 'Y', 'Z', '^',
                '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '|', '~',
        };

        //规范化header的key
        void httpUtils::formatHeaderKey(std::string &key){
            size_t klen = key.size();
            size_t i;
            for (i = 0; i < klen; i++){
                uint8_t c = key[i];
                if (validHeaderFieldByte(c)){
                    continue;
                }
                return;
            }
            uint8_t c;
            bool upper = true;
            for (i = 0; i < klen; i++){
                c = key[i];
                if (upper && c >= 'a' && c <= 'z'){
                    c = toupper(c);
                }
                else if (!upper && c >= 'A' && c <= 'Z'){
                    c = tolower(c);
                }
                key[i] = c;
                upper = (c == '-');
            }
        }

        //是否为合法字符
        bool httpUtils::validHeaderFieldByte(uint8_t c){
            if ((int) c < 127 && isTokenTable.find(c) != isTokenTable.end()){
                return true;
            }
            return false;
        }


        //规范化header的key
        void httpUtils::canonicalHeaderKey(std::string &key){
            bool upper = true;
            size_t i;
            for (i = 0; i < key.size(); i++){
                uint8_t c = key[i];
                if (!validHeaderFieldByte(c)){
                    return;
                }
                if (upper && c >= 'a' && c <= 'z'){
                    formatHeaderKey(key);
                    return;
                }
                if (!upper && c >= 'A' && c <= 'Z'){
                    formatHeaderKey(key);
                    return;
                }
                upper = (c == '-');
            }
            return;
        }


        //切割cookie信息
        void httpUtils::splitRawCookie(const std::string &rawCookie, std::map <std::string, std::string> &cookies){
            std::vector <std::string> kvs;
            strUtils::strSplit(rawCookie, ';', kvs);
            if (kvs.size() <= 0){
                return;
            }
            size_t i;
            size_t pos;
            for (i = 0; i < kvs.size(); i++){
                std::string kv = kvs[i];
                pos = kv.find('=', 0);
                if (pos == std::string::npos){
                    continue;
                }
                std::string k(kv, 0, pos);
                std::string v(kv, pos + 1, kv.size());
                strUtils::trimSpace(k);
                strUtils::trimSpace(v);
                if (k.size() <= 0){
                    continue;
                }
                cookies[k] = v;
            }
        }

        //合并cookie
        void httpUtils::joinRawCookie(const std::map <std::string, std::string> &cookie, std::string &rawCookie){
            std::map< std::string, std::string >::const_iterator iter;
            for (iter = cookie.begin(); iter != cookie.end(); iter++){
                rawCookie.append(iter->first).append("=").append(iter->second).append("; ");
            }
            strUtils::trimSpace(rawCookie);
            strUtils::trimChar(rawCookie, ';');
        }

        //切割原始的头信息
        void httpUtils::splitRawHeaders(const std::string &header, std::map <std::string, std::string> &rspHeader){
            std::vector <std::string> sublist;
            strUtils::strSplit(header, '\n', sublist);
            std::vector< std::string >::const_iterator iter;
            for (iter = sublist.begin(); iter != sublist.end(); iter++){
                std::string kv = *iter;
                strUtils::trimSpace(kv);
                size_t pos = kv.find_first_of(":");
                if (pos == std::string::npos){
                    continue;
                }
                std::string k = kv.substr(0, pos);
                strUtils::trimSpace(k);
                if (k.size() <= 0){
                    continue;
                }
                strUtils::toLower(k);
                std::string v = kv.substr(pos + 1, kv.size());
                strUtils::trimSpace(v);
                rspHeader.insert(std::pair< std::string, std::string >(k, v));
            }
        }
    }
} //namespace Project
