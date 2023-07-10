/*************************************************************************
 * File:	timedmutex.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 17:35
 ************************************************************************/
#ifndef _LYSUTIL_TIMEDMUTEX_H_202307101735_
#define _LYSUTIL_TIMEDMUTEX_H_202307101735_

#include<string>
#include<vector>
#include<map>
#include "common.h"

namespace lysutil{
    namespace threadpool{
        class TimedMutex{
        public:
            TimedMutex();

            ~TimedMutex() = default;

            void lock() const;

            bool trylock() const;

            bool timedlock(int64_t milliseconds) const;

            void unlock() const;

            std::timed_mutex *getTimedMutex() const;

        private:
            TimedMutex(const TimedMutex &) = delete;

            TimedMutex &operator=(const TimedMutex &) = delete;

            std::shared_ptr< std::timed_mutex > mutex_;
        };
    }
} //namespace lysutil


#endif //_LYSUTIL_TIMEDMUTEX_H_202307101735_
