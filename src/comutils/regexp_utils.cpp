/*************************************************************************
 * File:	regexp_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:57
 ************************************************************************/
#include<iostream>
#include "regexp_utils.h"

namespace haomo{
    namespace transtopic{
        //找出所有的子串：成功返回0，失败返回负数
        //内层的vector，第一个为整个匹配串，其他的表示捕获子串
        int regexpUtils::reg_match_all(const std::string &pattern,
                                       const std::string &str,
                                       std::vector <std::vector< std::string >> &subList){
            return 0;
        }

        //用正则表达式切字符串：成功返回0，失败返回负数
        //忽略捕获组
        int regexpUtils::reg_split(const std::string &pattern,
                                   const std::string &str,
                                   std::vector <std::string> &subList){
            return 0;
        }

        //是否跟当前字符串匹配：成功返回0，失败返回负数
        //忽略捕获组
        int regexpUtils::reg_match(const std::string &pattern, const std::string &str){
            return 0;
        }

        //正则替换：成功返回0，失败返回负数
        int regexpUtils::reg_replace(const std::string &pattern,
                                     const std::string &str,
                                     const std::string &replace,
                                     std::string &newStr){
            return 0;
        }

        //高仿PHP的 preg_replace_callback() 函数，利用回调函数替换字符串，生成新的字符串
        //成功返回0，失败返回负数
        int regexpUtils::reg_replace_callback(const std::string &pattern,
                                              const std::string &originStr,//要替换的字符串
                                              std::string &newStr,//替换后的字符串
                                              void (*callbackFunc)(//回调函数：0表示整个匹配的字符串，1表示正则里的第一个捕获项、2表示第二个、依次类推。。。。
                                                      const std::vector <std::string> &subMatchStrs,
                                                      std::string &replaceStr
                                              )){
            return 0;
        }

        //内部使用的匹配方法，这里的返回值是双层vector，考虑到会有多个捕获组的情况
        //外层的vector是匹配次数，内层的是一次匹配命中的所有的子串
        //matchLimit=-1表示不限制匹配次数
        int regexpUtils::reg_match_general(const std::string &str,
                                           size_t matchLimit,
                                           std::vector <std::vector< RegSubMatch >> &output){
            return 0;
        }
    }
}
