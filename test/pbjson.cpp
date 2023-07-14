/*************************************************************************
 * File:	pbjson.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-14 17:54
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "comutils/pb_utils.h"
#include "protocol/httpresp.pb.h"
#include "httpsvr/global_conf.h"

int main(int argc, char *argv[]) {
    std::string absbin = lysutil::comutils::fileUtils::getRealPath(argv[0]);
    std::cout << "absbin=" << absbin << std::endl;

    std::vector<std::string> tmpdirs;
    lysutil::comutils::strUtils::strSplit(absbin, '/', tmpdirs);
    tmpdirs[tmpdirs.size() - 1] = "server.toml";
    tmpdirs[tmpdirs.size() - 2] = "conf";
    std::string confdir = "/" + lysutil::comutils::strUtils::strJoin(tmpdirs, "/");
    std::cout << "confdir=" << confdir << std::endl;

    std::shared_ptr<lysutil::httpsvr::globalConf> gconf = lysutil::httpsvr::globalConf::get_instance();
    gconf->parseConf(confdir);

    lysutil::httpresp::getTopicListResp resp;
    resp.set_error(43);
    resp.set_msg("safasdfasdfasdf");
    for (const std::string &topic: gconf->topic_list) {
        std::string *t = resp.add_topic_list();
        t->append(topic);
    }

    //转成json
    std::string topicJson = lysutil::comutils::pbUtils::pb2json(resp);
    std::cout << "topicJson=" << topicJson << std::endl;
    return 0;
}