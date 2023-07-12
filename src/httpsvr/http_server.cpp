/*************************************************************************
 * File:	http_server.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-06-27 11:19
 ************************************************************************/
#include<iostream>
#include "httpsvr/http_server.h"

namespace lysutil{
    namespace httpsvr{
        std::shared_ptr< httpServer > httpServer::instance_ = nullptr;

        /**
         * 设置端口号
         */
        void httpServer::setPort(uint16_t port){
            this->port_ = port;
        }

        /**
         * 启动函数
         */
        void httpServer::start(){
            this->listen();
            while (true){
                this->accept();
            }
        }

        /**
         * 开始监听
         */
        void httpServer::listen(){
            if (this->listenfd_ > 0){
                close(this->listenfd_);
            }
            if (this->epollfd_ > 0){
                close(this->epollfd_);
            }

            //创建监听socket
            this->listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
            if (this->listenfd_ < 0){
                perror("create socket failed");
                exit(1);
            }

            //设置属性
            setNonBlocking(this->listenfd_);
            setReUseAddr(this->listenfd_);

            //创建epoll,对2.6.8以后的版本,其参数无效,只要大于0的数值就行,内核自己动态分配
            this->epollfd_ = epoll_create(MAX_EVENTS);
            if (this->epollfd_ < 0){
                perror("epoll_create failed");
                exit(1);
            }

            //设置与要处理的事件相关的文件描述符
            this->ev_.data.fd = this->listenfd_;
            //设置要处理的事件类型
            this->ev_.events = EPOLLIN | EPOLLET;
            //注册epoll事件
            int ret = epoll_ctl(this->epollfd_, EPOLL_CTL_ADD, this->listenfd_, &(this->ev_));
            if (ret != 0){
                perror("epoll_ctl");
                close(this->listenfd_);
                close(this->epollfd_);
                exit(1);
            }

            //监听
            struct sockaddr_in svraddr;
            bzero(&svraddr, sizeof(svraddr));
            svraddr.sin_family = AF_INET;
            svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
            svraddr.sin_port = htons(this->port_);
            bind(this->listenfd_, (sockaddr *) &svraddr, sizeof(svraddr));
            //监听,准备接收连接
            ret = ::listen(this->listenfd_, LISTEN_BACKLOG_SIZE);
            if (ret != 0){
                perror("listen");
                close(this->listenfd_);
                close(this->epollfd_);
                exit(1);
            }
            std::cout << "[" << __FUNCTION__ << "]success listenfd=" << this->listenfd_ << "\tport=" << this->port_ << std::endl;
        }

        /**
         * 接收请求
         */
        void httpServer::accept(){
            struct sockaddr_in cliaddr;
            socklen_t clilen;
            //接收到的连接socket临时变量
            int connfd;

            //临时变量,有多少个socket有事件
            int nfds, i;

            //等待epoll事件的发生,如果当前有信号的句柄数大于输出事件数组的最大大小,
            //超过部分会在下次epoll_wait时输出,事件不会丢
            nfds = epoll_wait(this->epollfd_, this->events_, MAX_EVENTS, 500);
            std::cout << "[" << __FUNCTION__ << "]nfds=" << nfds << std::endl;

            //处理所发生的所有事件
            for (i = 0; i < nfds; ++i){
                //是本监听socket上的事件
                if (this->events_[i].data.fd == this->listenfd_){
                    std::cout << "events:" << this->events_[i].events << ",errno:" << errno << std::endl;
                    //有连接到来
                    if (this->events_[i].events & EPOLLIN){
                        do{
                            clilen = sizeof(struct sockaddr);
                            connfd = ::accept(this->listenfd_, (sockaddr *) &cliaddr, &clilen);
                            if (connfd > 0){
                                //this->setNonBlocking(connfd);
                                char *clientIP = inet_ntoa(cliaddr.sin_addr);
                                std::cout << "accept:" << connfd << ",errno:" << errno << ",connect:" << clientIP << ":" << ntohs(cliaddr.sin_port) << std::endl;
                                std::shared_ptr< apache::thrift::concurrency::Runnable > task = std::shared_ptr< apache::thrift::concurrency::Runnable >(new httpTask(connfd, clientIP));
                                this->threadManager_->add(task);
                            }
                            else{
                                std::cout << "accept:" << connfd << ",errno:" << errno << std::endl;
                                //没有连接需要接收了
                                if (errno == EAGAIN){
                                    break;
                                }
                                else{
                                    //此时说明该描述字已经出错了,需要重新创建和监听
                                    this->listen();
                                }
                            }
                        }while (true);
                    }
                    else if (this->events_[i].events & EPOLLERR || this->events_[i].events & EPOLLHUP){//有异常发生
                        //此时说明该描述字已经出错了,需要重新创建和监听
                        this->listen();
                    }
                }
            }
        }

        /**
         * 设置句柄非阻塞属性
         */
        void httpServer::setNonBlocking(int sockfd){
            int opts;
            opts = fcntl(sockfd, F_GETFL);
            if (opts < 0){
                perror("fcntl(sock,GETFL)");
                exit(1);
            }
            opts = opts | O_NONBLOCK;
            if (fcntl(sockfd, F_SETFL, opts) < 0){
                perror("fcntl(sock,SETFL,opts)");
                exit(1);
            }
        }

        /**
         * 设置地址重用
         */
        void httpServer::setReUseAddr(int sockfd){
            int opt;
            opt = 1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(&opt)) < 0){
                perror("setsockopt");
                exit(1);
            }
        }

        /**
         * 释放资源
         */
        httpServer::~httpServer(){
            if (this->listenfd_ > 0){
                close(this->listenfd_);
            }
            if (this->epollfd_ > 0){
                close(this->epollfd_);
            }
        }
    }
} //namespace Project
