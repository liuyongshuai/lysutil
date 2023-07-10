/*************************************************************************
 * File:	timedmutex.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 17:35
 ************************************************************************/
#include<iostream>
#include "timedmutex.h"

namespace lysutil{
    namespace threadpool{
        std::timed_mutex *TimedMutex::getTimedMutex() const{
            return this->mutex_.get();
        }

        void TimedMutex::lock() const{
            this->mutex_->lock();
        }

        bool TimedMutex::trylock() const{
            return this->mutex_->try_lock();
        }

        bool TimedMutex::timedlock(int64_t ms) const{
            return this->mutex_->try_lock_for(std::chrono::milliseconds(ms));
        }

        void TimedMutex::unlock() const{
            this->mutex_->unlock();
        }

    }
} //namespace lysutil
