/*************************************************************************
 * File:	http_svr_test.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 14:46
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<iostream>
#include "httpsvr/http_request.h"
#include "httpsvr/http_response.h"
#include "httpsvr/http_router.h"
#include "httpsvr/http_server.h"
#include "httpsvr/http_common.h"
#include "comutils/file_utils.h"
#include "httpsvr/global_conf.h"

void index(lysutil::httpsvr::httpRequest &req, lysutil::httpsvr::httpResponse &resp){
    resp.setBody("Welcome:" + req.client_ip);
}

void getJson(lysutil::httpsvr::httpRequest &req, lysutil::httpsvr::httpResponse &resp){
    resp.setBody("{\"q\":\"http s\",\"p\":false,\"g\":[{\"type\":\"sug\",\"sa\":\"s_1\",\"q\":\"http status 500\"},{\"type\":\"sug\",\"sa\":\"s_2\",\"q\":\"http status 400\"},{\"type\":\"sug\",\"sa\":\"s_3\",\"q\":\"http socket\"},{\"type\":\"sug\",\"sa\":\"s_4\",\"q\":\"http session\"},{\"type\":\"sug\",\"sa\":\"s_5\",\"q\":\"httpstatus404解决步骤\"},{\"type\":\"sug\",\"sa\":\"s_6\",\"q\":\"http status\"},{\"type\":\"sug\",\"sa\":\"s_7\",\"q\":\"http stat jseea\"},{\"type\":\"sug\",\"sa\":\"s_8\",\"q\":\"http socket tcp区别\"},{\"type\":\"sug\",\"sa\":\"s_9\",\"q\":\"http status 404-not found\"},{\"type\":\"sug\",\"sa\":\"s_10\",\"q\":\"http status 500解决\"}]}");
    resp.setJsonHeader();
}

int main(int argc, char *argv[]){
    std::string absbin = lysutil::comutils::fileUtils::getRealPath(argv[0]);
    std::cout << "absbin=" << absbin << std::endl;

    std::vector< std::string > tmpdirs;
    lysutil::comutils::strUtils::strSplit(absbin, '/', tmpdirs);
    tmpdirs[tmpdirs.size() - 1] = "server.toml";
    tmpdirs[tmpdirs.size() - 2] = "conf";
    std::string confdir = "/" + lysutil::comutils::strUtils::strJoin(tmpdirs, "/");
    std::cout << "confdir=" << confdir << std::endl;

    std::shared_ptr< lysutil::httpsvr::globalConf > gconf = lysutil::httpsvr::globalConf::get_instance();
    gconf->parseConf(confdir);

    //添加路由信息
    std::shared_ptr< lysutil::httpsvr::httpRouter > routers = lysutil::httpsvr::httpRouter::get_instance();
    routers->addRouter(lysutil::httpsvr::ROUTER_TYPE_REGEXP, R"(static/.+)", lysutil::httpsvr::staticResourceFunc);
    routers->addRouter(lysutil::httpsvr::ROUTER_TYPE_PATH_INFO, "/ipc", lysutil::httpsvr::staticResourceFunc, "/static/html/ipc.html");
    routers->addRouter(lysutil::httpsvr::ROUTER_TYPE_PATH_INFO, "/", index);
    routers->addRouter(lysutil::httpsvr::ROUTER_TYPE_PATH_INFO, "/getjson", getJson);

    //启动server
    std::shared_ptr< lysutil::httpsvr::httpServer > server = lysutil::httpsvr::httpServer::get_instance();
    server->setPort(5000);
    server->start();
    return 0;
}