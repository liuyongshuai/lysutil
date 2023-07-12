/*************************************************************************
 * File:	md5_utils.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 16:24
 ************************************************************************/
#ifndef _LYSUTIL_MD5_UTILS_H_202307121624_
#define _LYSUTIL_MD5_UTILS_H_202307121624_

#include<string>
#include<vector>
#include<map>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<stddef.h>

namespace lysutil{
    namespace comutils{
        class md5Utils{
        public:

            //MD5值的字符串表示
            int MD5Str(const void *input, size_t inputLen, char *output, size_t outputLen);

            //MD5值的原始值
            int MD5Raw(const void *input, size_t inputLen, unsigned char *output, size_t outputLen);

            //文件的MD5值的字符串表示
            int MD5FileStr(const char *file, char *output, size_t outputLen);

            //文件的MD5值的原始值
            int MD5FileRaw(const char *file, unsigned char *output, size_t outputLen);

        private:
            uint32_t lo_, hi_;
            uint32_t a_, b_, c_, d_;
            unsigned char buffer_[64];
            uint32_t block_[16];

            //转换
            void make_digest(char *md5str, const unsigned char *digest);

            const void *body(const void *data, size_t size);

            void MD5Update(const void *data, size_t size);

            void MD5Final(unsigned char *result);

            void init();
        };
    }
} //namespace lysutil


#endif //_LYSUTIL_MD5_UTILS_H_202307121624_
