/*************************************************************************
 * File:	thread_factory.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 18:50
 ************************************************************************/
#ifndef _LYSUTIL_THREAD_FACTORY_H_202307101850_
#define _LYSUTIL_THREAD_FACTORY_H_202307101850_

#include<string>
#include<vector>
#include<map>
#include "common.h"
#include "thread.h"

namespace lysutil{
    namespace threadpool{
        class ThreadFactory{
        public:
            ThreadFactory(bool detached = true) : detached_(detached){}

            ~ThreadFactory() = default;


            bool isDetached() const{
                return this->detached_;
            }


            void setDetached(bool detached){
                this->detached_ = detached;
            }

            /**
             * 创建一个新线程
             */
            std::shared_ptr< Thread > newThread(std::shared_ptr< Runnable > runnable) const;


            std::thread::id getCurrentThreadId() const;

        private:
            bool detached_;
        };
    }
} //namespace lysutil


#endif //_LYSUTIL_THREAD_FACTORY_H_202307101850_
