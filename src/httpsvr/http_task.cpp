/*************************************************************************
 * File:	http_task.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 14:09
 ************************************************************************/
#include<iostream>
#include "httpsvr/http_task.h"

namespace lysutil{
    namespace httpsvr{
        void httpTask::run(){
            std::shared_ptr< httpRouter > routers_ = httpRouter::get_instance();
            std::string rcvBuf;

            //响应信息
            httpResponse httpRsp;
            std::string rspStr;

            //开始读数据
            size_t read_size;
            if (this->readData(rcvBuf, &read_size) > 0){
                httpRsp.setStatus(NOT_IMPLEMENTED);
                httpRsp.setBody(httpStatusDesc.find(NOT_IMPLEMENTED)->second);
                httpRsp.getRsp(rspStr);
                write(this->sockfd_, rspStr.c_str(), rspStr.size());
                close(this->sockfd_);
                return;
            }

            //解析请求
            httpRequest httpReq;
            httpReq.parseBody(rcvBuf.c_str(), rcvBuf.size());



            //如果header里有expect：100-continue表示后续还有数据，server端要返回一个响应给client才会继续发送下面的数据
            std::string expect;
            std::string contentLengthStr;
            httpReq.getHeader("expect", expect);
            httpReq.getHeader("content-length", contentLengthStr);
            uint64_t contentLength = lysutil::comutils::strUtils::safe_strtoull(contentLengthStr.c_str());
            if (expect == "100-continue"){
                httpRsp.setStatus(CONTINUE);
                httpRsp.setBody(httpStatusDesc.find(CONTINUE)->second);
                httpRsp.getRsp(rspStr);
                write(this->sockfd_, rspStr.c_str(), rspStr.size());

                //此时后续还有 contentLength 大小的数据，等一段时间即可
                size_t haveRead = 0;
                do{
                    //读数据时意外终止
                    if (this->readData(rcvBuf, &read_size) > 0){
                        httpRsp.setStatus(NOT_IMPLEMENTED);
                        httpRsp.setBody(httpStatusDesc.find(NOT_IMPLEMENTED)->second);
                        httpRsp.getRsp(rspStr);
                        write(this->sockfd_, rspStr.c_str(), rspStr.size());
                        close(this->sockfd_);
                        return;
                    }
                    haveRead += read_size;
                }while (haveRead >= contentLength);

                httpReq.reset();
                httpReq.parseBody(rcvBuf.c_str(), rcvBuf.size());
            }


            httpReq.client_ip = this->clientip_;


            //匹配路由信息
            std::map< std::string, std::string > args;
            std::shared_ptr< routerItem > r = routers_->matchRouter(httpReq.uri, args);
            if (r == nullptr){
                httpRsp.setStatus(NOT_FOUND);
                httpRsp.setBody(httpStatusDesc.find(NOT_FOUND)->second);
                httpRsp.getRsp(rspStr);
                write(this->sockfd_, rspStr.c_str(), rspStr.size());
                close(this->sockfd_);
                return;
            }


            //如果匹配成功且有参数的话
            if (!args.empty()){
                std::map< std::string, std::string >::const_iterator iter;
                for (iter = args.begin(); iter != args.end(); iter++){
                    std::vector< std::string > tmpa;
                    tmpa.push_back(iter->second);
                    httpReq.args[iter->first] = tmpa;
                }
            }

            //执行请求
            httpReq.printReq();
            httpReq.static_file = r->static_file;
            r->func(httpReq, httpRsp);

            //回写请求句柄
            httpRsp.getRsp(rspStr);
            write(this->sockfd_, rspStr.c_str(), rspStr.size());
            close(this->sockfd_);
        }

        int httpTask::readData(std::string &data, size_t *read_size) const{
            char buf[MAX_SOCK_BUF_SIZE];
            //读到的字节数
            size_t nread;
            do{
                bzero(buf, MAX_SOCK_BUF_SIZE);
                nread = read(this->sockfd_, buf, MAX_SOCK_BUF_SIZE);
                *read_size += nread;
                if (nread > 0){//读到数据
                    data.append(buf, nread);
                    //此处的数据读取方式较为暴力，假设所有的数据一次性到达
                    if (nread < MAX_SOCK_BUF_SIZE){
                        break;
                    }
                }
                else if (nread < 0){//读取失败
                    if (errno == EAGAIN){//没有数据了
                        std::cout << "[EAGAIN]read:" << nread << ",errno:" << errno << ",no data" << std::endl;
                        break;
                    }
                    else if (errno == EINTR){//可能被内部中断信号打断,经过验证对非阻塞socket并未收到此错误,应该可以省掉该步判断
                        std::cout << "[EINTR]read:" << nread << ",errno:" << errno << ",interrupt" << std::endl;
                        break;
                    }
                    else{//客户端主动关闭
                        std::cout << "[other]read:" << nread << ",errno:" << errno << ",peer error" << std::endl;
                        return 1;
                    }
                }
                else if (nread == 0){//客户端主动关闭
                    std::cout << "ReadThread, read:" << nread << ",errno:" << errno << ",peer close" << std::endl;
                    return 1;
                }
            }while (true);
            return 0;
        }
    }
} //namespace lysutil
