/*************************************************************************
 * File:	logger.h
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2023-08-14 12:35
 ************************************************************************/
#ifndef _LYSUTIL_LOGGER_H_202308141235_
#define _LYSUTIL_LOGGER_H_202308141235_

#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/details/file_helper.h>
#include <spdlog/details/registry.h>
#include <mutex>

namespace spdlog {
    namespace sinks {

        template<class Mutex>
        class HourlyRotatedSink final : public base_sink<Mutex> {
        public:
            //create daily file sink which rotates on given time
            HourlyRotatedSink(const filename_t &base_filename): _base_filename(base_filename) {
                filename_t suffix;
                _rotation_tp = _next_rotation_tp(suffix);
                _file_helper.open(_base_filename + suffix);
            }

        protected:
            void _sink_it(const details::log_msg &msg) override {
                if (std::chrono::system_clock::now() >= _rotation_tp) {
                    filename_t suffix;
                    _rotation_tp = _next_rotation_tp(suffix);
                    _file_helper.open(_base_filename + suffix);
                }
                _file_helper.write(msg);
            }

            void _flush() override {
                _file_helper.flush();
            }

        private:
            std::chrono::system_clock::time_point _next_rotation_tp(filename_t &suffix) {
                auto now = std::chrono::system_clock::now();
                time_t tnow = std::chrono::system_clock::to_time_t(now);
                if (tnow % 3600 == 3599) { //小于1秒钟就归到下一小时去
                    tnow += 3601;
                } else {
                    tnow = tnow + 3600 - (tnow % 3600);
                }
                tm date = spdlog::details::os::localtime(tnow - 3600);
                char suffix_str[15];
                snprintf(suffix_str, 15, ".%04d%02d%02d%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday,date.tm_hour);
                suffix = suffix_str;
                return std::chrono::system_clock::from_time_t(tnow);
            }

            filename_t _base_filename;
            int _rotation_h;
            int _rotation_m;
            std::chrono::system_clock::time_point _rotation_tp;
            spdlog::details::file_helper _file_helper;
        };

    }
}

namespace lysutil {
    namespace comutils {
        class CommLogger {
        public:
            std::shared_ptr<spdlog::logger> s_error_logger;
            std::shared_ptr<spdlog::logger> s_info_logger;
            std::shared_ptr<spdlog::logger> s_debug_logger;

            CommLogger() {}

            void init(const std::string &logger_name, const std::string &log_dir) {
                std::string error_file_path = std::string("./") + log_dir + "/error.log";
                std::shared_ptr<spdlog::sinks::sink> error_sink = std::make_shared<spdlog::sinks::HourlyRotatedSink<std::mutex> >(error_file_path);
                s_error_logger = spdlog::details::registry::instance().create(logger_name + "_error", {error_sink});
                s_error_logger->set_pattern("[%l][%Y-%m-%dT%H:%M:%S.%e%z]%v");
                s_error_logger->set_level(spdlog::level::warn);
#ifdef __DEBUG_MODE__
                s_error_logger->flush_on(spdlog::level::warn);
#endif

                std::string info_file_path = std::string("./") + log_dir + "/info.log";
                std::shared_ptr<spdlog::sinks::sink> info_sink = std::make_shared<spdlog::sinks::HourlyRotatedSink<std::mutex> >(info_file_path);
                s_info_logger = spdlog::details::registry::instance().create(logger_name + "_info", {info_sink});
                s_info_logger->set_pattern("[%l][%Y-%m-%dT%H:%M:%S.%e%z]%v");
                s_info_logger->set_level(spdlog::level::info);
#ifdef __DEBUG_MODE__
                s_info_logger->flush_on(spdlog::level::info);
#endif

                std::string debug_file_path = std::string("./") + log_dir + "/debug.log";
                std::shared_ptr<spdlog::sinks::sink> debug_sink = std::make_shared<spdlog::sinks::HourlyRotatedSink<std::mutex> >(debug_file_path);
                s_debug_logger = spdlog::details::registry::instance().create(logger_name + "_debug", {debug_sink});
                s_debug_logger->set_pattern("[%l][%Y-%m-%dT%H:%M:%S.%e%z]%v");
#ifdef __DEBUG_MODE__
                s_debug_logger->set_level(spdlog::level::debug);
                s_debug_logger->flush_on(spdlog::level::debug);
#else
                s_debug_logger->set_level(spdlog::level::critical);
#endif
            }

            //刷新输出日志
            ~CommLogger() {
                this->s_info_logger->flush();
                this->s_error_logger->flush();
                this->s_debug_logger->flush();
            }

            static CommLogger *getInstance() {
                static CommLogger *inst = nullptr;
                if (inst == nullptr) {
                    inst = new lysutil::comutils::CommLogger();
                }
                return inst;
            }
        };
    }
}

#define DEBUG_LOG(fmt, ...) didi::map::common::BthreadLogger::getInstance()->s_debug_logger->debug( "[%s:%d] " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define INFO_LOG(fmt, ...) didi::map::common::BthreadLogger::getInstance()->s_info_logger->info( "[%s:%d] " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define WARNING_LOG(fmt, ...) didi::map::common::BthreadLogger::getInstance()->s_error_logger->warn( "[%s:%d] " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) didi::map::common::BthreadLogger::getInstance()->s_error_logger->error( "[%s:%d] " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define FATAL_LOG(fmt, ...) didi::map::common::BthreadLogger::getInstance()->s_error_logger->critical( "[%s:%d] " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)

#endif //_LYSUTIL_LOGGER_H_202308141235_
