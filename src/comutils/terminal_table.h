/*************************************************************************
 * File:	terminal_table.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:58
 ************************************************************************/
#ifndef _PFSM_LOGSIM_TERMINAL_TABLE_H_202306271158_
#define _PFSM_LOGSIM_TERMINAL_TABLE_H_202306271158_

#include<string>
#include<vector>
#include<map>
#include "str_utils.h"
#include "color_utils.h"
#include "sys_utils.h"
#include "rune_utils.h"
#include "pcre_utils.h"

namespace haomo{
    namespace transtopic{

        class terminalTable{
        public:
            terminalTable();

            //添加表头数据
            void addHeadData(const std::vector <std::string> &headData);

            void addHeadData(int num, ...);

            //添加行数据
            void addRowData(const std::vector <std::string> &rowData);

            void addRowData(int num, ...);

            //输出最终的结果
            void renderToString(std::string &table);

            //设置相关的颜色：表头的字体颜色，默认Yellow
            void setHeaderFontColorType(ColorType t);

            //设置相关的颜色：表格内容的字体颜色
            void setRowFontColorType(ColorType t);

            //设置相关的颜色：边框的颜色
            void setBorderColorType(ColorType t);

        private:
            std::vector <std::string> rawheadData;            //原始的表头数据
            std::vector <std::vector< std::string >> rawRowData;    //原始的行数据信
            std::vector <size_t> rawRowColNums;          //各行的原始列数
            size_t maxColumnNum;                    //列的数量，以最多的一行的列为准
            std::vector <size_t> maxColumnWidth;         //每列的最大宽度，对齐用的
            size_t allTableAllowWidth;              //表格允许的最大宽度，根据屏幕及总列数计算出来的
            std::vector <tableRow> rowData;              //所有经过处理后的行，包括表头数据

            ColorType headerFontColorType; //表头的字体颜色，默认Yellow
            ColorType rowFontColorType;     //表格内容的字体颜色
            ColorType borderColorType;      //边框的颜色

            //整理各列的宽度信息等
            void reProcessTableWidth();

            //给各行的数据折行处理
            void wrapTableRows(tableRow &row, std::vector <std::string> &rowStr);

            //重新处理各行的数据
            void reProcessTableRowData();

            //渲染一行的数据
            void renderSingleRow(tableRow &row, std::string &output);

            void quickSortTableCellWidth(std::vector< tableRowWidth * > &rowWidths, int left, int right);

            void genHeaderStrWidthColor(std::string &str);

            void genBorderStrWidthColor(std::string &str);

            void genRowStrWidthColor(std::string &str);
        };
    }
} //namespace Project


#endif //_PFSM_LOGSIM_TERMINAL_TABLE_H_202306271158_
