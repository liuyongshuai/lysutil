/*************************************************************************
 * File:	global_conf.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 18:24
 ************************************************************************/
#include<iostream>
#include "httpsvr/global_conf.h"

namespace lysutil{
    namespace httpsvr{
        /**
         * 解析配置文件
         */
        bool globalConf::parseConf(const std::string &conf_file){
            std::string realPath = lysutil::comutils::fileUtils::getRealPath(conf_file);
            if (!lysutil::comutils::fileUtils::isFileExist(realPath)){
                std::cout << "conf file :" << conf_file << " not exists" << std::endl;
                return false;
            }
            std::cout << "start parse " << realPath << std::endl;

            //开始解析
            lysutil::comutils::TomlParser tomlParser(realPath);
            int64_t port64;
            tomlParser.getInt64("http.port", port64);
            this->listen_port = (uint16_t) port64;
            std::cout << "listen_port=" << this->listen_port << std::endl;

            std::string staticname;
            tomlParser.getString("http.static_dir", staticname);
            std::vector< std::string > tmpdirs;
            lysutil::comutils::strUtils::strSplit(realPath, '/', tmpdirs);
            tmpdirs[tmpdirs.size() - 1] = "";
            tmpdirs[tmpdirs.size() - 2] = staticname;
            this->static_resource_dir = "/" + lysutil::comutils::strUtils::strJoin(tmpdirs, "/");
            lysutil::comutils::strUtils::trimRightSpace(this->static_resource_dir);
            lysutil::comutils::strUtils::trimChar(this->static_resource_dir, '/');
            std::cout << "static_resource_dir=" << this->static_resource_dir << std::endl;

            int64_t wcount64;
            tomlParser.getInt64("thread.worker_count", wcount64);
            this->thread_worker_count = (uint16_t) wcount64;
            std::cout << "thread_worker_count=" << this->thread_worker_count << std::endl;

            int64_t wpendingcount64;
            tomlParser.getInt64("thread.max_pending_count", wpendingcount64);
            this->max_pending_worker_count = (uint16_t) wpendingcount64;
            std::cout << "max_pending_worker_count=" << this->max_pending_worker_count << std::endl;
            return true;
        }
    }
} //namespace lysutil
