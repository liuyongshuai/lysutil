/*************************************************************************
 * File:	http_request.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:02
 ************************************************************************/
#include<iostream>
#include "httpsvr/http_request.h"

namespace lysutil{
    namespace httpsvr{
        httpRequest::httpRequest(const char *body, size_t bodyLen){
            this->parseBody(body, bodyLen);
        }

        /**
         * 提取header中的信息
         */
        void httpRequest::getHeader(const std::string &key, std::string &val){
            std::string k = key;
            comutils::strUtils::toLower(k);
            std::map< std::string, std::string >::const_iterator iter;
            if ((iter = this->headers.find(k)) != this->headers.end()){
                val = iter->second;
            }
        }

        /**
         * 提取单个请求参数
         */
        void httpRequest::getArg(const std::string &key, std::string &val){
            std::vector< std::string > vals;
            this->getArg(key, vals);
            if (vals.empty()){
                return;
            }
            val = vals[0];
        }

        /**
         * 提取请求参数的数组
         */
        void httpRequest::getArg(const std::string &key, std::vector< std::string > &vals){
            std::map< std::string, std::vector< std::string >>::const_iterator
                    iter;
            if ((iter = this->args.find(key)) != this->args.end()){
                vals = iter->second;
            }
        }

        /**
         * 是否为ajax请求
         */
        bool httpRequest::isAjax(){
            std::string v;
            this->getHeader("X-Requested-With", v);
            return v == "XMLHttpRequest";
        }

        /**
         * 打印请求的信息
         */
        void httpRequest::printReq(){
            std::cout << "method=" << method << std::endl;
            std::cout << "uri=" << uri << std::endl;
            std::cout << "protocol=" << protocol << std::endl;
            std::map< std::string, std::string >::const_iterator iter;
            std::string sepline;

            //打印header信息
            comutils::terminalTable tHeader;
            tHeader.addHeadData(2, "key", "value");
            for (iter = this->headers.begin(); iter != this->headers.end(); iter++){
                tHeader.addRowData(2, iter->first.c_str(), iter->second.c_str());
            }
            std::string tstr;
            tHeader.renderToString(tstr);
            std::cout << tstr << std::endl;

            //开始打印请求参数
            if (!args.empty()){
                std::cout << std::endl;
                std::map< std::string, std::vector< std::string > >::const_iterator mapVecIter;
                comutils::terminalTable tArgs;
                tArgs.addHeadData(2, "key", "value");
                for (mapVecIter = this->args.begin(); mapVecIter != this->args.end(); mapVecIter++){
                    std::string v;
                    comutils::strUtils::strJoin(mapVecIter->second, "\n", v);
                    tArgs.addRowData(2, mapVecIter->first.c_str(), v.c_str());
                }
                std::string tstr;
                tArgs.renderToString(tstr);
                std::cout << tstr << std::endl;
            }

            //打印上传文件信息
            std::map< std::string, uploadFile >::const_iterator fileIter;
            for (fileIter = this->uploadFiles.begin(); fileIter != this->uploadFiles.end(); fileIter++){
                std::cout << "fileName=" << fileIter->second.fileName << "\ttype=" << fileIter->second.type << "\tfileLen=" << fileIter->second.content.size() << std::endl;
            }
        }

        /**
         * 解析http请求的body，会填充请求参数、上传文件等各种信息，基本格式：
         * GET /abc.....
         * Key: Value
         *
         * bodyBin
         */
        int httpRequest::parseBody(const void *body, size_t bodyLen){
            if (bodyLen == 0){
                return -1;
            }
            const char *sepChar1 = "\r\n";
            const char *sepChar2 = "\r\n\r\n";
            char tmpBuf[HTTP_HEADER_BUF_SIZE];
            std::string tmpStr;
            std::vector< std::string > tmpVec;
            size_t sz, pos;
            std::vector< std::string >::const_iterator vecStrIter;
            const char *tmpReqBody = (char *) body;
            const char *tmpHeaderCur, *tmpHeaderPre = tmpReqBody;


            //以类似于"GET /abc HTTP/1.1"开头
            tmpHeaderCur = strstr(tmpReqBody, sepChar1);
            if (tmpHeaderCur == nullptr){
                return -1;
            }
            sz = tmpHeaderCur - tmpHeaderPre;
            bzero(tmpBuf, HTTP_HEADER_BUF_SIZE);
            memcpy(tmpBuf, tmpHeaderPre, sz);
            tmpStr = tmpBuf;
            comutils::strUtils::strSplit(tmpStr, ' ', tmpVec);
            if (tmpVec.size() < 3){
                return -1;
            }


            //0即为method
            comutils::strUtils::trimSpace(tmpVec[0]);
            comutils::strUtils::toUpper(tmpVec[0]);
            if (tmpVec[0] == "GET"){
                this->method = GET;
            }
            else if (tmpVec[0] == "GET"){
                this->method = GET;
            }
            else if (tmpVec[0] == "POST"){
                this->method = POST;
            }
            else if (tmpVec[0] == "PUT"){
                this->method = PUT;
            }
            else if (tmpVec[0] == "HEAD"){
                this->method = HEAD;
            }
            else if (tmpVec[0] == "DELETE"){
                this->method = DELETE;
            }
            else{
                return -1;
            }

            //1即为uri
            comutils::strUtils::trimSpace(tmpVec[1]);
            uri = tmpVec[1];
            if ((pos = uri.find_first_of("?", 0)) != std::string::npos){
                std::string argStr(uri, pos + 1);
                uri = uri.substr(0, pos);
                this->parseArgs(argStr);
            }

            //2即为protocol
            comutils::strUtils::trimSpace(tmpVec[2]);
            protocol = tmpVec[2];
            tmpHeaderCur += 2;
            tmpHeaderPre = tmpReqBody + bodyLen;

            //解析请求的header信息
            if ((tmpHeaderPre = strstr(tmpHeaderCur, sepChar2)) == nullptr){
                tmpHeaderPre = tmpReqBody + bodyLen;
            }
            tmpStr.clear();
            tmpVec.clear();
            tmpStr.append(tmpHeaderCur, tmpHeaderPre - tmpHeaderCur);
            comutils::pcreUtils reg(sepChar1);
            reg.reg_split(tmpStr, tmpVec);
            if (!tmpVec.empty()){
                for (vecStrIter = tmpVec.begin(); vecStrIter != tmpVec.end(); vecStrIter++){
                    pos = vecStrIter->find_first_of(":", 0);
                    if (pos == std::string::npos || pos == 0){
                        continue;
                    }
                    std::string k(*vecStrIter, 0, pos);
                    std::string v(*vecStrIter, pos + 1);
                    comutils::strUtils::trimSpace(k);
                    comutils::strUtils::trimSpace(v);
                    comutils::strUtils::toLower(k);
                    this->headers[k] = v;
                }
            }

            //如果是get/head/delete可以返回了
            if (this->method == GET || this->method == HEAD || this->method == DELETE){
                return 0;
            }

            //开始解析body信息
            tmpHeaderPre += 4;
            if (tmpHeaderPre >= tmpReqBody + bodyLen){
                return 0;
            }

            //body的长度
            sz = tmpReqBody + bodyLen - tmpHeaderPre;
            char *rawBody = (char *) malloc(sizeof(char) * (sz + 1));
            bzero(rawBody, sizeof(char) * (sz + 1));
            memcpy(rawBody, tmpHeaderPre, sz);
            size_t rawBodyLen = sz;

            //如果是普通的POST
            std::string cntType;
            std::string upBoundary;
            this->getHeader("Content-Type", cntType);
            if (cntType.find_first_of(";", 0) != std::string::npos){
                tmpVec.clear();
                comutils::strUtils::strSplit(cntType, ';', tmpVec);
                for (vecStrIter = tmpVec.begin(); vecStrIter != tmpVec.end(); vecStrIter++){
                    std::string tmpCntType = *vecStrIter;
                    comutils::strUtils::trimSpace(tmpCntType);
                    if (tmpCntType.find_first_of("/", 0) != std::string::npos){
                        cntType = tmpCntType;
                    }
                    else if (tmpCntType.find_first_of("boundary=", 0) == 0){
                        upBoundary = tmpCntType.substr(9);
                    }
                }
            }
            comutils::strUtils::trimSpace(cntType);
            comutils::strUtils::toLower(cntType);
            if (cntType == NORMAL_POST_CONTENT_TYPE){
                this->parseArgs(rawBody);
                free(rawBody);
                return 0;
            }
            //如果不是上传文件
            if (cntType != UPLOAD_POST_CONTENT_TYPE){
                free(rawBody);
                return 0;
            }

            //解析以multipart/form-data上传的字段或文件
            this->parseMultiPartFormDataArgs(upBoundary.c_str(), rawBody, rawBodyLen);
            free(rawBody);
            return 0;
        }

        /**
         * 解析multipart/form-data的参数
         */
        void httpRequest::parseMultiPartFormDataArgs(const std::string &boundary, const char *rawBody, size_t rawBodyLen){
            if (rawBody == nullptr || rawBodyLen == 0){
                return;
            }
            const char *tmpBuf = rawBody;
            const char *cur = nullptr, *next = nullptr, *tmpPtr = nullptr;
            std::string tmpBoundary = boundary;
            tmpBoundary.append("\r\n");
            size_t i, pos;

            //寻找分界符
            while ((cur = strstr(tmpBuf, tmpBoundary.c_str())) != nullptr){
                std::string fieldName, fieldValue, fileName, fileContentType;
                cur += tmpBoundary.size();
                std::string tmpPart;
                tmpPtr = cur;
                //先读一行，判断是字段还是文件
                while (true){
                    if (*tmpPtr == '\r' && tmpPtr < rawBody + rawBodyLen && *(tmpPtr + 1) == '\n'){
                        break;
                    }
                    tmpPart.append(tmpPtr++, 1);
                }
                comutils::strUtils::trimSpace(tmpPart);
                //非法数据
                if (tmpPart.empty() || tmpPart.find("Content-Disposition") == std::string::npos){
                    return;
                }
                std::vector< std::string > tmpVec;
                comutils::strUtils::strSplit(tmpPart, ';', tmpVec);
                if (tmpVec.empty()){
                    return;
                }
                for (i = 0; i < tmpVec.size(); i++){
                    comutils::strUtils::trimSpace(tmpVec[i]);
                    if (tmpVec[i].find("filename=") == 0){
                        fileName = tmpVec[i].substr(9);
                        comutils::strUtils::trimChar(fileName, '"');
                        continue;
                    }
                    if (tmpVec[i].find("name=") == 0){
                        fieldName = tmpVec[i].substr(5);
                        comutils::strUtils::trimChar(fieldName, '"');
                        continue;
                    }
                }

                //字段
                if (fileName.empty()){
                    //跳过\r\n\r\n
                    tmpPtr += 4;
                    while (true){
                        if (*tmpPtr == '\r' && tmpPtr < rawBody + rawBodyLen && *(tmpPtr + 1) == '\n'){
                            break;
                        }
                        fieldValue.append(tmpPtr++, 1);
                    }
                    tmpPtr += 2;
                    tmpBuf = tmpPtr;
                    this->args[fieldName].push_back(fieldValue);
                    continue;
                }

                //如果是文件的话，下一行是Content-Type
                tmpPtr += 2;
                std::string tmpFileContentType;
                while (true){
                    if (*tmpPtr == '\r' && tmpPtr < rawBody + rawBodyLen && *(tmpPtr + 1) == '\n'){
                        break;
                    }
                    tmpFileContentType.append(tmpPtr++, 1);
                }
                if ((pos = tmpFileContentType.find(":")) != std::string::npos){
                    fileContentType = tmpFileContentType.substr(pos + 1);
                    comutils::strUtils::trimSpace(fileContentType);
                }
                //跳过\r\n\r\n
                tmpPtr += 4;
                next = tmpPtr;
//                std::cout << "fileName=" << fileName << "\tfieldName=" << fieldName << "\tfileContentType=" << fileContentType << std::endl;
//                std::cout << "remain size=" << rawBody + rawBodyLen - tmpPtr << "\trawBodyLen=" << rawBodyLen << std::endl;
                //搜索\r，下一个字符必须是\n，前面的字符串必须是分界符
//                std::cout << "tmpBoundary.size()=" << tmpBoundary.size() << "\ttmpBoundary=" << tmpBoundary << std::endl;
                while (next < rawBody + rawBodyLen){
                    next++;
                    if ((size_t) (next - tmpPtr) < tmpBoundary.size()){
                        continue;
                    }
                    //分找分界符了，回退到上一个\r\n
                    if (*next == '\r' && *(next + 1) == '\n'){
                        std::string abc(next - tmpBoundary.size(), tmpBoundary.size());
                        if (abc.find(boundary) != std::string::npos){
//                            std::cout << "find\t" << tmpBoundary << std::endl;
                            while (true){
                                next--;
                                if (*next == '\r' && *(next + 1) == '\n'){
                                    break;
                                }
                            }
                            break;
                        }

                    }
                }
                if (next == nullptr){
                    return;
                }
                uploadFile upF;
                upF.type = fileContentType;
                upF.fileName = fileName;
                upF.content.append(tmpPtr, next - tmpPtr);
                this->uploadFiles[fileName] = upF;
                tmpBuf = next;
            }
        }

        /**
         * 简单的解析KV参数
         */
        void httpRequest::parseArgs(const std::string &str){
            std::vector< std::string > tmpArgVec;
            size_t pos = 0;
            comutils::strUtils::strSplit(str, '&', tmpArgVec);
            std::vector< std::string >::const_iterator vecStrIter;
            for (vecStrIter = tmpArgVec.begin(); vecStrIter != tmpArgVec.end(); vecStrIter++){
                pos = vecStrIter->find_first_of("=", 0);
                if (pos == std::string::npos){
                    continue;
                }
                std::string k(*vecStrIter, 0, pos);
                std::string v(*vecStrIter, pos + 1);
                this->args[k].push_back(v);
            }
        }

        /**
         * 释放资源
         */
        httpRequest::~httpRequest(){

        }
    }
} //namespace pfsm_logsim
