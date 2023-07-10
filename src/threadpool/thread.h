/*************************************************************************
 * File:	thread.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 16:45
 ************************************************************************/
#ifndef _LYSUTILS_THREAD_H_202307101645_
#define _LYSUTILS_THREAD_H_202307101645_

#include<string>
#include<vector>
#include<map>
#include<memory>
#include<thread>
#include "common.h"
#include "runnable.h"
#include "monitor.h"
#include "guard.h"

namespace lysutil{
    namespace threadpool{
        class Thread : public std::enable_shared_from_this< Thread >{

        public:
            enum STATE{
                UNINITIALIZED, STARTING, STARTED, STOPPING, STOPPED
            };

            typedef void (*thread_funct_t)(std::shared_ptr< Thread >);

            static void threadMain(std::shared_ptr< Thread > thread);

            static inline bool is_current(std::thread::id t){
                return t == std::this_thread::get_id();
            }

            static inline std::thread::id get_current(){
                return std::this_thread::get_id();
            }

            Thread(bool detached, std::shared_ptr< Runnable > runnable)
                    : monitor_(new Monitor), state_(UNINITIALIZED), detached_(detached){
                this->_runnable = runnable;
            }

            virtual ~Thread(){
                if (!this->detached_ && this->thread_->joinable()){
                    try{
                        this->join();
                    }
                    catch (...){
                        // We're really hosed.
                    }
                }
            }

            STATE getState() const{
                Guard g(this->monitor_->mutex());
                return this->state_;
            }

            void setState(STATE newState){
                Guard g(this->monitor_->mutex());
                this->state_ = newState;

                // unblock start() with the knowledge that the thread has actually
                // started running, which avoids a race in detached threads.
                if (newState == STARTED){
                    this->monitor_->notify();
                }
            }

            /**
             * 开始运行线程
             */
            virtual void start(){
                if (getState() != UNINITIALIZED){
                    return;
                }

                std::shared_ptr< Thread > selfRef = shared_from_this();
                this->setState(STARTING);

                Guard g(this->monitor_->mutex());
                this->thread_ = std::unique_ptr< std::thread >(new std::thread(getThreadFunc(), selfRef));
                if (this->detached_){
                    this->thread_->detach();
                }


                this->monitor_->waitForTimeRelative(0);
            }

            /**
             * 等待线程运行完
             */
            virtual void join(){
                if (!this->detached_ && this->state_ != UNINITIALIZED){
                    this->thread_->join();
                }
            }

            /**
             * 当前的线程ID
             */
            std::thread::id getId() const{
                return this->thread_.get() ? this->thread_->get_id() : std::thread::id();
            }

            /**
             * 当前的运行对象
             */
            std::shared_ptr< Runnable > runnable() const{
                return this->_runnable;
            }

        protected:
            thread_funct_t getThreadFunc(){
                return threadMain;
            }

        private:
            std::shared_ptr< Runnable > _runnable;
            std::unique_ptr< std::thread > thread_;
            std::shared_ptr< Monitor > monitor_;
            STATE state_;
            bool detached_;
        };
    }
} //namespace lysutils


#endif //_LYSUTILS_THREAD_H_202307101645_
