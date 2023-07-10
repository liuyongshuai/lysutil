/*************************************************************************
 * File:	monitor.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 17:46
 ************************************************************************/
#include<iostream>
#include "monitor.h"

namespace lysutil{
    namespace threadpool{
        Monitor::Monitor() : ownedMutex_(new TimedMutex), mutex_(nullptr), conditionVariable_(){
            this->init(this->ownedMutex_.get());
        }

        Monitor::Monitor(TimedMutex *mutex) : ownedMutex_(nullptr), mutex_(nullptr), conditionVariable_(){
            this->init(mutex);
        }

        Monitor::Monitor(Monitor *monitor) : ownedMutex_(nullptr), mutex_(nullptr), conditionVariable_(){
            this->init(monitor->mutex());
        }

        TimedMutex *Monitor::mutex(){
            return this->mutex_;
        }

        void Monitor::lock(){
            this->mutex_->lock();
        }

        void Monitor::unlock(){
            this->mutex_->unlock();
        }

        /**
         * 等待一个相对的时间
         */
        int Monitor::waitForTimeRelative(const std::chrono::milliseconds &timeout){
            if (timeout.count() == 0){
                return this->waitForever();
            }

            std::unique_lock< std::timed_mutex > lock(*(this->mutex_->getTimedMutex()), std::adopt_lock);
            bool timedout = (this->conditionVariable_.wait_for(lock, timeout) == std::cv_status::timeout);
            lock.release();
            return (timedout ? ETIMEDOUT : 0);
        }

        int Monitor::waitForTimeRelative(uint64_t timeout_ms){
            return this->waitForTimeRelative(std::chrono::milliseconds(timeout_ms));
        }

        /**
         * 等待一个绝对时间
         */
        int Monitor::waitForTime(const std::chrono::time_point< std::chrono::steady_clock > &abstime){
            std::unique_lock< std::timed_mutex > lock(*(this->mutex_->getTimedMutex()), std::adopt_lock);
            bool timedout = (this->conditionVariable_.wait_until(lock, abstime) == std::cv_status::timeout);
            lock.release();
            return (timedout ? ETIMEDOUT : 0);
        }

        int Monitor::waitForever(){
            std::unique_lock< std::timed_mutex > lock(*(this->mutex_->getTimedMutex()), std::adopt_lock);
            this->conditionVariable_.wait(lock);
            lock.release();
            return 0;
        }

        void Monitor::wait(const std::chrono::milliseconds &timeout){
            int result = waitForTimeRelative(timeout);
            if (result == ETIMEDOUT){
                throw TimedOutException();
            }
            else if (result != 0){
                throw TException("Monitor::wait() failed");
            }
        }

        void Monitor::wait(uint64_t timeout_ms){
            this->wait(std::chrono::milliseconds(timeout_ms));
        }

        /**
         * 唤醒一个线程
         */
        void Monitor::notify(){
            this->conditionVariable_.notify_one();
        }

        /**
         * 唤醒所有线程
         */
        void Monitor::notifyAll(){
            this->conditionVariable_.notify_all();
        }
    }
} //namespace lysutil
