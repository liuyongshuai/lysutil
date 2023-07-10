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

#ifndef _THRIFT_CONCURRENCY_EXCEPTION_H_
#define _THRIFT_CONCURRENCY_EXCEPTION_H_ 1

#include <exception>
#include <string>

namespace apache{
    namespace thrift{
        namespace concurrency{
            class TException : public std::exception{
            public:
                TException() : message_(){}
                TException(const std::string &message) : message_(message){}
                virtual ~TException() noexcept override = default;
                const char *what() const noexcept override{
                    if (message_.empty()){
                        return "Default TException.";
                    }
                    else{
                        return message_.c_str();
                    }
                }

            protected:
                std::string message_;
            };

            class NoSuchTaskException : public TException{
            };

            class UncancellableTaskException : public TException{
            };

            class InvalidArgumentException : public TException{
            };

            class IllegalStateException : public TException{
            public:
                IllegalStateException() = default;
                IllegalStateException(const std::string &message) : TException(message){}
            };

            class TimedOutException : public TException{
            public:
                TimedOutException() : TException("TimedOutException"){};
                TimedOutException(const std::string &message) : TException(message){}
            };

            class TooManyPendingTasksException : public TException{
            public:
                TooManyPendingTasksException() : TException("TooManyPendingTasksException"){};
                TooManyPendingTasksException(const std::string &message) : TException(message){}
            };

            class SystemResourceException : public TException{
            public:
                SystemResourceException() = default;
                SystemResourceException(const std::string &message) : TException(message){}
            };
        }
    }
} // apache::thrift::concurrency

#endif // #ifndef _THRIFT_CONCURRENCY_EXCEPTION_H_
