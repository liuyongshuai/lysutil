/*************************************************************************
 * File:	pb_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-14 17:43
 ************************************************************************/
#include<iostream>
#include "comutils/pb_utils.h"

namespace lysutil {
    namespace comutils {
        void pbUtils::pb2json(const google::protobuf::Message &message, std::string &output) {
            google::protobuf::util::JsonPrintOptions options;
            options.add_whitespace = false;
            options.always_print_primitive_fields = true;
            options.always_print_enums_as_ints = false;
            options.preserve_proto_field_names = true;
            bool ok = google::protobuf::util::MessageToJsonString(message, &output, options).ok();
            if (!ok) {
                output = "";
            }
        }

        std::string pbUtils::pb2json(const google::protobuf::Message &message) {
            std::string ret;
            pb2json(message, ret);
            return ret;
        }

        void pbUtils::pb2json(const google::protobuf::Message *message, std::string &output) {
            pb2json(*message, output);
        }

        std::string pbUtils::pb2json(const google::protobuf::Message *message) {
            std::string ret;
            pb2json(message, ret);
            return ret;
        }
    }
} //namespace lysutil
