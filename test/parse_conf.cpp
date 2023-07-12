/*************************************************************************
 * File:	parse_conf.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 19:07
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "httpsvr/global_conf.h"

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
    return 0;
}