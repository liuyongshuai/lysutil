/*************************************************************************
 * File:	terminal_table.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:58
 ************************************************************************/
#include<iostream>
#include "comutils/terminal_table.h"

namespace lysutil{
    namespace comutils{
        static const char *replaceStr[] = {
                "“", "\"",
                "”", "\"",
                "\t", "    ",
                "…", "...",
                "—", "-"
        };

        //替换掉会影响整行宽度的字符
        static void replaceInvalidStr(std::string &originStr){
            std::string tmpStr = "";
            size_t i = 0;
            size_t rlen = sizeof(replaceStr) / sizeof(char *);
            for (i = 0; i < rlen; i += 2){
                std::string s1 = replaceStr[i];
                std::string s2 = replaceStr[i + 1];
                strUtils::strReplace(originStr, s1, s2, tmpStr);
                originStr = tmpStr;
                tmpStr = "";
            }
        }


        //对表格单元快速排序
        void terminalTable::quickSortTableCellWidth(std::vector< tableRowWidth * > &rowWidths, int left, int right){
            if (left > right){
                return;
            }
            int i, j;
            tableRowWidth *t, *temp;

            //最左边为基准数
            temp = rowWidths[left];
            i = left;
            j = right;
            while (i != j){
                //先从最右边开始查找
                while (rowWidths[j]->allowWidth <= temp->allowWidth && i < j){
                    j--;
                }
                //再找左边的
                while (rowWidths[i]->allowWidth >= temp->allowWidth && i < j){
                    i++;
                }
                //交换位置
                if (i < j){
                    t = rowWidths[i];
                    rowWidths[i] = rowWidths[j];
                    rowWidths[j] = t;
                }
            }
            //最终将基准数归位
            rowWidths[left] = rowWidths[i];
            rowWidths[i] = temp;
            quickSortTableCellWidth(rowWidths, left, i - 1);
            quickSortTableCellWidth(rowWidths, i + 1, right);

        }

        terminalTable::terminalTable(){
            maxColumnNum = 0;
            allTableAllowWidth = 0;
            headerFontColorType = COLOR_YELLOW;
            rowFontColorType = -1;
            borderColorType = -1;
        }

        //设置相关的颜色：表头的字体颜色，默认Yellow
        void terminalTable::setHeaderFontColorType(ColorType t){
            headerFontColorType = t;
        }

        void terminalTable::genHeaderStrWidthColor(std::string &str){
            if (headerFontColorType > 0){
                std::string output;
                colorUtils::cliColorRender(str, output, headerFontColorType, 1, 0, 0);
                str = output;
            }
        }

        //设置相关的颜色：表格内容的字体颜色
        void terminalTable::setRowFontColorType(ColorType t){
            rowFontColorType = t;
        }

        void terminalTable::genRowStrWidthColor(std::string &str){
            if (rowFontColorType > 0){
                std::string output;
                colorUtils::cliColorRender(str, output, rowFontColorType, 0, 0, 0);
                str = output;
            }
        }

        //设置相关的颜色：边框的颜色
        void terminalTable::setBorderColorType(ColorType t){
            borderColorType = t;
        }

        void terminalTable::genBorderStrWidthColor(std::string &str){
            if (borderColorType > 0){
                std::string output;
                colorUtils::cliColorRender(str, output, borderColorType, 0, 0, 0);
                str = output;
            }
        }

        //添加表头数据
        void terminalTable::addHeadData(const std::vector <std::string> &headData){
            if (headData.empty()){
                return;
            }
            std::vector< std::string >::const_iterator iter;
            for (iter = headData.begin(); iter != headData.end(); iter++){
                std::string tmpStr = *iter;
                replaceInvalidStr(tmpStr);
                rawheadData.push_back(tmpStr);
            }
            if (rawheadData.size() > maxColumnNum){
                maxColumnNum = rawheadData.size();
            }
        }

        //添加表头数据
        void terminalTable::addHeadData(int count, ...){
            if (count <= 0){
                return;
            }
            va_list ap;
            int j;
            va_start(ap, count);
            for (j = 0; j < count; j++){
                const char *str = va_arg(ap, char * );
                std::string tmpStr = str;
                replaceInvalidStr(tmpStr);
                rawheadData.push_back(tmpStr);
            }
            if (rawheadData.size() > maxColumnNum){
                maxColumnNum = rawheadData.size();
            }

            va_end(ap);
            return;
        }

        //添加行数据
        void terminalTable::addRowData(const std::vector <std::string> &rowData){
            if (rowData.empty()){
                return;
            }
            std::vector< std::string >::const_iterator iter;
            std::vector <std::string> tmpRows;
            for (iter = rowData.begin(); iter != rowData.end(); iter++){
                std::string tmpStr = *iter;
                replaceInvalidStr(tmpStr);
                tmpRows.push_back(tmpStr);
            }
            rawRowData.push_back(tmpRows);

            if (tmpRows.size() > maxColumnNum){
                maxColumnNum = tmpRows.size();
            }
        }

        //添加行数据
        void terminalTable::addRowData(int count, ...){
            if (count <= 0){
                return;
            }
            va_list ap;
            int j;
            va_start(ap, count);
            std::vector <std::string> tmpRows;
            for (j = 0; j < count; j++){
                const char *str = va_arg(ap, char * );
                std::string tmpStr = str;
                replaceInvalidStr(tmpStr);
                tmpRows.push_back(tmpStr);
            }
            rawRowData.push_back(tmpRows);
            if (tmpRows.size() > maxColumnNum){
                maxColumnNum = tmpRows.size();
            }

            va_end(ap);
            return;
        }

        //整理各列的宽度信息等
        void terminalTable::reProcessTableWidth(){
            //根据屏幕宽度计算表格的总宽度，留一些冗余宽度
            size_t screenWidth = 0;
            sysUtils::getTerminalSize(&screenWidth, nullptr);
            allTableAllowWidth = screenWidth - maxColumnNum * 3 - 3;

            //初始化各列宽度
            size_t i;
            for (i = 0; i < maxColumnNum; i++){
                maxColumnWidth.push_back(0);;
            }

            //遍历每行数据，找出来每列的最大宽度，表格对齐要用
            std::vector< std::string >::iterator iterCol;
            std::vector < std::vector < std::string > > ::iterator
            iterRow;
            size_t w = 0;
            for (iterCol = rawheadData.begin(); iterCol != rawheadData.end(); iterCol++){
                maxColumnWidth[w++] = runeUtils::runeStringWidth(*iterCol);
            }
            for (iterRow = rawRowData.begin(); iterRow != rawRowData.end(); iterRow++){
                w = 0;
                for (iterCol = iterRow->begin(); iterCol != iterRow->end(); iterCol++){
                    size_t rwidth = runeUtils::runeStringWidth(*iterCol);
                    if (maxColumnWidth[w] < rwidth){
                        maxColumnWidth[w] = rwidth;
                    }
                    w++;
                }
            }

            //计算一下各列允许总宽度，如果超宽要截断处理
            size_t allWidth = 0;
            std::vector < tableRowWidth * > rowWidths;
            for (i = 0; i < maxColumnNum; i++){
                size_t w = maxColumnWidth[i];
                allWidth += w;
                tableRowWidth *tmpRowW = new tableRowWidth();
                tmpRowW->columnNo = i;
                tmpRowW->allowWidth = w;
                rowWidths.push_back(tmpRowW);
            }

            //如果各小格子的宽度和大于屏幕宽度，每轮循环都将最宽的列折行，一直折到适合屏幕大小为止
            while (allWidth > allTableAllowWidth){
                size_t diff = allWidth - allTableAllowWidth;
                quickSortTableCellWidth(rowWidths, 0, maxColumnNum - 1);
                if (rowWidths[0]->allowWidth <= 1){
                    break;
                }
                //本次总宽度消除量
                size_t reduce = rowWidths[0]->allowWidth / 4;
                if (reduce > diff){
                    reduce = diff;
                }
                rowWidths[0]->allowWidth -= reduce;
                allWidth -= reduce;
            }

            //重新设置各列宽度最大值
            for (i = 0; i < maxColumnNum; i++){
                tableRowWidth *rowW = rowWidths[i];
                maxColumnWidth[rowW->columnNo] = rowW->allowWidth + 2;
                delete rowW;
            }
        }

        //给各行的数据折行处理
        void terminalTable::wrapTableRows(tableRow &row, std::vector <std::string> &rowStr){
            size_t i = 0, j = 0;
            //各小格子折行后的最大行数
            size_t maxLineNum = 0;

            std::vector< std::string >::iterator iterCol;

            //统一每列最大宽度
            for (i = 0; i < maxColumnNum; i++){
                std::string tmpStr = rowStr[i];
                size_t w = maxColumnWidth[i] - 2;
                size_t wrapWidth = runeUtils::runeStringWidth(tmpStr);
                size_t lineNum = 1;
                //如果本列的数据超过了本列最大允许的宽度
                if (wrapWidth > w){
                    std::string wrapStr;
                    runeUtils::runeWrap(tmpStr, w, wrapStr, lineNum);
                }
                if (lineNum > maxLineNum){
                    maxLineNum = lineNum;
                }
            }
            row.lineNum = maxLineNum;

            pcreUtils splitRow("\n");

            //计算并补齐各小格子的行数
            for (i = 0; i < maxColumnNum; i++){
                std::string tmpStr = rowStr[i];
                size_t w = maxColumnWidth[i] - 2;
                size_t wrapWidth = runeUtils::runeStringWidth(tmpStr);
                size_t lineNum = 1;

                //如果本列的数据超过了本列最大允许的宽度
                if (wrapWidth > w){
                    std::string wrapStr;
                    runeUtils::runeWrap(tmpStr, w, wrapStr, lineNum);
                    rowStr[i] = wrapStr;
                }

                //对本小格子的数据折行处理
                tmpStr = rowStr[i];
                std::vector <std::string> subCells;
                splitRow.reg_split(tmpStr, subCells);
                for (j = 0; j < subCells.size(); j++){
                    std::string skipValidStr;
                    strUtils::strReplace(subCells[j], "\n", "", skipValidStr);
                    subCells[j] = skipValidStr;
                }
                //所有的小格子折的行数要保持一致
                if (subCells.size() < maxLineNum){
                    for (j = subCells.size(); j < maxLineNum; j++){
                        subCells.emplace_back(" ");
                    }
                }

                tableRowCell cell;
                for (iterCol = subCells.begin(); iterCol != subCells.end(); iterCol++){
                    std::string tmpCell = " " + *iterCol + " ";
                    cell.cellStrs.push_back(tmpCell);
                }
                cell.columnNo = i;
                cell.maxWidth = w;
                row.cellList.push_back(cell);
            }
            return;
        }

        //重新处理各行的数据
        void terminalTable::reProcessTableRowData(){
            reProcessTableWidth();

            size_t i = 0;

            //把所有的行的列数补齐到一致，方便输出
            std::vector< std::string >::iterator iterCol;
            std::vector < std::vector < std::string > > ::iterator
            iterRow;

            //先补齐表头的数据
            if (rawheadData.size() > 0 && rawheadData.size() < maxColumnNum){
                for (i = rawheadData.size(); i < maxColumnNum; i++){
                    rawheadData.emplace_back(" ");
                }
            }

            //再补齐各行的列数
            for (iterRow = rawRowData.begin(); iterRow != rawRowData.end(); iterRow++){
                if (iterRow->size() < maxColumnNum){
                    for (i = iterRow->size(); i < maxColumnNum; i++){
                        iterRow->push_back(" ");
                    }
                }
            }

            //如果有某小格子的内容特别特别的长的话，统一给各行折一下行
            if (!rawheadData.empty()){
                tableRow headObj;
                headObj.rowType = 0;
                wrapTableRows(headObj, rawheadData);
                rowData.push_back(headObj);
            }

            for (iterRow = rawRowData.begin(); iterRow != rawRowData.end(); iterRow++){
                tableRow rowObj;
                rowObj.rowType = 1;
                wrapTableRows(rowObj, *iterRow);
                rowData.push_back(rowObj);
            }

            //将每列的数据补整齐
            std::vector< tableRow >::iterator iterTable;
            std::vector< tableRowCell >::iterator iterCell;
            for (iterTable = rowData.begin(); iterTable != rowData.end(); iterTable++){
                i = 0;
                for (iterCell = iterTable->cellList.begin(); iterCell != iterTable->cellList.end(); iterCell++){
                    size_t w = maxColumnWidth[i++];
                    for (iterCol = iterCell->cellStrs.begin(); iterCol != iterCell->cellStrs.end(); iterCol++){
                        std::string tmpStr;
                        runeUtils::runeFillRight(*iterCol, w, tmpStr);
                        *iterCol = tmpStr;
                    }
                }
            }
        }

        //渲染一行的数据
        void terminalTable::renderSingleRow(tableRow &row, std::string &output){
            std::vector< std::string >::iterator iter;
            size_t i, j;
            std::string sep = "|";
            genBorderStrWidthColor(sep);
            for (i = 0; i < row.lineNum; i++){
                for (j = 0; j < maxColumnNum; j++){
                    output.append(sep);
                    std::string rowContent = row.cellList[j].cellStrs[i];
                    //搞一下颜色
                    if (row.rowType == 0){
                        genHeaderStrWidthColor(rowContent);
                    }
                    else if (row.rowType == 1){
                        genRowStrWidthColor(rowContent);
                    }
                    //再添加到输出里
                    output.append(rowContent);
                }
                output.append(sep);
                output.append("\n");
            }
        }

        //输出最终的结果
        void terminalTable::renderToString(std::string &table){
            reProcessTableRowData();

            std::string lineSep = "+";
            size_t i, j;
            for (i = 0; i < maxColumnNum; i++){
                size_t w = maxColumnWidth[i];
                for (j = 0; j < w; j++){
                    lineSep.append("-");
                }
                lineSep.append("+");
            }
            genBorderStrWidthColor(lineSep);

            table.append(lineSep).append("\n");
            std::vector< tableRow >::iterator iter;
            for (iter = rowData.begin(); iter != rowData.end(); iter++){
                std::string rowStr;
                renderSingleRow(*iter, rowStr);
                table.append(rowStr);
                table.append(lineSep).append("\n");
            }
        }
    }
}
