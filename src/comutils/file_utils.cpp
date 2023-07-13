/*************************************************************************
 * File:	file_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:56
 ************************************************************************/
#include<iostream>
#include "comutils/file_utils.h"

namespace lysutil{
    namespace comutils{
        //判断文件是否存在
        bool fileUtils::isFileExist(const std::string &filePath){
            struct stat buffer{};
            return (stat(filePath.c_str(), &buffer) == 0);
        }

        //获取文件的最后修改时间
        uint32_t fileUtils::getFileLastModifyTime(const std::string &filePath){
            struct stat statInfo{};
            stat(filePath.c_str(), &statInfo);
            return statInfo.st_mtime;
        }

        //读取目录下所有的文件列表，包括隐藏的
        int fileUtils::getDirFileList(const std::string &dir, std::list< std::string > &filelList){
            std::string realDir = getRealPath(dir);
            if (!isFileExist(realDir)){
                return -1;
            }
            //打开目录
            DIR *dp = opendir(realDir.c_str());
            if (nullptr == dp){
                perror("open dir failed");
                return -1;
            }
            dirent *p = nullptr;
            while ((p = readdir(dp)) != nullptr){
                std::string tmpFile(p->d_name, strlen(p->d_name));
                if (tmpFile == "." || tmpFile == ".."){
                    continue;
                }
                tmpFile = realDir + "/" + tmpFile;
                struct stat st{};
                stat(tmpFile.c_str(), &st);
                //如果是目录，继续遍历
                if (S_ISDIR(st.st_mode)){
                    getDirFileList(tmpFile, filelList);
                }
                else if (S_ISREG(st.st_mode)){
                    filelList.push_back(tmpFile);
                }
            }
            closedir(dp);
            return 0;
        }

        //获取文件大小
        size_t fileUtils::fileSize(const std::string &filePath){
            struct stat statbuf;
            stat(filePath.c_str(), &statbuf);
            size_t size = statbuf.st_size;
            return size;
        }

        //读取文件全部内容
        int fileUtils::getTxtFileContent(const std::string &filePath, std::string &content){
            FILE *fp = fopen(filePath.c_str(), "r");
            if (fp == nullptr){
                perror("open file failed\n");
                return -1;
            }
            char buf[5120] = {0};
            size_t bsize = 4096;
            size_t rnum = 0;
            while (!feof(fp)){
                memset(buf, 0, 5120);
                rnum = fread(buf, bsize, 1, fp);
                content.append(buf, strlen(buf));
                if (rnum == 0){
                    break;
                }
            }
            fclose(fp);
            return 0;
        }

        //读取文件全部内容
        int fileUtils::getTxtFileContent(const std::string &filePath, std::vector< std::string > &lines){
            std::vector< std::string > tmp;
            std::string content;
            getTxtFileContent(filePath, content);
            strUtils::strSplit(content, '\n', tmp);
            for (auto line: tmp){
                strUtils::trimSpace(line);
                if (line.empty()){
                    continue;
                }
                lines.push_back(line);
            }
            return 0;
        }

        //写内容入文件
        int fileUtils::putTxtFileContent(const std::string &filePath, const std::string &content, bool needBackup){
            std::string realFilePath = backupFile(filePath, needBackup);

            //开始写文件
            FILE *fp = fopen(realFilePath.c_str(), "w");
            if (fp == nullptr){
                perror("open file failed");
                return -1;
            }
            size_t fsize = content.size();
            int n = fwrite(content.c_str(), fsize, 1, fp);
            if (n <= 0){
                perror("fwrite file failed");
                std::cerr << "fwrite failed,n=" << n << "\tfsize=" << fsize << std::endl;
                fclose(fp);
                return -1;
            }
            fclose(fp);
            return 0;
        }

        //写内容入文件
        int fileUtils::putTxtFileContent(const std::string &filePath, const std::vector< std::string > &content, bool needBackup){
            std::string newContent;
            strUtils::strJoin(content, "\n", newContent);
            return putTxtFileContent(filePath, newContent, needBackup);
        }

        int fileUtils::getRawFileContent(const std::string &filePath, std::string &content){
            FILE *fp = fopen(filePath.c_str(), "br");
            if (fp == nullptr){
                perror("open file failed\n");
                return -1;
            }
            char buf[4096] = {0};
            size_t bsize = 4096;
            size_t rnum = 0;
            while (!feof(fp)){
                memset(buf, 0, 4096);
                rnum = fread(buf, 1, bsize, fp);
                if (rnum == 0){
                    break;
                }
                content.append(buf, rnum);
            }
            fclose(fp);
            return 0;
        }

        int fileUtils::putRawFileContent(const std::string &filePath, const std::string &content, bool needBackup){
            putRawFileContent(filePath, content.c_str(), content.size(), needBackup);
        }

        int fileUtils::putRawFileContent(const std::string &filePath, const char *content, size_t content_len, bool needBackup){
            std::string realFilePath = backupFile(filePath, needBackup);

            //开始写文件
            FILE *fp = fopen(realFilePath.c_str(), "bw");
            if (fp == nullptr){
                perror("open file failed");
                return -1;
            }
            int n = fwrite(content, content_len, 1, fp);
            if (n <= 0){
                perror("fwrite file failed");
                std::cerr << "fwrite failed,n=" << n << "\tfsize=" << content_len << std::endl;
                fclose(fp);
                return -1;
            }
            fclose(fp);
            return 0;
        }

        std::string fileUtils::backupFile(const std::string &filePath, bool needBackup){
            std::string realFilePath;
            getRealPath(filePath, realFilePath);

            //如果已存在的文件需要备份的话
            if (isFileExist(realFilePath) && needBackup){
                std::string newFile = realFilePath + "." + std::to_string(time(nullptr));
                int ret = rename(realFilePath.c_str(), newFile.c_str());
                if (ret != 0){
                    perror("rename file failed");
                    return realFilePath;
                }
            }

            //提取dirname确保已经创建
            std::vector< std::string > vec;
            strUtils::strSplit(realFilePath, '/', vec);
            std::string dirName;
            for (size_t i = 0; i < vec.size() - 1; i++){
                dirName.append("/").append(vec[i]);
            }
            mkDirsAll(dirName);
            return realFilePath;
        }

        //创建目录
        bool fileUtils::mkDirsAll(const std::string &dirName){
            std::string realpath;
            getRealPath(dirName, realpath);
            if (realpath.empty()){
                return false;
            }
            if (isFileExist(realpath)){
                return true;
            }
            std::string tmp;
            std::vector< std::string > vec;
            strUtils::strSplit(realpath, '/', vec);
            for (auto &i: vec){
                tmp.append("/").append(i);
                if (isFileExist(tmp)){
                    continue;
                }
                if (mkdir(tmp.c_str(), 0755) != 0){
                    return false;
                }
            }
            return true;
        }

        //封装的提取文件的base name
        void fileUtils::fileBaseName(const std::string &filePath, std::string &baseName){
            char buf[512] = {0};
            sprintf(buf, "%s", filePath.c_str());
            char *bname = strrchr(buf, '/');
            if (bname == nullptr){
                bname = buf;
            }
            else if (bname < buf + strlen(buf)){
                bname++;
            }
            baseName = bname;
        }

        //获取目录的规范的路径
        bool fileUtils::getRealPath(const std::string &path, std::string &realPath){
            realPath = path;
            if (path.empty()){
                return false;
            }
            std::string curPath;
            if (path.at(0) != '/'){
                char buf[4096] = {0};
                if (getcwd(buf, 4096) != nullptr){
                    curPath.append(buf, strlen(buf)).append("/").append(path);
                }
            }
            else{
                curPath = path;
            }
            std::vector< std::string > tmp;
            int idx = 0;
            strUtils::strSplit(curPath, '/', tmp);
            for (size_t i = 0; i < tmp.size(); i++){
                if (tmp[i] == "." || tmp[i].empty()){
                    continue;
                }
                if (tmp[i] != ".."){
                    tmp[idx++] = tmp[i];
                    continue;
                }
                if (tmp[i] == ".."){
                    idx--;
                    if (idx < 0){
                        idx = 0;
                    }
                }
            }
            realPath.clear();
            for (int i = 0; i < idx; i++){
                realPath.append("/").append(tmp[i]);
            }

            return true;
        }

        //获取目录的规范的路径
        std::string fileUtils::getRealPath(const std::string &path){
            std::string ret;
            getRealPath(path, ret);
            return ret;
        }

    }
}
