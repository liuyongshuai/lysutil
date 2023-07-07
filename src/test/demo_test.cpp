/*************************************************************************
 * File:	gen_test.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-25 18:13
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "topicmgr/topic_manager.h"
#include "car_info.pb.h"

int main(int argc, char *argv[]){
    std::shared_ptr <haomo::hios::MsgNode> node = std::make_shared< haomo::hios::MsgNode >();
    node->init("127.0.0.1", 7000);

    std::shared_ptr <haomo::transtopic::TopicManager> topicManager = haomo::transtopic::TopicManager::get_instance();
    topicManager->set_node(node);


    topicManager->register_topic< haomo::hios::BrakeInfoPb >("break_rx", 20000);
    topicManager->register_topic< haomo::hios::GearBoxInfoPb >("gear_rx", 20000);

    //设置值
    haomo::hios::BrakeInfoPb breakMsg;
    breakMsg.set_brake_pedal_status(true);
    breakMsg.set_pedal_output(333.00);
    breakMsg.set_brake_enable(true);
    topicManager->get_topic_msg("break_rx")->update_topic(&breakMsg);

    haomo::hios::GearBoxInfoPb gearMsg;
    gearMsg.set_gearbox_position_display(haomo::hios::GearBoxInfoPb_GearNum_GEAR_PARKING);
    topicManager->get_topic_msg("gear_rx")->update_topic(&gearMsg);

    while (true){
        topicManager->publish_topic();
    }
    return 0;
}
