/*************************************************************************
 * File:	pb_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-14 17:43
 ************************************************************************/
#include<iostream>
#include "comutils/pb_utils.h"

namespace lysutil {
    namespace comutils {
        google::protobuf::util::JsonPrintOptions pbUtils::printOptions_ = {false, true, true, true};

        void pbUtils::pb2json(const google::protobuf::util::Message &message, std::string &output) {
            bool ok = google::protobuf::util::MessageToJsonString(message, &output, printOptions_).ok();
            if (!ok) {
                output = "";
            }
        }

        std::string pbUtils::pb2json(const google::protobuf::util::Message &message) {
            std::string ret;
            pb2json(message, ret);
            return ret;
        }

        void pbUtils::pb2json(const google::protobuf::util::Message *message, std::string &output) {
            pb2json(*message, output);
        }

        std::string pbUtils::pb2json(const google::protobuf::util::Message *message) {
            std::string ret;
            pb2json(message, ret);
            return ret;
        }
    }
} //namespace lysutil
