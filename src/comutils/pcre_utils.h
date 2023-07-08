/*************************************************************************
 * File:	pcre_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 19:41
 ************************************************************************/
#ifndef _PFSM_LOGSIM_PCRE_UTILS_H_202306271941_
#define _PFSM_LOGSIM_PCRE_UTILS_H_202306271941_

#include<string>
#include<vector>
#include<map>
#include<pcre.h>
#include "str_utils.h"

namespace haomo{
    namespace transtopic{
        class pcreUtils{
        public:
            pcreUtils(const std::string &pattern){
                this->pattern = pattern;
                this->re = nullptr;
            };

            //内部使用的匹配方法，这里的返回值是双层vector，考虑到会有多个捕获组的情况
            //外层的vector是匹配次数，内层的是一次匹配命中的所有的子串
            //matchLimit=-1表示不限制匹配次数
            int reg_match_general(
                    const std::string &str,
                    size_t matchLimit,
                    std::vector <std::vector< RegSubMatch >> &output
            );

            //找出所有的子串：成功返回0，失败返回负数
            //内层的vector，第一个为整个匹配串，其他的表示捕获子串
            int reg_match_all(const std::string &str, std::vector <std::vector< std::string >> &subList);

            //用正则表达式切字符串：成功返回0，失败返回负数
            //忽略捕获组
            int reg_split(const std::string &str, std::vector <std::string> &subList);

            //是否跟当前字符串匹配：成功返回0，失败返回负数
            //忽略捕获组
            int reg_match(const std::string &str);

            //正则替换：成功返回0，失败返回负数
            int reg_replace(const std::string &str, const std::string &replace, std::string &newStr);

            //高仿PHP的 preg_replace_callback() 函数，利用回调函数替换字符串，生成新的字符串
            //成功返回0，失败返回负数
            int reg_replace_callback(
                    //要替换的字符串
                    const std::string &originStr,
                    //替换后的字符串
                    std::string &newStr,
                    //回调函数：0表示整个匹配的字符串，1表示正则里的第一个捕获项、2表示第二个、依次类推。。。。
                    void (*callbackFunc)(
                            const std::vector <std::string> &subMatchStrs,
                            std::string &replaceStr//返回的要替换匹配上的字符串
                    )
            );

            ~pcreUtils();

        private:
            std::string pattern; //正则表达式
            pcre *re;

            void init();
            pcreUtils(){};
        };
    }
} //namespace Project


#endif //_PFSM_LOGSIM_PCRE_UTILS_H_202306271941_