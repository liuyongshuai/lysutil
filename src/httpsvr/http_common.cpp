/*************************************************************************
 * File:	http_common.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 17:33
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "httpsvr/http_common.h"
#include "httpsvr/global_conf.h"
#include "httpsvr/http_request.h"
#include "httpsvr/http_response.h"

namespace lysutil{
    namespace httpsvr{
        /**
         * 访问静态资源的常用方法
         */
        void staticResourceFunc(httpRequest &req, httpResponse &rsp){
            //如果不是以/static/开头的话，直接返回404
            if (!lysutil::comutils::strUtils::hasPrefix(req.uri, "/static/")){
                rsp.setStatus(NOT_FOUND);
                rsp.setBody(httpStatusDesc.find(NOT_FOUND)->second);
                return;
            }


            //文件名称
            std::shared_ptr< globalConf > gconf = globalConf::get_instance();
            std::string filepath = gconf->static_resource_dir + "/.." + req.uri;
            filepath = lysutil::comutils::fileUtils::getRealPath(filepath);
            if (!lysutil::comutils::fileUtils::isFileExist(filepath)){
                rsp.setStatus(NOT_FOUND);
                rsp.setBody(httpStatusDesc.find(NOT_FOUND)->second);
                std::cout << "static file " << filepath << " not exists" << std::endl;
                return;
            }

            rsp.setHeader("last-modified", lysutil::comutils::httpUtils::genRespDate(0));
            rsp.setHeader("cache-control", "max-age=31536000");
            rsp.setHeader("Expires", lysutil::comutils::httpUtils::genRespDate(31536000));

            //提取文件扩展名，再根据其设置相应的响应头信息
            size_t dot = filepath.find_last_of(".");
            std::string extension = filepath.substr(dot + 1, filepath.length() - dot - 1);
            auto iter = contentTypeList.find(extension);
            if (iter != contentTypeList.cend()){
                rsp.setHeader("Content-Type", iter->second);
            }

            //设置相应的内容
            std::string filebody;
            lysutil::comutils::fileUtils::getTxtFileContent(filepath, filebody);
            rsp.setBody(filebody);
        }
    }
}