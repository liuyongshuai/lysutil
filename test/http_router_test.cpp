/*************************************************************************
 * File:	http_router_test.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-28 14:33
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "httpsvr/http_router.h"
#include "httpsvr/http_request.h"
#include "httpsvr/http_response.h"

void httpFunc1(lysutil::httpsvr::httpRequest &req, lysutil::httpsvr::httpResponse &rsp) {
    std::cout << "httpFunc1" << std::endl;
}

void httpFunc2(lysutil::httpsvr::httpRequest &req, lysutil::httpsvr::httpResponse &rsp) {
    std::cout << "httpFunc2" << std::endl;
}

void printArgs(std::map<std::string, std::string> &args) {
    std::cout << std::endl;
    std::map<std::string, std::string>::const_iterator iter;
    for (iter = args.begin(); iter != args.end(); iter++) {
        std::cout << iter->first << "\t" << iter->second << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    std::map<std::string, std::string> args;
    std::shared_ptr<lysutil::httpsvr::httpRouter> routers = lysutil::httpsvr::httpRouter::get_instance();
    const lysutil::httpsvr::routerItem *rItem;
    routers->addRouter(lysutil::httpsvr::ROUTER_TYPE_PATH_INFO, "/abc/:aaaa:/:dddd", httpFunc1);
    routers->addRouter(lysutil::httpsvr::ROUTER_TYPE_PATH_INFO, "/abc/aaaa/dddd", httpFunc1);
    routers->addRouter(lysutil::httpsvr::ROUTER_TYPE_REGEXP, "/abcd/(\\d+)/(\\w+)", httpFunc2, "", "a=$1&b=$2");
    std::string uri = "/abc/wendao/444444";
    rItem = routers->matchRouter(uri, args);
    std::cout << uri << "\tmatch\t" << (rItem == nullptr ? "failed" : rItem->config) << std::endl;
    printArgs(args);
    uri = "/abc/aaaa/dddd";
    rItem = routers->matchRouter(uri, args);
    std::cout << uri << "\tmatch\t" << (rItem == nullptr ? "failed" : rItem->config) << std::endl;
    printArgs(args);
    uri = "/abc/5544554/wendao";
    rItem = routers->matchRouter(uri, args);
    std::cout << uri << "\tmatch\t" << (rItem == nullptr ? "failed" : rItem->config) << std::endl;
    printArgs(args);
    uri = "/abcd/333333/8888";
    rItem = routers->matchRouter(uri, args);
    std::cout << uri << "\tmatch\t" << (rItem == nullptr ? "failed" : rItem->config) << std::endl;
    printArgs(args);
    return 0;
}