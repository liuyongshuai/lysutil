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
    return 0;
}