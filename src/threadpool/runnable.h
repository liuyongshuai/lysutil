/*************************************************************************
 * File:	runnable.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-06 16:49
 ************************************************************************/
#ifndef _PFSM_LOGSIM_RUNNABLE_H_202307061649_
#define _PFSM_LOGSIM_RUNNABLE_H_202307061649_

#include<memory>
#include"common.h"

namespace lysutil{
    namespace threadpool{

        class Thread;

        class Runnable{

        public:
            virtual ~Runnable() = default;

            virtual void run() = 0;

            /**
             * Gets the thread object that is hosting this runnable object  - can return
             * an empty boost::shared pointer if no references remain on that thread object
             */
            virtual std::shared_ptr< Thread > thread(){
                return this->thread_.lock();
            }

            /**
             * Sets the thread that is executing this object.  This is only meant for
             * use by concrete implementations of Thread.
             */
            virtual void thread(std::shared_ptr< Thread > value){
                this->thread_ = value;
            }

        private:
            std::weak_ptr< Thread > thread_;
        };

    }
}
#endif //_PFSM_LOGSIM_RUNNABLE_H_202307061649_
