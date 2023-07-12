/*************************************************************************
 * File:	str_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:55
 ************************************************************************/
#include<iostream>
#include "str_utils.h"

#define ENCODE64(c)     (((c) & 0xC0) ? -1 : base64digits[(int)c])
#define DECODE64(c)     (((c) & 0x80) ? -1 : base64val[(int)c])
#define UTF8_CHAR_CHECK(c) if(c<(uint8_t)0x80||c>(uint8_t)0xBF){break;}
namespace lysutil{
    namespace comutils{
        //基本常用汉字的unicode范围
        int32_t strUtils::chineseCharUnicode[2] = {0x4E00, 0x9FA5};
        //半角阿拉伯数字0~9范围
        int32_t strUtils::enNumberCharUnicode[2] = {0x30, 0x39};
        //半角大写字母的unicode范围
        int32_t strUtils::enUpperLetterCharUnicode[2] = {0x41, 0x5A};
        //半角小写字母的unicode范围
        int32_t strUtils::enLowerLetterCharUnicode[2] = {0x61, 0x7A};
        //全角阿拉伯数字0~9范围
        int32_t strUtils::cnNumberCharUnicode[2] = {65296, 65305};
        //全角大写字母的unicode范围
        int32_t strUtils::cnUpperLetterCharUnicode[2] = {65313, 65338};
        //全角小写字母的unicode范围
        int32_t strUtils::cnLowerLetterCharUnicode[2] = {65345, 65370};
        unsigned char strUtils::hexchars[] = "0123456789ABCDEF";
        char strUtils::base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        char strUtils::base64val[] = {
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
                -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
                -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
        };

        /**
         * 只保留str中的汉字、字母、数字，其他的一律替换为空格
         */
        void strUtils::keepNormChar(std::string &str){
            //先转为unicode
            std::vector< int32_t > unicodes;
            utf8ToUnicodes(str, unicodes);
            for (size_t i = 0; i < unicodes.size(); i++){
                int32_t unicode = unicodes[i];
                //如果是全角的话
                if (isCnLowerLetter(unicode) || isCnUpperLetter(unicode) || isCnNumber(unicode)){
                    unicodes[i] = unicode - 65248;
                    continue;
                }
                //再判断是否为正常的汉字、字母、数字，如果不是的话则直接替换为空格的unicode码32
                if (!(isCnChar(unicode) || isEnNumber(unicode) || isEnUpperLetter(unicode) || isEnLowerLetter(unicode))){
                    unicodes[i] = 32;
                    continue;
                }
            }
            //将多个相邻空格替换为一个
            std::vector< int32_t > newUnicodes;
            for (int unicode: unicodes){
                //如果newUnicodes最后一个是空格，当前这个又是空格，则忽略之
                if (unicode == 32 && !newUnicodes.empty() && newUnicodes[newUnicodes.size() - 1] == 32){
                    continue;
                }
                newUnicodes.push_back(unicode);
            }

            std::string tmpStr;
            unicodesToUTF8(newUnicodes, tmpStr);
            str = tmpStr;
        }

        //str里是否包含中文
        bool strUtils::isContainChinese(const std::string &str){
            //先转为unicode
            std::vector< int32_t > unicodes;
            utf8ToUnicodes(str, unicodes);
            for (int unicode: unicodes){
                if (isCnChar(unicode)){
                    return true;
                }
            }
            return false;
        }

        //是否为常用的中文汉字
        bool strUtils::isCnChar(const wchar_t &unicode){
            return unicode >= chineseCharUnicode[0] && unicode <= chineseCharUnicode[1];
        }

        //是否为半角小写字母
        bool strUtils::isEnLowerLetter(const wchar_t &unicode){
            return unicode >= enLowerLetterCharUnicode[0] && unicode <= enLowerLetterCharUnicode[1];
        }


        //是否为半角大写写字母
        bool strUtils::isEnUpperLetter(const wchar_t &unicode){
            return unicode >= enUpperLetterCharUnicode[0] && unicode <= enUpperLetterCharUnicode[1];
        }


        //是否为半角阿拉伯数字
        bool strUtils::isEnNumber(const wchar_t &unicode){
            return unicode >= enNumberCharUnicode[0] && unicode <= enNumberCharUnicode[1];
        }


        //是否为全角小写字母
        bool strUtils::isCnLowerLetter(const wchar_t &unicode){
            return unicode >= cnLowerLetterCharUnicode[0] && unicode <= cnLowerLetterCharUnicode[1];
        }


        //是否为全角大写写字母
        bool strUtils::isCnUpperLetter(const wchar_t &unicode){
            return unicode >= cnUpperLetterCharUnicode[0] && unicode <= cnUpperLetterCharUnicode[1];
        }

        //是否为全角阿拉伯数字
        bool strUtils::isCnNumber(const wchar_t &unicode){
            return unicode >= cnNumberCharUnicode[0] && unicode <= cnNumberCharUnicode[1];
        }

        //是否全为数字
        bool strUtils::isAllNumber(const std::string &str){
            if (str.empty()){
                return false;
            }
            for (const char &c: str){
                if (!isdigit(c)){
                    return false;
                }
            }
            return true;
        }

        //md5加密
        std::string strUtils::md5sum(const std::string &str){
            md5Utils md5{};
            char buf[32] = {0};
            md5.MD5Str(str.c_str(), str.size(), buf, 32);
            return buf;
        }

        //vec1是否完全包含vec2
        bool strUtils::isVecContainAll(const std::vector< std::string > &vec1, const std::vector< std::string > &vec2){
            if (vec1.size() < vec2.size() || vec1.empty() || vec2.empty()){
                return false;
            }
            std::map< std::string, bool > m1;
            for (auto it = vec1.cbegin(); it != vec1.cend(); it++){
                std::string t = *it;
                trimSpace(t);
                m1.insert(std::make_pair(t, true));
            }
            for (auto it = vec2.cbegin(); it != vec2.cend(); it++){
                std::string t = *it;
                trimSpace(t);
                if (m1.find(t) == m1.end()){
                    return false;
                }
            }
            return true;
        }

        //去掉两端的空格
        void strUtils::trimSpace(std::string &str){
            size_t slen = str.size();
            if (slen <= 0){
                return;
            }
            const char *pstart = str.c_str();
            const char *pend = pstart + slen - 1;
            char c;
            size_t i;
            for (i = 0; i < slen; i++){
                c = *pstart;
                if (isspace(c) == 0){
                    break;
                }
                pstart++;
            }
            for (i = slen - 1; (i >= 0 && pend > pstart); i--){
                c = *pend;
                if (isspace(c) == 0){
                    break;
                }
                pend--;
            }
            std::string tmpStr(pstart, pend - pstart + 1);
            str = tmpStr;
        }

        //简简单单的连接
        void strUtils::strJoin(const std::vector< std::string > &in, const std::string &j, std::string &out){
            size_t i, len = in.size();
            for (i = 0; i < len; i++){
                out.append(in[i]);
                if (i != len - 1){
                    out.append(j);
                }
            }
        }

        //简简单单的连接
        void strUtils::strJoin(const std::set< std::string > &in, const std::string &j, std::string &out){
            std::vector< std::string > inVec;
            inVec.reserve(in.size());
            for (const std::string &v: in){
                inVec.push_back(v);
            }
            strJoin(inVec, j, out);
        }

        //简简单单的连接
        std::string strUtils::strJoin(const std::vector< std::string > &in, const std::string &j){
            std::string ret;
            strJoin(in, j, ret);
            return ret;
        }


        //去掉端的空格
        void strUtils::trimLeftSpace(std::string &str){
            size_t slen = str.size();
            if (slen <= 0){
                return;
            }
            const char *pstart = str.c_str();
            const char *pend = pstart + slen - 1;
            char c;
            size_t i;
            for (i = 0; i < slen; i++){
                c = *pstart;
                if (isspace(c) == 0){
                    break;
                }
                pstart++;
            }
            std::string tmpStr(pstart, pend - pstart + 1);
            str = tmpStr;
        }

        //去掉端的空格
        void strUtils::trimRightSpace(std::string &str){
            size_t slen = str.size();
            if (slen <= 0){
                return;
            }
            const char *pstart = str.c_str();
            const char *pend = pstart + slen - 1;
            char c;
            int i;
            for (i = slen - 1; (i >= 0 && pend > pstart); i--){
                c = *pend;
                if (isspace(c) == 0){
                    break;
                }
                pend--;
            }
            std::string tmpStr(pstart, pend - pstart + 1);
            str = tmpStr;
        }

        //去掉两端的Char
        void strUtils::trimChar(std::string &str, char ch){
            size_t slen = str.size();
            if (slen <= 0){
                return;
            }
            const char *pstart = str.c_str();
            const char *pend = pstart + slen - 1;
            char c;
            size_t i;
            for (i = 0; i < slen; i++){
                c = *pstart;
                if (c != ch){
                    break;
                }
                pstart++;
            }
            for (i = slen - 1; (i >= 0 && pend > pstart); i--){
                c = *pend;
                if (c != ch){
                    break;
                }
                pend--;
            }
            std::string tmpStr(pstart, pend - pstart + 1);
            str = tmpStr;
        }

        //去掉端的Char
        void strUtils::trimLeftChar(std::string &str, char ch){
            size_t slen = str.size();
            if (slen <= 0){
                return;
            }
            const char *pstart = str.c_str();
            const char *pend = pstart + slen - 1;
            char c;
            size_t i;
            for (i = 0; i < slen; i++){
                c = *pstart;
                if (c != ch){
                    break;
                }
                pstart++;
            }
            std::string tmpStr(pstart, pend - pstart + 1);
            str = tmpStr;
        }

        //去掉端的Char
        void strUtils::trimRightChar(std::string &str, char ch){
            size_t slen = str.size();
            if (slen <= 0){
                return;
            }
            const char *pstart = str.c_str();
            const char *pend = pstart + slen - 1;
            char c;
            int i;
            for (i = slen - 1; (i >= 0 && pend > pstart); i--){
                c = *pend;
                if (c != ch){
                    break;
                }
                pend--;
            }
            std::string tmpStr(pstart, pend - pstart + 1);
            str = tmpStr;
        }

        //简简单单的替换
        void strUtils::strReplace(const std::string &in, const std::string &replace, const std::string &r, std::string &out){
            std::string tmpIN = in;
            size_t start_pos = 0, pos;
            while ((pos = tmpIN.find(replace, start_pos)) != std::string::npos){
                tmpIN.replace(pos, replace.size(), r);
                start_pos += replace.size();
            }
            out = tmpIN;
        }

        //简简单单的切割
        void strUtils::strSplit(const std::string &in, char c, std::vector< std::string > &out){
            size_t prev_pos = 0, pos = 0;
            while ((pos = in.find(c, prev_pos)) != std::string::npos){
                std::string tmp(in, prev_pos, pos - prev_pos);
                out.push_back(tmp);
                prev_pos = pos + 1;
            }
            if (prev_pos < in.size()){
                std::string tmp(in, prev_pos, in.size() - prev_pos);
                out.push_back(tmp);
            }
        }

        //是否以某个字符串开头
        bool strUtils::hasPrefix(const std::string &str, const std::string &substr){
            if (str.length() < substr.length()){
                return false;
            }
            return (0 == str.compare(0, substr.length(), substr));
        }

        //是否以某个字符串结尾
        bool strUtils::hasSuffix(const std::string &str, const std::string &substr){
            if (str.length() < substr.length()){
                return false;
            }
            return (0 == str.compare(str.length() - substr.length(), substr.length(), substr));
        }

        //转为大写
        void strUtils::toUpper(std::string &str){
            size_t i, len = str.size();
            for (i = 0; i < len; i++){
                str[i] = toupper(str[i]);
            }
        }

        //转为小写
        void strUtils::toLower(std::string &str){
            size_t i, len = str.size();
            for (i = 0; i < len; i++){
                str[i] = tolower(str[i]);
            }
        }

        /**
         * 提取字符串的unicode值
         * 1字节 0xxxxxxx
         * 2字节 110xxxxx 10xxxxxx
         * 3字节 1110xxxx 10xxxxxx 10xxxxxx
         * 4字节 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
         * 5字节 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
         * 6字节 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
         */
        int strUtils::utf8ToUnicodes(const std::string &str, std::vector< int32_t > &unicodes){
            if (str.empty()){
                return -1;
            }
            const char *strStartPtr = str.c_str();
            const char *strEndPtr = str.c_str() + str.size();
            for (;;){
                int32_t unicode = 0;
                uint8_t c = *strStartPtr;
                //先判断是否为字符串结束符
                if ('\0' == c){
                    break;
                }
                if (c <= (uint8_t) 0x7F){//单字节编码形式：0xxxxxxx
                    unicode = (int32_t) c;
                    strStartPtr++;
                }
                else if (c <= (uint8_t) 0xDF && c >= (uint8_t) 0xC0){//双字节编码形式：110xxxxx 10xxxxxx
                    if (strEndPtr - strStartPtr < 1){
                        break;
                    }
                    uint8_t char1 = *strStartPtr;
                    uint8_t char2 = *(strStartPtr + 1);
                    UTF8_CHAR_CHECK(char2)

                    int32_t t1 = (int32_t) (char1 & 0x1F) << 6;
                    int32_t t2 = (int32_t) (char2 & 0x3F);
                    unicode = t1 | t2;
                    strStartPtr += 2;
                }
                else if (c <= (uint8_t) 0xEF && c >= (uint8_t) 0xE0){//三字节编码形式：1110xxxx 10xxxxxx 10xxxxxx
                    if (strEndPtr - strStartPtr < 2){
                        break;
                    }
                    uint8_t char1 = *strStartPtr;
                    uint8_t char2 = *(strStartPtr + 1);
                    UTF8_CHAR_CHECK(char2)
                    uint8_t char3 = *(strStartPtr + 2);
                    UTF8_CHAR_CHECK(char3)

                    int32_t t1 = (int32_t) (char1 & 0x0F) << 12;
                    int32_t t2 = (int32_t) (char2 & 0x3F) << 6;
                    int32_t t3 = (int32_t) (char3 & 0x3F);
                    unicode = t1 | t2 | t3;
                    strStartPtr += 3;
                }
                else if (c <= (uint8_t) 0xF7 && c >= (uint8_t) 0xF0){//四字节编码形式：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    if (strEndPtr - strStartPtr < 3){
                        break;
                    }
                    uint8_t char1 = *strStartPtr;
                    uint8_t char2 = *(strStartPtr + 1);
                    UTF8_CHAR_CHECK(char2)
                    uint8_t char3 = *(strStartPtr + 2);
                    UTF8_CHAR_CHECK(char3)
                    uint8_t char4 = *(strStartPtr + 3);
                    UTF8_CHAR_CHECK(char4)

                    int32_t t1 = (int32_t) (char1 & 0x07) << 18;
                    int32_t t2 = (int32_t) (char2 & 0x3F) << 12;
                    int32_t t3 = (int32_t) (char3 & 0x3F) << 6;
                    int32_t t4 = (int32_t) (char4 & 0x3F);
                    unicode = t1 | t2 | t3 | t4;
                    strStartPtr += 4;
                }
                else if (c <= (uint8_t) 0xFB && c >= (uint8_t) 0xF8){//五字节编码形式：111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                    if (strEndPtr - strStartPtr < 4){
                        break;
                    }
                    uint8_t char1 = *strStartPtr;
                    uint8_t char2 = *(strStartPtr + 1);
                    UTF8_CHAR_CHECK(char2)
                    uint8_t char3 = *(strStartPtr + 2);
                    UTF8_CHAR_CHECK(char3)
                    uint8_t char4 = *(strStartPtr + 3);
                    UTF8_CHAR_CHECK(char4)
                    uint8_t char5 = *(strStartPtr + 4);
                    UTF8_CHAR_CHECK(char5)

                    int32_t t1 = (int32_t) (char1 & 0x03) << 24;
                    int32_t t2 = (int32_t) (char2 & 0x3F) << 18;
                    int32_t t3 = (int32_t) (char3 & 0x3F) << 12;
                    int32_t t4 = (int32_t) (char4 & 0x3F) << 6;
                    int32_t t5 = (int32_t) (char5 & 0x3F);
                    unicode = t1 | t2 | t3 | t4 | t5;
                    strStartPtr += 5;
                }
                else if (c <= (uint8_t) 0xFD && c >= (uint8_t) 0xFC){//六字节编码形式：1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                    if (strEndPtr - strStartPtr < 5){
                        break;
                    }
                    uint8_t char1 = *strStartPtr;
                    uint8_t char2 = *(strStartPtr + 1);
                    UTF8_CHAR_CHECK(char2)
                    uint8_t char3 = *(strStartPtr + 2);
                    UTF8_CHAR_CHECK(char3)
                    uint8_t char4 = *(strStartPtr + 3);
                    UTF8_CHAR_CHECK(char4)
                    uint8_t char5 = *(strStartPtr + 4);
                    UTF8_CHAR_CHECK(char5)
                    uint8_t char6 = *(strStartPtr + 5);
                    UTF8_CHAR_CHECK(char6)

                    int32_t t1 = (int32_t) (char1 & 0x01) << 30;
                    int32_t t2 = (int32_t) (char2 & 0x3F) << 24;
                    int32_t t3 = (int32_t) (char3 & 0x3F) << 18;
                    int32_t t4 = (int32_t) (char4 & 0x3F) << 12;
                    int32_t t5 = (int32_t) (char5 & 0x3F) << 6;
                    int32_t t6 = (int32_t) (char6 & 0x3F);
                    unicode = t1 | t2 | t3 | t4 | t5 | t6;
                    strStartPtr += 6;
                }
                else{
                    if (strStartPtr > strEndPtr){
                        break;
                    }
                    strStartPtr++;
                    continue;
                }
                unicodes.push_back(unicode);
            }
            return 0;
        }

        //将unicode再转为utf8字符串
        int strUtils::unicodesToUTF8(const std::vector< int32_t > &unicodes, std::string &str){
            if (unicodes.empty()){
                return -1;
            }

            for (int unicode: unicodes){
                char tmpStrBuf[6] = {0};
                if (unicode <= 0x0000007F){//单字节:0xxxxxxx
                    *tmpStrBuf = (unicode & 0x7F);
                }
                else if (unicode >= 0x00000080 && unicode <= 0x000007FF){//双字节:110xxxxx 10xxxxxx
                    *(tmpStrBuf + 1) = (unicode & 0x3F) | 0x80;
                    *tmpStrBuf = ((unicode >> 6) & 0x1F) | 0xC0;
                }
                else if (unicode >= 0x00000800 && unicode <= 0x0000FFFF){//三字节:1110xxxx 10xxxxxx 10xxxxxx
                    *(tmpStrBuf + 2) = (unicode & 0x3F) | 0x80;
                    *(tmpStrBuf + 1) = ((unicode >> 6) & 0x3F) | 0x80;
                    *tmpStrBuf = ((unicode >> 12) & 0x0F) | 0xE0;
                }
                else if (unicode >= 0x00010000 && unicode <= 0x001FFFFF){//四字节:11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    *(tmpStrBuf + 3) = (unicode & 0x3F) | 0x80;
                    *(tmpStrBuf + 2) = ((unicode >> 6) & 0x3F) | 0x80;
                    *(tmpStrBuf + 1) = ((unicode >> 12) & 0x3F) | 0x80;
                    *tmpStrBuf = ((unicode >> 18) & 0x07) | 0xF0;
                }
                else if (unicode >= 0x00200000 && unicode <= 0x03FFFFFF){//五字节:111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                    *(tmpStrBuf + 4) = (unicode & 0x3F) | 0x80;
                    *(tmpStrBuf + 3) = ((unicode >> 6) & 0x3F) | 0x80;
                    *(tmpStrBuf + 2) = ((unicode >> 12) & 0x3F) | 0x80;
                    *(tmpStrBuf + 1) = ((unicode >> 18) & 0x3F) | 0x80;
                    *tmpStrBuf = ((unicode >> 24) & 0x03) | 0xF8;
                }
                else if (unicode >= 0x04000000 && unicode <= 0x7FFFFFFF){//六字节:1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                    *(tmpStrBuf + 5) = (unicode & 0x3F) | 0x80;
                    *(tmpStrBuf + 4) = ((unicode >> 6) & 0x3F) | 0x80;
                    *(tmpStrBuf + 3) = ((unicode >> 12) & 0x3F) | 0x80;
                    *(tmpStrBuf + 2) = ((unicode >> 18) & 0x3F) | 0x80;
                    *(tmpStrBuf + 1) = ((unicode >> 24) & 0x3F) | 0x80;
                    *tmpStrBuf = ((unicode >> 30) & 0x01) | 0xFC;
                }
                else{
                    continue;
                }
                str.append(tmpStrBuf);
            }
            return 0;
        }

        //将unicode再转为utf8字符串
        std::string strUtils::unicodesToUTF8(const std::vector< int32_t > &unicodes){
            std::string ret;
            unicodesToUTF8(unicodes, ret);
            return ret;
        }

        //单个unicode码转utf8
        int strUtils::unicodeToUTF8(const int32_t &unicode, std::string &str){
            std::vector< int32_t > tmpRune;
            tmpRune.push_back(unicode);
            return unicodesToUTF8(tmpRune, str);
        }

        //单个unicode码转utf8
        std::string strUtils::unicodeToUTF8(const int32_t &unicode){
            std::string ret;
            unicodeToUTF8(unicode, ret);
            return ret;
        }

        //单个utf8转unicode
        int strUtils::utf8ToUnicode(const std::string &str, int32_t &unicode){
            std::vector< int32_t > unicodes;
            utf8ToUnicodes(str, unicodes);
            if (unicodes.empty()){
                return -1;
            }
            unicode = unicodes[0];
            return 0;
        }

        //单个utf8转unicode
        int32_t strUtils::utf8ToUnicode(const std::string &str){
            int32_t unicode;
            utf8ToUnicode(str, unicode);
            return unicode;
        }

        //string 转为 wstring，要求 input 必须为utf8编码
        int strUtils::string2wstring(const std::string &input, std::wstring &output){
            std::vector< int32_t > unicodes;
            int ret = utf8ToUnicodes(input, unicodes);
            if (ret != 0){
                return ret;
            }
            for (int unicode: unicodes){
                output.append(1, (wchar_t) unicode);
            }
            return 0;
        }

        //string 转为 wstring，要求 input 必须为utf8编码
        std::wstring strUtils::string2wstring(const std::string &input){
            std::wstring ret;
            string2wstring(input, ret);
            return ret;
        }

        //wstring 转为 string，要求 input 必须为utf8编码
        int strUtils::wstring2string(const std::wstring &input, std::string &output){
            std::vector< int32_t > unicodes;
            for (wchar_t wch: input){
                unicodes.push_back((int32_t) wch);
            }
            return unicodesToUTF8(unicodes, output);
        }

        //wstring 转为 string，要求 input 必须为utf8编码
        std::string strUtils::wstring2string(const std::wstring &input){
            std::string ret;
            wstring2string(input, ret);
            return ret;
        }


        //base64 encode
        int strUtils::base64Encode(const std::string &in, std::string &out){
            size_t inlen = in.size();
            if (inlen <= 0){
                return -1;
            }

            const char *inStr = in.c_str();
            char c;
            for (; inlen >= 3; inlen -= 3, inStr += 3){
                c = ENCODE64((inStr[0] >> 2) & 0x3F);
                if (c == -1){
                    out.clear();
                    return -1;
                }
                out.push_back(c);
                c = ENCODE64(((inStr[0] << 4) & 0x30) | ((inStr[1] >> 4) & 0x0F));
                if (c == -1){
                    out.clear();
                    return -1;
                }
                out.push_back(c);
                c = ENCODE64(((inStr[1] << 2) & 0x3C) | ((inStr[2] >> 6) & 0x03));
                if (c == -1){
                    out.clear();
                    return -1;
                }
                out.push_back(c);
                c = ENCODE64(inStr[2] & 0x3F);
                if (c == -1){
                    out.clear();
                    return -1;
                }
                out.push_back(c);
            }

            if (inlen > 0){
                char fragment;
                c = ENCODE64((inStr[0] >> 2) & 0x3F);
                if (c == -1){
                    out.clear();
                    return -1;
                }
                out.push_back(c);
                fragment = (inStr[0] << 4) & 0x30;
                if (inlen > 1){
                    fragment |= (inStr[1] >> 4) & 0x0F;
                }
                c = ENCODE64(fragment);
                if (c == -1){
                    out.clear();
                    return -1;
                }
                out.push_back(c);
                if (inlen < 2){
                    out.push_back('=');
                }
                else{
                    c = ENCODE64((inStr[1] << 2) & 0x3C);
                    if (c == -1){
                        out.clear();
                        return -1;
                    }
                }
                out.push_back('=');
            }
            return 0;
        }

        //base64 decode
        int strUtils::base64Decode(const std::string &in, std::string &out){
            if (in.size() <= 0){
                return -1;
            }
            size_t inlen = in.size();
            const char *inStr = in.c_str();
            int over = 0;
            char digit0, digit1, digit2, digit3;

            // decode each four base64 characters
            for (; inlen >= 4; inlen -= 4, inStr += 4){
                if ((digit0 = DECODE64(inStr[0])) == -1){
                    return -1;
                }
                if ((digit1 = DECODE64(inStr[1])) == -1){
                    return -1;
                }
                out.push_back(((digit0 << 2) & 0xFC) | ((digit1 >> 4) & 0x03));

                if (inStr[2] != '='){
                    if ((digit2 = DECODE64(inStr[2])) == -1){
                        return -1;
                    }
                    out.push_back(((digit1 << 4) & 0xF0) | ((digit2 >> 2) & 0x0F));
                    if (inStr[3] != '='){
                        if ((digit3 = DECODE64(inStr[3])) == -1){
                            return -1;
                        }
                        out.push_back(((digit2 << 6) & 0xC0) | (digit3 & 0x3F));
                    }
                    else{
                        over = 1;
                        break;
                    }
                }
                else{
                    over = 1;
                    break;
                }
            }

            if (!over && inlen > 0){
                return -1;
            }

            return 0;
        }

        //url encode
        int strUtils::urlEncode(const std::string &in, std::string &out){
            size_t inlen = in.size();
            if (inlen <= 0){
                return -1;
            }
            unsigned char c;
            unsigned const char *inStart = (unsigned const char *) in.c_str();
            unsigned const char *inEnd = inStart + inlen;
            while (inStart < inEnd){
                c = *inStart++;
                if (c == ' '){
                    out.push_back('+');
                }
                else if ((c < '0' && c != '-' && c != '.') ||
                         (c < 'A' && c > '9') ||
                         (c > 'Z' && c < 'a' && c != '_') ||
                         (c > 'z')){
                    out.push_back('%');
                    out.push_back(hexchars[c >> 4]);
                    out.push_back(hexchars[c & 15]);
                }
                else{
                    out.push_back(c);
                }
            }
            return 0;
        }

        //url encode
        int strUtils::urlDecode(const std::string &in, std::string &out){
            size_t inlen = in.size();
            if (inlen <= 0){
                return -1;
            }

            int value;
            int c;
            const char *inStr = in.c_str();

            while (inlen--){
                if (*inStr == '+'){
                    out.push_back(' ');
                }
                else if (*inStr == '%' && inlen >= 2 && isxdigit((int) *(inStr + 1)) && isxdigit((int) *(inStr + 2))){
                    c = ((unsigned char *) (inStr + 1))[0];
                    if (isupper(c)){
                        c = tolower(c);
                    }
                    value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;
                    c = ((unsigned char *) (inStr + 1))[1];
                    if (isupper(c)){
                        c = tolower(c);
                    }
                    value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;
                    out.push_back((char) value);
                    inStr += 2;
                    inlen -= 2;
                }
                else{
                    out.push_back(*inStr);
                }
                inStr++;
            }
            return 0;
        }

        bool strUtils::safe_strtoull(const char *str, uint64_t *out){
            errno = 0;
            *out = 0;
            char *endptr;
            unsigned long long ull = strtoull(str, &endptr, 10);
            if ((errno == ERANGE) || (str == endptr)){
                return false;
            }
            if (xisspace(*endptr) || (*endptr == '\0' && endptr != str)){
                if ((long long) ull < 0){
                    if (strchr(str, '-') != nullptr){
                        return false;
                    }
                }
                *out = ull;
                return true;
            }
            return false;
        }

        uint64_t strUtils::safe_strtoull(const char *str){
            uint64_t ret;
            safe_strtoull(str, &ret);
            return ret;
        }

        bool strUtils::safe_strtoull_hex(const char *str, uint64_t *out){
            errno = 0;
            *out = 0;
            char *endptr;
            unsigned long long ull = strtoull(str, &endptr, 16);
            if ((errno == ERANGE) || (str == endptr)){
                return false;
            }
            if (xisspace(*endptr) || (*endptr == '\0' && endptr != str)){
                if ((long long) ull < 0){
                    if (strchr(str, '-') != nullptr){
                        return false;
                    }
                }
                *out = ull;
                return true;
            }
            return false;
        }

        uint64_t strUtils::safe_strtoull_hex(const char *str){
            uint64_t ret;
            safe_strtoull_hex(str, &ret);
            return ret;
        }

        bool strUtils::safe_strtoll(const char *str, int64_t *out){
            errno = 0;
            *out = 0;
            char *endptr;
            long long ll = strtoll(str, &endptr, 10);
            if ((errno == ERANGE) || (str == endptr)){
                return false;
            }
            if (xisspace(*endptr) || (*endptr == '\0' && endptr != str)){
                *out = ll;
                return true;
            }
            return false;
        }

        int64_t strUtils::safe_strtoll(const char *str){
            int64_t ret;
            safe_strtoll(str, &ret);
            return ret;
        }

        bool strUtils::safe_strtoul(const char *str, uint32_t *out){
            char *endptr = nullptr;
            unsigned long l = 0;
            *out = 0;
            errno = 0;

            l = strtoul(str, &endptr, 10);
            if ((errno == ERANGE) || (str == endptr)){
                return false;
            }
            if (xisspace(*endptr) || (*endptr == '\0' && endptr != str)){
                if ((long) l < 0){
                    if (strchr(str, '-') != nullptr){
                        return false;
                    }
                }
                *out = l;
                return true;
            }

            return false;
        }

        uint32_t strUtils::safe_strtoul(const char *str){
            uint32_t ret;
            safe_strtoul(str, &ret);
            return ret;
        }

        bool strUtils::safe_strtol(const char *str, int32_t *out){
            errno = 0;
            *out = 0;
            char *endptr;
            long l = strtol(str, &endptr, 10);
            if ((errno == ERANGE) || (str == endptr)){
                return false;
            }
            if (xisspace(*endptr) || (*endptr == '\0' && endptr != str)){
                *out = l;
                return true;
            }
            return false;
        }

        int32_t strUtils::safe_strtol(const char *str){
            int32_t ret;
            safe_strtol(str, &ret);
            return ret;
        }

        bool strUtils::safe_strtod(const char *str, double *out){
            errno = 0;
            *out = 0;
            char *endptr;
            double d = strtod(str, &endptr);
            if ((errno == ERANGE) || (str == endptr)){
                return false;
            }
            if (xisspace(*endptr) || (*endptr == '\0' && endptr != str)){
                *out = d;
                return true;
            }
            return false;
        }

        double strUtils::safe_strtod(const char *str){
            double ret;
            safe_strtod(str, &ret);
            return ret;
        }
    }
}
