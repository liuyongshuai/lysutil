/*************************************************************************
 * File:	global_conf.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-07-12 18:24
 ************************************************************************/
#ifndef _LYSUTIL_GLOBAL_CONF_H_202307121824_
#define _LYSUTIL_GLOBAL_CONF_H_202307121824_

#include<string>
#include<vector>
#include<map>
#include<memory>
#include<libgen.h>
#include "comutils/toml_parser.h"
#include "comutils/file_utils.h"

namespace lysutil{
    namespace httpsvr{
        class globalConf{
        public:
            static std::shared_ptr< globalConf > get_instance(){
                if (nullptr != instance_){
                    return instance_;
                }
                instance_ = std::shared_ptr< globalConf >(new globalConf);
                instance_->startup_second_timestamp = time(nullptr);
                return instance_;
            }

            /**
             * 解析配置文件
             */
            bool parseConf(const std::string &conf_file);

            /**
             * 监听的端口号
             */
            uint16_t listen_port;

            /**
             * 静态文件的目录，绝对路径，后面不带斜杠
             */
            std::string static_resource_dir;

            /**
             * 线程池worker的数量
             */
            uint16_t thread_worker_count;

            /**
             * 最多允许的空闲worker
             */
            uint16_t max_pending_worker_count;

            /**
             * 启动时的时间戳
             */
            uint64_t startup_second_timestamp;
        private:
            globalConf(){};
            globalConf(const globalConf &) = delete;
            globalConf &operator=(const globalConf &) = delete;
            static std::shared_ptr< globalConf > instance_;
        };
    }
} //namespace lysutil


#endif //_LYSUTIL_GLOBAL_CONF_H_202307121824_
