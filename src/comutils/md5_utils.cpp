/*************************************************************************
 * File:	md5_utils.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 16:24
 ************************************************************************/
#include<iostream>
#include "comutils/md5_utils.h"

#define F(x, y, z)            ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)            ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)            ((x) ^ (y) ^ (z))
#define I(x, y, z)            ((y) ^ ((x) | ~(z)))

#define STEP(f, a, b, c, d, x, t, s) \
    (a) += f((b), (c), (d)) + (x) + (t); \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
    (a) += (b);

#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
# define SET(n) (*((uint32_t *)&ptr[(n) * 4]))
# define GET(n) SET(n)
#else
# define SET(n) \
    (this->block[(n)] = \
    (uint32_t)ptr[(n) * 4] | \
    ((uint32_t)ptr[(n) * 4 + 1] << 8) | \
    ((uint32_t)ptr[(n) * 4 + 2] << 16) | \
    ((uint32_t)ptr[(n) * 4 + 3] << 24))
# define GET(n) \
    (this->block[(n)])
#endif

namespace lysutil{
    namespace comutils{

        void md5Utils::init(){
            this->a_ = 0x67452301;
            this->b_ = 0xefcdab89;
            this->c_ = 0x98badcfe;
            this->d_ = 0x10325476;
            this->lo_ = 0;
            this->hi_ = 0;
        }

        //转换
        void md5Utils::make_digest(char *md5str, const unsigned char *digest){
            static const char hexits[17] = "0123456789abcdef";
            int i;
            for (i = 0; i < 16; i++){
                md5str[i * 2] = hexits[digest[i] >> 4];
                md5str[(i * 2) + 1] = hexits[digest[i] & 0x0F];
            }
            md5str[32] = '\0';
        }

        const void *md5Utils::body(const void *data, size_t size){
            const unsigned char *ptr;
            uint32_t a, b, c, d;
            uint32_t saved_a, saved_b, saved_c, saved_d;

            ptr = (unsigned char *) data;

            a = this->a_;
            b = this->b_;
            c = this->c_;
            d = this->d_;

            do{
                saved_a = a;
                saved_b = b;
                saved_c = c;
                saved_d = d;

                /* Round 1 */
                STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
                STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
                STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
                STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
                STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
                STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
                STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
                STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
                STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
                STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
                STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
                STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
                STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
                STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
                STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
                STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)

                /* Round 2 */
                STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
                STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
                STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
                STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
                STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
                STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
                STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
                STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
                STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
                STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
                STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
                STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
                STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
                STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
                STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
                STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

                /* Round 3 */
                STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
                STEP(H, d, a, b, c, GET(8), 0x8771f681, 11)
                STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
                STEP(H, b, c, d, a, GET(14), 0xfde5380c, 23)
                STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
                STEP(H, d, a, b, c, GET(4), 0x4bdecfa9, 11)
                STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
                STEP(H, b, c, d, a, GET(10), 0xbebfbc70, 23)
                STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
                STEP(H, d, a, b, c, GET(0), 0xeaa127fa, 11)
                STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
                STEP(H, b, c, d, a, GET(6), 0x04881d05, 23)
                STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
                STEP(H, d, a, b, c, GET(12), 0xe6db99e5, 11)
                STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
                STEP(H, b, c, d, a, GET(2), 0xc4ac5665, 23)

                /* Round 4 */
                STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
                STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
                STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
                STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
                STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
                STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
                STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
                STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
                STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
                STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
                STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
                STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
                STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
                STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
                STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
                STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

                a += saved_a;
                b += saved_b;
                c += saved_c;
                d += saved_d;

                ptr += 64;
            }while (size -= 64);

            this->a_ = a;
            this->b_ = b;
            this->c_ = c;
            this->d_ = d;

            return ptr;
        }

        void md5Utils::MD5Update(const void *data, size_t size){
            uint32_t saved_lo;
            uint32_t used, free;
            saved_lo = this->lo_;
            if ((this->lo_ = (saved_lo + size) & 0x1fffffff) < saved_lo){
                this->hi_++;
            }
            this->hi_ += size >> 29;
            used = saved_lo & 0x3f;
            if (used){
                free = 64 - used;
                if (size < free){
                    memcpy(&this->buffer_[used], data, size);
                    return;
                }
                memcpy(&this->buffer_[used], data, free);
                data = (unsigned char *) data + free;
                size -= free;
                body(this->buffer_, 64);
            }

            if (size >= 64){
                data = body(data, size & ~(size_t) 0x3f);
                size &= 0x3f;
            }
            memcpy(this->buffer_, data, size);
        }

        void md5Utils::MD5Final(unsigned char *result){
            uint32_t used, free;
            used = this->lo_ & 0x3f;
            this->buffer_[used++] = 0x80;
            free = 64 - used;
            if (free < 8){
                memset(&this->buffer_[used], 0, free);
                body(this->buffer_, 64);
                used = 0;
                free = 64;
            }

            memset(&this->buffer_[used], 0, free - 8);

            this->lo_ <<= 3;
            this->buffer_[56] = this->lo_;
            this->buffer_[57] = this->lo_ >> 8;
            this->buffer_[58] = this->lo_ >> 16;
            this->buffer_[59] = this->lo_ >> 24;
            this->buffer_[60] = this->hi_;
            this->buffer_[61] = this->hi_ >> 8;
            this->buffer_[62] = this->hi_ >> 16;
            this->buffer_[63] = this->hi_ >> 24;

            body(this->buffer_, 64);

            result[0] = this->a_;
            result[1] = this->a_ >> 8;
            result[2] = this->a_ >> 16;
            result[3] = this->a_ >> 24;
            result[4] = this->b_;
            result[5] = this->b_ >> 8;
            result[6] = this->b_ >> 16;
            result[7] = this->b_ >> 24;
            result[8] = this->c_;
            result[9] = this->c_ >> 8;
            result[10] = this->c_ >> 16;
            result[11] = this->c_ >> 24;
            result[12] = this->d_;
            result[13] = this->d_ >> 8;
            result[14] = this->d_ >> 16;
            result[15] = this->d_ >> 24;
        }

        //MD5值的字符串表示
        int md5Utils::MD5Str(const void *input, size_t inputLen, char *output, size_t outputLen){
            if (outputLen < 32){
                return -1;
            }
            unsigned char digest[16];
            MD5Raw(input, inputLen, digest, 16);
            make_digest(output, digest);
            if (outputLen > 32){
                output[32] = '\0';
            }
            return 0;
        }

        //MD5值的原始值
        int md5Utils::MD5Raw(const void *input, size_t inputLen, unsigned char *output, size_t outputLen){
            if (outputLen < 16){
                return -1;
            }
            init();
            MD5Update(input, inputLen);
            MD5Final(output);
            return 0;
        }

        //文件的MD5值的字符串表示
        int md5Utils::MD5FileStr(const char *file, char *output, size_t outputLen){
            if (outputLen < 32){
                return -1;
            }
            unsigned char digest[16];
            MD5FileRaw(file, digest, 16);
            make_digest(output, digest);
            if (outputLen > 32){
                output[32] = '\0';
            }
            return 0;
        }

        //文件的MD5值的原始值
        int md5Utils::MD5FileRaw(const char *file, unsigned char *output, size_t outputLen){
            if (outputLen < 16){
                return -1;
            }
            init();
            FILE *fp = fopen(file, "rb");
            if (fp == nullptr){
                perror("open file failed\n");
                return -1;
            }
            char buf[4096] = {0};
            while (!feof(fp)){
                fread(buf, 4096, 1, fp);
                MD5Update(buf, strlen(buf));
                bzero(buf, 4096);
            }
            if (ferror(fp)){
                fclose(fp);
                printf("read file %s failed\n", file);
                return -1;
            }
            fclose(fp);
            MD5Final(output);
            return 0;
        }
    }
} //namespace lysutil
