/*************************************************************************
 * File:	color_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:58
 ************************************************************************/
#ifndef _PFSM_LOGSIM_COLOR_UTILS_H_202306271158_
#define _PFSM_LOGSIM_COLOR_UTILS_H_202306271158_

#include<string>
#include<vector>
#include<map>
#include "str_utils.h"

namespace haomo{
    namespace transtopic{
        class colorUtils{
        public:
            static void Green(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string Green(const std::string &inStr, int blink = 0, int underline = 0);
            static void LightGreen(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string LightGreen(const std::string &inStr, int blink = 0, int underline = 0);
            static void Cyan(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string Cyan(const std::string &inStr, int blink = 0, int underline = 0);
            static void LightCyan(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string LightCyan(const std::string &inStr, int blink = 0, int underline = 0);
            static void Red(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string Red(const std::string &inStr, int blink = 0, int underline = 0);
            static void LightRed(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string LightRed(const std::string &inStr, int blink = 0, int underline = 0);
            static void Yellow(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string Yellow(const std::string &inStr, int blink = 0, int underline = 0);
            static void Black(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string Black(const std::string &inStr, int blink = 0, int underline = 0);
            static void DarkGray(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string DarkGray(const std::string &inStr, int blink = 0, int underline = 0);
            static void LightGray(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string LightGray(const std::string &inStr, int blink = 0, int underline = 0);
            static void White(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string White(const std::string &inStr, int blink = 0, int underline = 0);
            static void Blue(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string Blue(const std::string &inStr, int blink = 0, int underline = 0);
            static void LightBlue(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string LightBlue(const std::string &inStr, int blink = 0, int underline = 0);
            static void Purple(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string Purple(const std::string &inStr, int blink = 0, int underline = 0);
            static void LightPurple(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string LightPurple(const std::string &inStr, int blink = 0, int underline = 0);
            static void Brown(const std::string &inStr, std::string &outStr, int blink = 0, int underline = 0);
            static std::string Brown(const std::string &inStr, int blink = 0, int underline = 0);
            static void cliColorRender(const std::string &in, std::string &out, ColorType color, int weight = 0, int blink = 0, int underline = 0);
            static void println(const std::string &in, ColorType color, int weight = 0, int blink = 0, int underline = 0);

        private:
            colorUtils() = delete;
            colorUtils(const colorUtils &) = delete;
            colorUtils &operator=(const colorUtils &) = delete;
        };
    }
} //namespace Project


#endif //_PFSM_LOGSIM_COLOR_UTILS_H_202306271158_
