/*************************************************************************
 * File:	wsocket_test.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-08-11 13:12
 ************************************************************************/
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<iostream>
#include "websocketpp/client.hpp"
#include <websocketpp/config/asio_client.hpp>

int main(int argc, char *argv[]) {
    std::string uri = "ws://localhost:9002"; // WebSocket服务器地址
    websocketpp::client<websocketpp::config::asio_client> client; // 客户端对象
    try {
        client.set_access_channels(websocketpp::log::alevel::none);
        client.set_error_channels(websocketpp::log::elevel::warn);
        // 设置连接成功回调函数
        client.init_asio();
        client.set_open_handler([&client](websocketpp::connection_hdl hdl) {
            std::cout << "Connected to server." << std::endl;
            client.send(hdl, "Hello, server!", websocketpp::frame::opcode::text);
        });
        // 设置接收消息回调函数
        client.set_message_handler(
                [](websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type msg) {
                    std::cout << "Received message: " << msg.get_payload() << std::endl;
                });
        // 连接WebSocket服务器
        websocketpp::lib::error_code ec;
        auto con = client.get_connection(uri, ec);
        if (ec) {
            std::cout << "Failed to get connection: " << ec.message() << std::endl;
            return -1;
        }
        client.connect(con);
        // 运行事件循环，直到连接关闭或出现错误
        client.run();
    } catch (const std::exception &e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}