/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef _THRIFT_CONCURRENCY_TIMERMANAGER_H_
#define _THRIFT_CONCURRENCY_TIMERMANAGER_H_ 1

#include <thrift/concurrency/Monitor.h>
#include <thrift/concurrency/ThreadFactory.h>

#include <memory>
#include <map>

namespace apache{
    namespace thrift{
        namespace concurrency{

            /**
             * Timer Manager
             *
             * This class dispatches timer tasks when they fall due.
             *
             * @version $Id:$
             */
            class TimerManager{

            public:
                class Task;

                typedef std::weak_ptr <Task> Timer;

                TimerManager();

                virtual ~TimerManager();

                virtual std::shared_ptr< const ThreadFactory > threadFactory() const;

                virtual void threadFactory(std::shared_ptr< const ThreadFactory > value);

                /**
                 * Starts the timer manager service
                 *
                 * @throws IllegalArgumentException Missing thread factory attribute
                 */
                virtual void start();

                /**
                 * Stops the timer manager service
                 */
                virtual void stop();

                virtual size_t taskCount() const;

                /**
                 * Adds a task to be executed at some time in the future by a worker thread.
                 *
                 * @param task The task to execute
                 * @param timeout Time in milliseconds to delay before executing task
                 * @return Handle of the timer, which can be used to remove the timer.
                 */
                virtual Timer add(std::shared_ptr <Runnable> task, const std::chrono::milliseconds &timeout);

                Timer add(std::shared_ptr <Runnable> task, uint64_t timeout){ return add(task, std::chrono::milliseconds(timeout)); }

                /**
                 * Adds a task to be executed at some time in the future by a worker thread.
                 *
                 * @param task The task to execute
                 * @param abstime Absolute time in the future to execute task.
                 * @return Handle of the timer, which can be used to remove the timer.
                 */
                virtual Timer add(std::shared_ptr <Runnable> task, const std::chrono::time_point <std::chrono::steady_clock> &abstime);

                /**
                 * Removes a pending task
                 *
                 * @param task The task to remove. All timers which execute this task will
                 * be removed.
                 * @throws NoSuchTaskException Specified task doesn't exist. It was either
                 *                             processed already or this call was made for a
                 *                             task that was never added to this timer
                 *
                 * @throws UncancellableTaskException Specified task is already being
                 *                                    executed or has completed execution.
                 */
                virtual void remove(std::shared_ptr <Runnable> task);

                /**
                 * Removes a single pending task
                 *
                 * @param timer The timer to remove. The timer is returned when calling the
                 * add() method.
                 * @throws NoSuchTaskException Specified task doesn't exist. It was either
                 *                             processed already or this call was made for a
                 *                             task that was never added to this timer
                 *
                 * @throws UncancellableTaskException Specified task is already being
                 *                                    executed or has completed execution.
                 */
                virtual void remove(Timer timer);

                enum STATE{
                    UNINITIALIZED, STARTING, STARTED, STOPPING, STOPPED
                };

                virtual STATE state() const;

            private:
                std::shared_ptr< const ThreadFactory > threadFactory_;

                friend class Task;

                std::multimap <std::chrono::time_point< std::chrono::steady_clock >, std::shared_ptr< Task >> taskMap_;
                size_t taskCount_;
                Monitor monitor_;
                STATE state_;

                class Dispatcher;

                friend class Dispatcher;

                std::shared_ptr <Dispatcher> dispatcher_;
                std::shared_ptr <Thread> dispatcherThread_;
                using task_iterator =
                decltype(taskMap_)
                ::iterator;
                typedef std::pair <task_iterator, task_iterator> task_range;
            };
        }
    }
} // apache::thrift::concurrency

#endif // #ifndef _THRIFT_CONCURRENCY_TIMERMANAGER_H_
