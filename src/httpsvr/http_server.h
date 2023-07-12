/*************************************************************************
 * File:	http_server.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:19
 ************************************************************************/
#ifndef _TRANSTOPIC_HTTP_SERVER_H_202306271119_
#define _TRANSTOPIC_HTTP_SERVER_H_202306271119_

#include<string>
#include<vector>
#include<map>
#include "httpsvr/http_common.h"
#include "httpsvr/http_router.h"
#include "httpsvr/http_task.h"

namespace lysutil{
    namespace httpsvr{
        class httpServer{
        public:
            static std::shared_ptr< httpServer > get_instance(){
                if (nullptr != instance_){
                    return instance_;
                }
                instance_ = std::shared_ptr< httpServer >(new httpServer);
                instance_->routers_ = httpRouter::get_instance();
                instance_->threadManager_ = apache::thrift::concurrency::ThreadManager::newSimpleThreadManager(64, 4);
                std::shared_ptr< apache::thrift::concurrency::ThreadFactory > threadFactory = std::make_shared< apache::thrift::concurrency::ThreadFactory >();
                instance_->threadManager_->threadFactory(threadFactory);
                instance_->threadManager_->start();
                return instance_;
            }

            /**
             * 设置端口号
             */
            void setPort(uint16_t port);

            /**
             * 启动函数
             */
            void start();


            ~httpServer();

        private:
            httpServer(){};
            httpServer(const httpServer &) = delete;
            httpServer &operator=(const httpServer &) = delete;
            static std::shared_ptr< httpServer > instance_;

            //端口号
            uint16_t port_{80};
            std::shared_ptr< httpRouter > routers_{nullptr};

            //监听句柄
            int listenfd_;
            int epollfd_;

            //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
            struct epoll_event ev_;

            //监听事件数组
            struct epoll_event events_[MAX_EVENTS];

            //线程管理
            std::shared_ptr< apache::thrift::concurrency::ThreadManager > threadManager_;

            /**
             * 开始监听
             */
            void listen();

            /**
             * 接收请求
             */
            void accept();

            /**
             * 设置句柄非阻塞属性
             */
            void setNonBlocking(int sockfd);

            /**
             * 设置地址重用
             */
            void setReUseAddr(int sockfd);
        };
    }
} //namespace Project


#endif //_TRANSTOPIC_HTTP_SERVER_H_202306271119_
