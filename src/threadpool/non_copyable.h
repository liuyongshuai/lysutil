/*************************************************************************
 * File:	non_copyable.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-10 17:43
 ************************************************************************/
#ifndef _LYSUTIL_NON_COPYABLE_H_202307101743_
#define _LYSUTIL_NON_COPYABLE_H_202307101743_

namespace lysutil{
    namespace threadpool{
        class NonCopyable{
        protected:
            NonCopyable() = default;

            ~NonCopyable() = default;

            NonCopyable(const NonCopyable &) = delete;

            NonCopyable &operator=(const NonCopyable &) = delete;
        };
    }
}
#endif //_LYSUTIL_NON_COPYABLE_H_202307101743_
