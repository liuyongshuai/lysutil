/*************************************************************************
 * File:	md5_test.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 16:32
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "comutils/md5_utils.h"
#include "comutils/str_utils.h"
#include "comutils/http_utils.h"

int main(int argc, char *argv[]){
    std::string str = "asdfasfda8743-t971-9834tlk=];....'.'";
    std::string md51 = lysutil::comutils::strUtils::md5sum(str);
    std::cout << "md51=" << md51 << std::endl;

    lysutil::comutils::md5Utils md5Utils;
    char buf[32] = {0};
    md5Utils.MD5Str(str.c_str(), str.size(), buf, 32);
    std::cout << "md52=" << buf << std::endl;
    std::cout << "genFixedRespDate=" << lysutil::comutils::httpUtils::genFixedRespDate(time(nullptr)) << std::endl;
    return 0;
}