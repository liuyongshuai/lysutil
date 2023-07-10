/*************************************************************************
 * File:	guard.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 17:42
 ************************************************************************/
#ifndef _LYSUTIL_GUARD_H_202307101742_
#define _LYSUTIL_GUARD_H_202307101742_

#include "non_copyable.h"
#include "timedmutex.h"

namespace lysutil{
    namespace threadpool{
        class Guard : NonCopyable{
        public:
            Guard(const TimedMutex *value, int64_t timeout = 0) : mutex_(value){
                if (timeout == 0){
                    value->lock();
                }
                else if (timeout < 0){
                    if (!value->trylock()){
                        mutex_ = nullptr;
                    }
                }
                else{
                    if (!value->timedlock(timeout)){
                        mutex_ = nullptr;
                    }
                }
            }

            ~Guard(){
                if (mutex_){
                    mutex_->unlock();
                }
            }

            operator bool() const{
                return (mutex_ != nullptr);
            }

        private:
            const TimedMutex *mutex_;
        };
    }
}
#endif //_LYSUTIL_GUARD_H_202307101742_
