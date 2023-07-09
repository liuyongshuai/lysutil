/*************************************************************************
 * File:	sys_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:58
 ************************************************************************/
#include<iostream>
#include "sys_utils.h"

namespace lysutil{
    namespace comutils{
        //对整数向上圆整为2^n倍数
        size_t sysUtils::up2n(size_t num){
            if (num <= 2){
                return 2;
            }
            size_t n = ceil(log((double) num) / log(2.0));
            size_t ret = pow(2.0, n);
            return ret;
        }

        //获取本机IP地址
        void sysUtils::getLocalIP(std::vector <std::string> &ips, std::string &hostName){
            char hname[256] = {0};
            int ret = gethostname(hname, sizeof(hname));
            if (ret == -1){
                return;
            }
            hostName.assign(hname, sizeof(hname));
            struct hostent *hent;
            hent = gethostbyname(hname);
            if (nullptr == hent){
                return;
            }
            int i;
            std::string ipStr;
            for (i = 0; hent->h_addr_list[i]; i++){
                uint32_t ipInt = ntohl(((struct in_addr *) hent->h_addr_list[i])->s_addr);
                char buf[32] = {0};
                memset(buf, 0, 32);
                sprintf(buf, "%d.%d.%d.%d", ipInt >> 24, ipInt << 8 >> 24, ipInt << 16 >> 24, ipInt << 24 >> 24);
                ipStr = buf;
                if (ipStr.empty()){
                    continue;
                }
                std::vector <std::string> tmp;
                strUtils::strSplit(ipStr, '.', tmp);
                if (tmp.size() != 4){
                    continue;
                }
                if (tmp[0] == "127"){
                    continue;
                }
                ips.push_back(ipStr);
            }
        }

        //获取从设备读取的随机字节
        void sysUtils::genDeviceRandomBytes(char *buf, size_t len){
            FILE *fp = fopen("/dev/urandom", "r");
            if (fp == nullptr){
                perror("open file failed");
                return;
            }
            if (fgets(buf, len, fp) == nullptr){
                perror("read file failed");
            }
            fclose(fp);
        }

        //从设备读取随机字符串
        void sysUtils::getDeviceRandom(std::string &random, size_t limit){
            size_t cnum = sizeof(char) * limit * 4;
            char *buf = (char *) malloc(cnum);
            bzero(buf, cnum);
            genDeviceRandomBytes(buf, cnum);
            size_t i;
            uint32_t tmpI;
            char randBuf[32] = {0};
            char *tmpIBuf = (char *) &tmpI;
            std::string tmpOutPut;
            for (i = 0; i < cnum; i++){
                memset(randBuf, 0, 32);
                tmpIBuf[0] = *(buf + i * 4);
                tmpIBuf[1] = *(buf + i * 4 + 1);
                tmpIBuf[2] = *(buf + i * 4 + 2);
                tmpIBuf[3] = *(buf + i * 4 + 3);
                sprintf(randBuf, "%x", tmpI);
                tmpOutPut.append(randBuf);
                if (tmpOutPut.size() >= limit){
                    break;
                }
            }
            free(buf);
            std::string tmpRandom(tmpOutPut, 0, limit);
            random = tmpRandom;
        }

        //产生一个 0至RAND_MAX 之间的随机数
        int sysUtils::getRandom(){
            char buf[5] = {0};
            genDeviceRandomBytes(buf, 5);
            uint32_t seed;
            char *tmp = (char *) &seed;
            *tmp = buf[0];
            *(tmp + 1) = buf[1];
            *(tmp + 2) = buf[2];
            *(tmp + 3) = buf[3];
            srand(seed);
            int r = rand();
            return r;
        }

        //当前的微秒数
        void sysUtils::getMicroSeconds(uint64_t &ms){
            struct timeval tv;
            int r = gettimeofday(&tv, nullptr);
            if (r != 0){
                perror("gettimeofday failed");
                return;
            }
            ms = 1000000 * (uint64_t) tv.tv_sec + (uint64_t) tv.tv_usec;
        }

        //当前的纳秒数
        void sysUtils::getNanoSeconds(uint64_t &ns){
#ifdef __linux__
            pid_t pid = getpid();
            clockid_t clk_id;
            int r = clock_getcpuclockid(pid, &clk_id);
            if (r != 0){
                perror("clock_getcpuclockid failed");
                return;
            }
            struct timespec ts;
            r = clock_gettime(clk_id, &ts);
            if (r != 0){
                perror("clock_gettime failed");
                return;
            }
            time_t ct = time(nullptr);
            ns = 1000000000 * (uint64_t) ct + (uint64_t) ts.tv_nsec;
#else
            uint64_t ms;
            getMicroSeconds(ms);
            ns = ms * 1000;
#endif
        }

        //获取屏幕终端的宽高
        void sysUtils::getTerminalSize(size_t *width, size_t *height){
            if (width != nullptr){
                *width = 0;
            }
            if (height != nullptr){
                *height = 0;
            }
            struct winsize sz;
            int r = ioctl(0, TIOCGWINSZ, &sz);
            if (r != 0){
                perror("ioctl failed");
                return;
            }
            if (width != nullptr){
                *width = sz.ws_col;
            }
            if (height != nullptr){
                *height = sz.ws_row;
            }
        }
    }
}
