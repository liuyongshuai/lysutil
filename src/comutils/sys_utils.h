/*************************************************************************
 * File:	sys_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:58
 ************************************************************************/
#ifndef _PFSM_LOGSIM_SYS_UTILS_H_202306271158_
#define _PFSM_LOGSIM_SYS_UTILS_H_202306271158_

#include<string>
#include<vector>
#include<map>
#include "comutils/str_utils.h"

namespace lysutil{
    namespace comutils{
        class sysUtils{
        public:
            //获取屏幕终端的宽高
            static void getTerminalSize(size_t *width, size_t *height);

            //随机数
            static void genDeviceRandomBytes(char *buf, size_t len);
            static void getDeviceRandom(std::string &random, size_t limit);
            static int getRandom();
            static size_t up2n(size_t num);

            //当前的微秒数
            static void getMicroSeconds(uint64_t &ms);

            //当前的纳秒数
            static void getNanoSeconds(uint64_t &ns);

            //获取本机IP地址
            static void getLocalIP(std::vector<std::string> &ips, std::string &hostname);
        private:
            sysUtils() = delete;
            sysUtils(const sysUtils &) = delete;
            sysUtils &operator=(const sysUtils &) = delete;
        };
    }
} //namespace Project


#endif //_PFSM_LOGSIM_SYS_UTILS_H_202306271158_
