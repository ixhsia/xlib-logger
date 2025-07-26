#ifndef LOGGER_IMPL_HPP
#define LOGGER_IMPL_HPP
#include <atomic>
#include <string>
#include <fstream>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "logger/sink.hpp"
#include "toml.hpp"
#include "definations.hpp"

/// 仅仅是想实现类似独热编码的效果，只是不愿意 '#define'
namespace xlib::Flags::ShowSinkFlag {
    constexpr uint8_t Show_None            = 0b0000'0000;
    constexpr uint8_t Sink_CommandLine     = 0b0000'0001;
    constexpr uint8_t Sink_File            = 0b0000'0010;
    constexpr uint8_t Sink_All             = Sink_CommandLine | Sink_File;
}

namespace xlib::logger {
    enum SetStatus
    {
        Disabled = 0,
        Enabled,
    };
    enum LogLevel {
        XLIB_LOG_LEVEL_DEBUG = 0,
        XLIB_LOG_LEVEL_INFO,
        XLIB_LOG_LEVEL_WARNING,
        XLIB_LOG_LEVEL_ERROR,
        XLIB_LOG_LEVEL_FATAL,
    };
    enum LogTimeStyle
    {
        YYYY_md_HMS_withNone = 0,
        YYYY_md_HMS_withSlash,
        YYYY_md_HMS_withSlashSlash,
        YYYY_md_HMS_withDash,
        YYYY_md_HMS_withDashDash,
        YYYY_md_HMS_withUnderline,
        YYYY_md_HMS_withUnderlineLine,
        YYYY_md_HMS_withDot,
        YYYY_md_HMS_withDotDot,

        yy_md_HMS_withNone,
        yy_md_HMS_withSlash,
        yy_md_HMS_withSlashSlash,
        yy_md_HMS_withDash,
        yy_md_HMS_withDashDash,
        yy_md_HMS_withUnderline,
        yy_md_HMS_withUnderlineLine,
        yy_md_HMS_withDot,
        yy_md_HMS_withDotDot,
    };

    std::string _get_log_level_str(uint8_t _level);

    struct LoggerEntity {
        LogLevel    level;
        std::string timestamp;
        std::string title;
        std::string information;

        void clean() {
            level = XLIB_LOG_LEVEL_DEBUG;
            timestamp.clear();
            title.clear();
            information.clear();
        }
        [[nodiscard]] std::string format() const {
            std::ostringstream oss;
            const std::string level_str = _get_log_level_str(static_cast<uint8_t>(level));
            oss <<  level_str      << " "
                <<  timestamp      << " "
                <<  title          << " > "
                <<  information;
            return  oss.str();
        }
    };

    void _command_print(const LoggerEntity& _entity);

    class LogWriter {
        // 日志基本变量
        LoggerEntity entity_ {};
        std::ofstream file_{};
        std::vector<Sink_Base> sinks_{};
        uint8_t show_flag_ = 0x00;
        bool is_thread_log_enabled_ = true;
        LogLevel log_level_line_ = XLIB_LOG_LEVEL_DEBUG;

        // 多线程配置
        std::deque<std::string> thread_queue_{};
        std::mutex thread_mutex_;
        std::condition_variable thread_cv_;
        std::atomic<bool> thread_is_run_ = false;
        std::thread thread_forWriting_;

        // toml解析器
        toml::table parse_{};

        // 日志时间格式字符串
        std::string log_time_style_strs_[18] = {
            "%Y%m%d %H:%M:%S",       //YYYY_md_HMS_withNone,
            "%Y/%m/%d %H:%M:%S",     //YYYY_md_HMS_withSlash,
            "%Y/%m/%d/%H:%M:%S",     //YYYY_md_HMS_withSlashSlash,
            "%Y-%m-%d %H:%M:%S",     //YYYY_md_HMS_withDash,
            "%Y-%m-%d-%H:%M:%S",     //YYYY_md_HMS_withDashDash,
            "%Y_%m_%d %H:%M:%S",     //YYYY_md_HMS_withUnderline,
            "%Y_%m_%d_%H:%M:%S",     //YYYY_md_HMS_withUnderlineLine
            "%Y.%m.%d %H:%M:%S",     //YYYY_md_HMS_withDot,
            "%Y.%m.%d.%H:%M:%S",     //YYYY_md_HMS_withDotDot,

            "%y%m%d %H:%M:%S",       //yy_md_HMS_withNone,
            "%y/%m/%d %H:%M:%S",     //yy_md_HMS_withSlash,
            "%y/%m/%d/%H:%M:%S",     //yy_md_HMS_withSlashSlash,
            "%y-%m-%d %H:%M:%S",     //yy_md_HMS_withDash,
            "%y-%m-%d-%H:%M:%S",     //yy_md_HMS_withDashDash,
            "%y_%m_%d %H:%M:%S",     //yy_md_HMS_withUnderline,
            "%y_%m_%d_%H:%M:%S",     //yy_md_HMS_withUnderlineLine,
            "%y.%m.%d %H:%M:%S",     //yy_md_HMS_withDot,
            "%y.%m.%d.%H:%M:%S",     //yy_md_HMS_withDotDot,
        };

    public:
        static std::unique_ptr<LogWriter> from_file_config(const std::string& _file_config);
        static std::unique_ptr<LogWriter> from_file_log(const std::string& _file_log);
        LogWriter() = default;
        void set_thread_ifEnabled();
        ~LogWriter();

        void set_logger_level_line(const LogLevel _level) {
            log_level_line_ = _level;
        }
        void set_logger_thread_isEnabled(const bool _enabled) {
            is_thread_log_enabled_ = _enabled;
        }

        void set_msg_level(const LogLevel _level) {
            entity_.level = _level;
        }
        void set_msg_title(const std::string& _title) {
            entity_.title = _title;
        }
        void set_information(const std::string& _information) {
            entity_.information = _information;
        }
        void set_show_flag(const uint8_t _flag) {
            show_flag_ = _flag;
        }
        void set_msg_timestamp(const std::string& _timestamp) {
            entity_.timestamp = _timestamp;
        }
        std::string set_msg_timestamp(LogTimeStyle _style);

    private:
        void init_from_file_log(const std::string& _file_log);
        void init_from_file_config(const std::string& _file_config);
        void write_ipt_impl();
        void thread_write_ipt_impl();

    public:
        void log();
        void log(LogLevel _level, const std::string& _tittle, const std::string& _info, LogTimeStyle _time_style = YYYY_md_HMS_withDash);
        void debug(const std::string& _tittle, const std::string& _info, LogTimeStyle _time_style = YYYY_md_HMS_withDash);
        void info(const std::string& _tittle, const std::string& _info, LogTimeStyle _time_style = YYYY_md_HMS_withDash);
        void warning(const std::string& _tittle, const std::string& _info, LogTimeStyle _time_style = YYYY_md_HMS_withDash);
        void error(const std::string& _tittle, const std::string& _info, LogTimeStyle _time_style = YYYY_md_HMS_withDash);
        void fatal(const std::string& _tittle, const std::string& _info, LogTimeStyle _time_style = YYYY_md_HMS_withDash);

    };
}
#endif // LOGGER_IMPL_HPP