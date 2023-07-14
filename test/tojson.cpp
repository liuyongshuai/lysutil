/*************************************************************************
 * File:	tojson.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-13 14:24
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "car_info.pb.h"
#include "google/protobuf/util/json_util.h"

int main(int argc, char *argv[]){
    haomo::hios::BrakeInfoPb breakMsg;
    breakMsg.mutable_header()->set_seq(44444444);
    breakMsg.mutable_header()->set_stamp(88888);
    breakMsg.set_brake_pedal_status(true);
    breakMsg.set_pedal_output(333.00);
    breakMsg.set_brake_enable(true);

    std::string json;
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = false;
    options.always_print_primitive_fields = true;
    options.preserve_proto_field_names = true;
    bool ok = google::protobuf::util::MessageToJsonString(breakMsg, &json, options).ok();
    std::cout << "ok=" << ok << std::endl;
    std::cout << "json=" << json << std::endl;

    //json to pb
    haomo::hios::BrakeInfoPb breakMsg1;
    ok = google::protobuf::util::JsonStringToMessage(json, &breakMsg1).ok();
    std::cout << "ok=" << ok << std::endl;
    std::cout << "seq=" << breakMsg1.header().seq() << std::endl;
    std::cout << "stamp=" << breakMsg1.header().stamp() << std::endl;
    std::cout << "brake_pedal_status=" << breakMsg1.brake_pedal_status() << std::endl;
    std::cout << "pedal_output=" << breakMsg1.pedal_output() << std::endl;
    std::cout << "brake_enable=" << breakMsg1.brake_enable() << std::endl;
    return 0;
}