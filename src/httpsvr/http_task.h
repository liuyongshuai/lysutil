/*************************************************************************
 * File:	http_task.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 14:09
 ************************************************************************/
#ifndef _LYSUTIL_HTTP_TASK_H_202307121409_
#define _LYSUTIL_HTTP_TASK_H_202307121409_

#include<string>
#include <utility>
#include<vector>
#include<map>
#include "thriftconcurrency/Thread.h"
#include "httpsvr/http_router.h"
#include "httpsvr/http_common.h"
#include "httpsvr/http_request.h"
#include "httpsvr/http_response.h"

namespace lysutil{
    namespace httpsvr{
        class httpTask : public apache::thrift::concurrency::Runnable{
        public:
            httpTask(int fd, std::string cip) : sockfd_(fd), clientip_(std::move(cip)){}

            virtual ~httpTask(){}

            void run();

        private:
            int sockfd_;
            std::string clientip_;

            int readData(std::string &data, size_t *read_size) const;
        };
    }
} //namespace lysutil


#endif //_LYSUTIL_HTTP_TASK_H_202307121409_
