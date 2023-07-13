/*************************************************************************
 * File:	file_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:56
 ************************************************************************/
#ifndef _PFSM_LOGSIM_FILE_UTILS_H_202306271156_
#define _PFSM_LOGSIM_FILE_UTILS_H_202306271156_

#include<string>
#include<vector>
#include<map>
#include "comutils/str_utils.h"

namespace lysutil{
    namespace comutils{
        class fileUtils{
        public:
            //文件操作相关，如判断文件或目录是否存在、最后修改时间、获取目录下的文件列表(包括隐藏的)、一次性读写文件等
            static bool isFileExist(const std::string &filePath);
            static uint32_t getFileLastModifyTime(const std::string &filePath);
            static int getDirFileList(const std::string &dir, std::list< std::string > &filelList);
            static size_t fileSize(const std::string &filePath);
            static int getTxtFileContent(const std::string &filePath, std::string &content);
            static int getTxtFileContent(const std::string &filePath, std::vector< std::string > &lines);
            static int putTxtFileContent(const std::string &filePath, const std::string &content, bool needBackup = false);
            static int putTxtFileContent(const std::string &filePath, const std::vector< std::string > &content, bool needBackup = false);
            static int getRawFileContent(const std::string &filePath, std::string &content);
            static int putRawFileContent(const std::string &filePath, const std::string &content, bool needBackup = false);
            static int putRawFileContent(const std::string &filePath, const char *content, size_t content_len, bool needBackup = false);

            //创建目录
            static bool mkDirsAll(const std::string &dirNAme);
            static std::string backupFile(const std::string &filePath, bool needBackup = false);

            //获取目录的规范的路径
            static bool getRealPath(const std::string &path, std::string &realPath);
            static std::string getRealPath(const std::string &path);

            //封装的提取文件的base name
            static void fileBaseName(const std::string &filePath, std::string &baseName);

        private:
            fileUtils() = delete;
            fileUtils(const fileUtils &) = delete;
            fileUtils &operator=(const fileUtils &) = delete;
        };
    }
} //namespace Project


#endif //_PFSM_LOGSIM_FILE_UTILS_H_202306271156_
