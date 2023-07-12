/*************************************************************************
 * File:	rune_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:56
 ************************************************************************/
#ifndef _PFSM_LOGSIM_RUNE_UTILS_H_202306271156_
#define _PFSM_LOGSIM_RUNE_UTILS_H_202306271156_

#include<string>
#include<vector>
#include<map>
#include "comutils/str_utils.h"

namespace lysutil{
    namespace comutils{
        class runeUtils{
        public:
            //单个字符宽度
            static size_t runeWidth(int32_t rune);

            //一串字符串宽度
            static size_t runeStringWidth(const std::string &str);

            //在右边补齐到指定宽度
            static void runeFillRight(const std::string &str, size_t width, std::string &output);

            //在左边补齐到指定宽度
            static void runeFillLeft(const std::string &str, size_t width, std::string &output);

            //将一段字符串折成多行，每行的宽度最多为width
            static void runeWrap(const std::string &str, size_t width, std::string &output, size_t &lineNum);

        private:
            runeUtils() = delete;
            runeUtils(const runeUtils &) = delete;
            runeUtils &operator=(const runeUtils &) = delete;

            static runeInterval privateTable[];
            static size_t privateTableLen;

            static runeInterval nonPrintTable[];
            static size_t nonPrintTableLen;

            static runeInterval combiningTable[];
            static size_t combiningTableLen;

            static runeInterval doubleWidthTable[];
            static size_t doubleWidthTableLen;

            static runeInterval ambiguousTable[];
            static size_t ambiguousTableLen;

            static runeInterval emojiTable[];
            static size_t emojiTableLen;

            static runeInterval notAssignedTable[];
            static size_t notAssignedTableLen;

            static runeInterval neutralTable[];
            static size_t neutralTableLen;

            static int isEastAsianLocale;

            static int isRuneInTable(int32_t rune, runeInterval *table, size_t tableLen);
            static int isRuneInTables(int32_t rune, int count, ...);

            //是否为东亚语系
            static void getIsEastAsianLocale();
        };
    }
} //namespace Project


#endif //_PFSM_LOGSIM_RUNE_UTILS_H_202306271156_
