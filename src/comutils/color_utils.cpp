/*************************************************************************
 * File:	color_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:58
 ************************************************************************/
#include<iostream>
#include "color_utils.h"

namespace haomo{
    namespace transtopic{
        void colorUtils::Green(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_GREEN, 0, blink, underline);
        }

        std::string colorUtils::Green(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_GREEN, 0, blink, underline);
            return outStr;
        }

        void colorUtils::LightGreen(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_LIGHTGREEN, 0, blink, underline);
        }

        std::string colorUtils::LightGreen(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_LIGHTGREEN, 0, blink, underline);
            return outStr;
        }

        void colorUtils::Cyan(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_CYAN, 0, blink, underline);
        }

        std::string colorUtils::Cyan(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_CYAN, 0, blink, underline);
            return outStr;
        }

        void colorUtils::LightCyan(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_LIGHTCYAN, 0, blink, underline);
        }

        std::string colorUtils::LightCyan(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_LIGHTCYAN, 0, blink, underline);
            return outStr;
        }

        void colorUtils::Red(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_RED, 0, blink, underline);
        }

        std::string colorUtils::Red(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_RED, 0, blink, underline);
            return outStr;
        }

        void colorUtils::LightRed(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_LIGHTRED, 0, blink, underline);
        }

        std::string colorUtils::LightRed(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_LIGHTRED, 0, blink, underline);
            return outStr;
        }

        void colorUtils::Yellow(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_YELLOW, 0, blink, underline);
        }

        std::string colorUtils::Yellow(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_YELLOW, 0, blink, underline);
            return outStr;
        }

        void colorUtils::Black(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_BLACK, 0, blink, underline);
        }

        std::string colorUtils::Black(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_BLACK, 0, blink, underline);
            return outStr;
        }

        void colorUtils::DarkGray(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_DARKGRAY, 0, blink, underline);
        }

        std::string colorUtils::DarkGray(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_DARKGRAY, 0, blink, underline);
            return outStr;
        }

        void colorUtils::LightGray(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_LIGHTGRAY, 0, blink, underline);
        }

        std::string colorUtils::LightGray(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_LIGHTGRAY, 0, blink, underline);
            return outStr;
        }

        void colorUtils::White(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_WHITE, 0, blink, underline);
        }

        std::string colorUtils::White(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_WHITE, 0, blink, underline);
            return outStr;
        }

        void colorUtils::Blue(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_BLUE, 0, blink, underline);
        }

        std::string colorUtils::Blue(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_BLUE, 0, blink, underline);
            return outStr;
        }

        void colorUtils::LightBlue(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_LIGHTBLUE, 0, blink, underline);
        }

        std::string colorUtils::LightBlue(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_LIGHTBLUE, 0, blink, underline);
            return outStr;
        }

        void colorUtils::Purple(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_PURPLE, 0, blink, underline);
        }

        std::string colorUtils::Purple(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_PURPLE, 0, blink, underline);
            return outStr;
        }

        void colorUtils::LightPurple(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_LIGHTPURPLE, 0, blink, underline);
        }

        std::string colorUtils::LightPurple(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_LIGHTPURPLE, 0, blink, underline);
            return outStr;
        }

        void colorUtils::Brown(const std::string &inStr, std::string &outStr, int blink, int underline){
            cliColorRender(inStr, outStr, COLOR_BROWN, 0, blink, underline);
        }

        std::string colorUtils::Brown(const std::string &inStr, int blink, int underline){
            std::string outStr;
            cliColorRender(inStr, outStr, COLOR_BROWN, 0, blink, underline);
            return outStr;
        }

        void colorUtils::cliColorRender(const std::string &in, std::string &out, ColorType color, int weight, int blink, int underline){
            std::string modifier;
            if (blink > 0){
                modifier.append("05;");
            }
            if (underline > 0){
                modifier.append("04;");
            }
            if (weight > 0){
                char tmpWeight[8] = {0};
                sprintf(tmpWeight, "%d;", weight);
                modifier.append(tmpWeight);
            }
            if (modifier.size() <= 0){
                modifier.append("0;");
            }
            char tmpColor[8] = {0};
            sprintf(tmpColor, "%d", color);
            size_t sz = sizeof(char) * (in.size() * 2 + 64);
            char *tmpOutput = (char *) malloc(sz);
            bzero(tmpOutput, sz);
            sprintf(tmpOutput, "\033[%s%sm%s\033[0m", modifier.c_str(), tmpColor, in.c_str());
            out.append(tmpOutput);
            free(tmpOutput);
        }

        void colorUtils::println(const std::string &in, ColorType color, int weight, int blink, int underline){
            std::string out;
            cliColorRender(in, out, color, weight, blink, underline);
            std::cout << out << std::endl;
        }
    }
}
