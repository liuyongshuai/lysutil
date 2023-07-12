/*************************************************************************
 * File:	str_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:55
 ************************************************************************/
#ifndef _PFSM_LOGSIM_STR_UTILS_H_202306271155_
#define _PFSM_LOGSIM_STR_UTILS_H_202306271155_

#include<string>
#include<vector>
#include<map>
#include "comutils/md5_utils.h"
#include "comutils/common.h"

namespace lysutil{
    namespace comutils{
        class strUtils{
        public:
            //只保留str中的汉字、字母、数字，其他的一律替换为空格
            static void keepNormChar(std::string &str);

            //str里是否包含中文
            static bool isContainChinese(const std::string &str);

            //是否为常用的中文汉字
            static bool isCnChar(const wchar_t &unicode);

            //是否为全角半角大小写字母
            static bool isEnLowerLetter(const wchar_t &unicode);
            static bool isEnUpperLetter(const wchar_t &unicode);
            static bool isCnLowerLetter(const wchar_t &unicode);
            static bool isCnUpperLetter(const wchar_t &unicode);

            //是否为全角、半角阿拉伯数字
            static bool isCnNumber(const wchar_t &unicode);
            static bool isEnNumber(const wchar_t &unicode);
            static bool isAllNumber(const std::string& str);

            //去掉两端、左端、右端的空格
            static void trimSpace(std::string &str);
            static void trimLeftSpace(std::string &str);
            static void trimRightSpace(std::string &str);

            //去掉两端、左端、右端的Char
            static void trimChar(std::string &str, char c);
            static void trimLeftChar(std::string &str, char c);
            static void trimRightChar(std::string &str, char c);

            //md5加密
            static std::string md5sum(const std::string &str);

            //vec1是否完全包含vec2
            static bool isVecContainAll(const std::vector< std::string > &vec1, const std::vector< std::string > &vec2);

            //字符串的切割、连接替换
            static void strSplit(const std::string &in, char c, std::vector< std::string > &out);
            static void strJoin(const std::vector< std::string > &in, const std::string &j, std::string &out);
            static void strJoin(const std::set< std::string > &in, const std::string &j, std::string &out);
            static std::string strJoin(const std::vector< std::string > &in, const std::string &j);
            static void strReplace(const std::string &in, const std::string &replace, const std::string &newStr, std::string &out);

            //是否以某个字符串开头、结尾
            static bool hasPrefix(const std::string &str, const std::string &substr);
            static bool hasSuffix(const std::string &str, const std::string &substr);

            //转为大小写
            static void toUpper(std::string &str);
            static void toLower(std::string &str);

            //url encode
            static int urlEncode(const std::string &in, std::string &out);
            static int urlDecode(const std::string &in, std::string &out);

            //base64 encode
            static int base64Encode(const std::string &in, std::string &out);
            static int base64Decode(const std::string &in, std::string &out);

            //多个unicode、utf8互转换，碰到残疾的字符会直接返回
            static int utf8ToUnicodes(const std::string &str, std::vector< int32_t > &unicodes);
            static int unicodesToUTF8(const std::vector< int32_t > &unicodes, std::string &str);
            static std::string unicodesToUTF8(const std::vector< int32_t > &unicodes);

            //单个unicode、utf8互转换
            static int unicodeToUTF8(const int32_t &unicode, std::string &str);
            static std::string unicodeToUTF8(const int32_t &unicode);
            static int utf8ToUnicode(const std::string &str, int32_t &unicode);
            static int32_t utf8ToUnicode(const std::string &str);

            /**
             * string 转为 wstring，要求 input 必须为utf8编码，不依赖locale
             * wstring 转为 string，要求 input 必须为utf8编码，不依赖locale
             * 其实在linux下wchar_t为4个字节，存的是unicode码，直接解析utf8字符串为unicode即可
             */
            static int string2wstring(const std::string &input, std::wstring &output);
            static std::wstring string2wstring(const std::string &input);
            static int wstring2string(const std::wstring &input, std::string &output);
            static std::string wstring2string(const std::wstring &input);


            //从memcached扒出来的
            static bool safe_strtoull(const char *str, uint64_t *out);
            static uint64_t safe_strtoull(const char *str);
            static bool safe_strtoull_hex(const char *str, uint64_t *out);
            static uint64_t safe_strtoull_hex(const char *str);
            static bool safe_strtoll(const char *str, int64_t *out);
            static int64_t safe_strtoll(const char *str);
            static bool safe_strtoul(const char *str, uint32_t *out);
            static uint32_t safe_strtoul(const char *str);
            static bool safe_strtol(const char *str, int32_t *out);
            static int32_t safe_strtol(const char *str);
            static bool safe_strtod(const char *str, double *out);
            static double safe_strtod(const char *str);

        private:
            strUtils() = delete;
            strUtils(const strUtils &) = delete;
            strUtils &operator=(const strUtils &) = delete;

            static char base64digits[];
            static char base64val[];
            static unsigned char hexchars[];
            //基本常用汉字的unicode范围
            static int32_t chineseCharUnicode[2];
            //半角阿拉伯数字0~9范围
            static int32_t enNumberCharUnicode[2];
            //半角大写字母的unicode范围
            static int32_t enUpperLetterCharUnicode[2];
            //半角小写字母的unicode范围
            static int32_t enLowerLetterCharUnicode[2];
            //全角阿拉伯数字0~9范围
            static int32_t cnNumberCharUnicode[2];
            //全角大写字母的unicode范围
            static int32_t cnUpperLetterCharUnicode[2];
            //全角小写字母的unicode范围
            static int32_t cnLowerLetterCharUnicode[2];
        };
    }
} //namespace Project


#endif //_PFSM_LOGSIM_STR_UTILS_H_202306271155_
