/*************************************************************************
 * File:	exception.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 18:46
 ************************************************************************/
#ifndef _LYSUTIL_EXCEPTION_H_202307101846_
#define _LYSUTIL_EXCEPTION_H_202307101846_

#include <exception>
#include <string>

namespace lysutil{
    namespace threadpool{
        class TException : public std::exception{
        public:
            TException() : message_(){}

            TException(const std::string &message) : message_(message){}

            virtual ~TException()

            noexcept override =
            default;

            const char *what() const

            noexcept override{
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
#endif //_LYSUTIL_EXCEPTION_H_202307101846_
