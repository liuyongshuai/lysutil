/*************************************************************************
 * File:	thread.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 16:45
 ************************************************************************/
#include<iostream>
#include "thread.h"

namespace lysutil{
    namespace threadpool{
        void Thread::threadMain(std::shared_ptr< Thread > thread){
            thread->setState(STARTED);
            thread->runnable()->run();
            if (thread->getState() != STOPPING && thread->getState() != STOPPED){
                thread->setState(STOPPING);
            }
        }
    }
} //namespace lysutils
