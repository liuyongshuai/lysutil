/*************************************************************************
 * File:	monitor.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 17:46
 ************************************************************************/
#ifndef _LYSUTIL_MONITOR_H_202307101746_
#define _LYSUTIL_MONITOR_H_202307101746_

#include<string>
#include<vector>
#include<map>
#include<errno.h>
#include "non_copyable.h"
#include "timed_mutex.h"
#include "exception.h"

namespace lysutil{
    namespace threadpool{
        class Monitor : NonCopyable{
        public:
            Monitor();

            explicit Monitor(TimedMutex *mutex);

            explicit Monitor(Monitor *monitor);

            TimedMutex *mutex();

            void lock();

            void unlock();

            /**
             * 等待一个相对的时间
             */
            int waitForTimeRelative(const std::chrono::milliseconds &timeout);

            int waitForTimeRelative(uint64_t timeout_ms);

            /**
             * 等待一个绝对时间
             */
            int waitForTime(const std::chrono::time_point< std::chrono::steady_clock > &abstime);

            int waitForever();

            /**
             * 抛异常版本
             */
            void wait(const std::chrono::milliseconds &timeout);

            void wait(uint64_t timeout_ms = 0ULL);

            /**
             * 唤醒一个线程
             */
            void notify();

            /**
             * 唤醒所有线程
             */
            void notifyAll();

        private:
            void init(TimedMutex *mutex){
                this->mutex_ = mutex;
            }

            /**
             * 自己的锁，析构时要释放
             */
            const std::unique_ptr< TimedMutex > ownedMutex_;

            /**
             * 外来的锁，不要释放
             */
            TimedMutex *mutex_;

            /**
             * 条件变量
             */
            std::condition_variable_any conditionVariable_;
        };
    }
} //namespace lysutil


#endif //_LYSUTIL_MONITOR_H_202307101746_
