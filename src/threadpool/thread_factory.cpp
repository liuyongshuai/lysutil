/*************************************************************************
 * File:	thread_factory.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 18:50
 ************************************************************************/
#include<iostream>
#include "thread_factory.h"

namespace lysutil{
    namespace threadpool{
        std::shared_ptr< Thread > ThreadFactory::newThread(std::shared_ptr< Runnable > runnable) const{
            std::shared_ptr< Thread > result = std::make_shared< Thread >(isDetached(), runnable);
            runnable->thread(result);
            return result;
        }

        std::thread::id ThreadFactory::getCurrentThreadId() const{
            return std::this_thread::get_id();
        }
    }
} //namespace lysutil
