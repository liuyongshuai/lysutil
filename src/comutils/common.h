/*************************************************************************
 * File:	common.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 12:44
 ************************************************************************/
#ifndef _PFSM_LOGSIM_COMMON_H_202306271244_
#define _PFSM_LOGSIM_COMMON_H_202306271244_

#include <time.h>
#include <string>
#include <regex.h>
#include <stdint.h>
#include <sys/time.h>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_set>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>

#define xisspace(c) isspace((unsigned char)c)

namespace lysutil{
    namespace comutils{
        //颜色用的方法
        typedef void (*ColorFunc)(const std::string &, std::string &, int, int);

        typedef int ColorType;
        const static ColorType COLOR_GREEN = 32;//绿色字体
        const static ColorType COLOR_LIGHTGREEN = 32;//淡绿
        const static ColorType COLOR_CYAN = 36;//青色/蓝绿色
        const static ColorType COLOR_LIGHTCYAN = 36;//淡青色
        const static ColorType COLOR_RED = 31;//红字体
        const static ColorType COLOR_LIGHTRED = 31;//淡红色
        const static ColorType COLOR_YELLOW = 33;//黄色字体
        const static ColorType COLOR_BLACK = 30;//黑色
        const static ColorType COLOR_DARKGRAY = 30;//深灰色
        const static ColorType COLOR_LIGHTGRAY = 37;//浅灰色
        const static ColorType COLOR_WHITE = 37;//白色
        const static ColorType COLOR_BLUE = 34;//蓝色
        const static ColorType COLOR_LIGHTBLUE = 34;//淡蓝
        const static ColorType COLOR_PURPLE = 35;//紫色
        const static ColorType COLOR_LIGHTPURPLE = 35;//淡紫色
        const static ColorType COLOR_BROWN = 33;//棕色

        typedef struct _runeInterval{
            int32_t first;
            int32_t last;
        } runeInterval;

        typedef struct _regExpSubMatch{
            size_t start_pos;
            std::string sub_str;
        } RegSubMatch;

        //一个小单元格的数据
        typedef struct _table_row_cell{
            size_t columnNo;                    //第几列
            size_t maxWidth;                    //允许的宽度
            std::vector <std::string> cellStrs;           //本小格子的数据，可能要分多行
        } tableRowCell;

        //一行的数据及处理逻辑
        typedef struct _table_row{
            int rowType;                        //本行数据的类型，是表头还是数据内容，0为表头，1为行数据
            size_t lineNum;                     //本行数据的行数
            std::vector <tableRowCell> cellList;     //本行所有小格子的数据
        } tableRow;

        //小格子宽度排序用的
        typedef struct _table_row_width{
            size_t columnNo;                    //第几列
            size_t allowWidth;                   //允许的宽度
        } tableRowWidth;

        enum TIME_UNIT{
            SECOND,
            MILLISECOND,
            MICROSECOND,
            NANOSECOND
        };

        inline std::string Now(const std::string &format){
            time_t tv = time(nullptr);
            struct tm *tmptr = localtime(&tv);
            char buf[64];
            if (strftime(buf, 64, format.c_str(), tmptr) == 0){
                return "";
            }
            return std::string(buf);
        }


        inline int64_t Now(TIME_UNIT unit){
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            int64_t timestamp = 0;
            switch (unit){
                case SECOND:
                    timestamp = ts.tv_sec;
                    break;
                case MILLISECOND:
                    timestamp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
                    break;
                case MICROSECOND:
                    timestamp = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
                    break;
                case NANOSECOND:
                    timestamp = ts.tv_sec * 1000000000 + ts.tv_nsec;
                    break;
            }
            return timestamp;
        }

        inline int64_t Now(){
            return Now(MILLISECOND);
        }

        // Cal elapsed time
        class Timer{
        public:
            Timer(TIME_UNIT unit = MILLISECOND) : unit_(unit){
                create_time_ = Now(unit_);
                start_time_ = create_time_;
            }

            void Start(){
                start_time_ = Now(unit_);
            }

            int64_t Elapsed(){
                int64_t now = Now(unit_);
                int64_t tmp = now - start_time_;
                start_time_ = now;
                return tmp;
            }

            int64_t LifeElapsed(){
                return Now(unit_) - create_time_;
            }

        private:
            //
            TIME_UNIT unit_;

            // time of creating this 'Timer'
            int64_t create_time_;

            // time of starting this 'Timer'
            int64_t start_time_;

        };
    }
}
#endif //_PFSM_LOGSIM_COMMON_H_202306271244_
