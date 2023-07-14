/*************************************************************************
 * File:	pb_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-14 17:43
 ************************************************************************/
#ifndef _LYSUTIL_PB_UTILS_H_202307141743_
#define _LYSUTIL_PB_UTILS_H_202307141743_

#include<string>
#include<vector>
#include<map>
#include "comutils/common.h"
#include "google/protobuf/util/json_util.h"

namespace lysutil {
    namespace comutils {
        class pbUtils {
        public:
            static void pb2json(const google::protobuf::Message &message, std::string &output);
            static std::string pb2json(const google::protobuf::Message &message);
            static void pb2json(const google::protobuf::Message *message, std::string &output);
            static std::string pb2json(const google::protobuf::Message *message);

        private:
            pbUtils() = delete;
            pbUtils(const pbUtils &) = delete;
            pbUtils &operator=(const pbUtils &) = delete;
        };
    }
} //namespace lysutil


#endif //_LYSUTIL_PB_UTILS_H_202307141743_
