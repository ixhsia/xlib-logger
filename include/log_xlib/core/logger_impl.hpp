#ifndef LOGGER_IMPL_HPP
#define LOGGER_IMPL_HPP
#include "definations.hpp"

#include <chrono>
#include <string>
#include <deque>
#include <iomanip>

#include "interface.hpp"
#include "sink_manager.hpp"

#ifdef EXTENSIONS_ALLOWED
#include "configuration/configer.hpp"
#include "sink/sink_factory.hpp"
#endif // EXTENSIONS_ALLOWED

namespace xlib::logger {
    enum SetStatus
    {
        Disabled = 0,
        Enabled,
    };
    /// 日志时间格式枚举
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

    /// @class LogWriter
    /// @brief 日志写入器类，负责处理日志的写入、格式化和多线程支持。
    class LogWriter {
        LoggerEntity entity_ {};
        SinkManager sink_manager_{};

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
        LogWriter() {
            std::unique_ptr<ILogSink> sink = std::make_unique<Sink_Command>();
            const auto config_param = new SinkDataStructure_CommandLine();
            sink->set_config(config_param);
            sink_manager_.registration(std::move(sink));
        }
        ~LogWriter() = default;

#ifdef EXTENSIONS_ALLOWED
        /// @brief 从配置文件加载日志写入器
        /// @param _file_config 配置文件路径
        void load_config(const std::string& _file_config);

        LogWriter& load_setting(uint8_t _show_flag, const std::any& _sink_content) {
            SinkFactory::create_sinks(_show_flag, _sink_content, sink_manager_);
            return *this;
        }
        LogWriter& load_setting(const SinkManagerInit& _init) {
            sink_manager_.init_manager(_init);
            return *this;
        }
#else
        LogWriter& load_setting(const LogLevel _level_filter) {
            const SinkManagerInit init{
                .is_thread_enabled = false,
                .level_filter = _level_filter,
            };
            sink_manager_.init_manager(init);
            return *this;
        }
#endif // EXTENSIONS_ALLOWED

        void set_msg_level(const LogLevel _level) {
            entity_.level = _level;
        }
        void set_msg_title(const std::string &_title) {
            entity_.title = _title;
        }
        void set_information(const std::string &_information) {
            entity_.information = _information;
        }
        void set_msg_timestamp(const std::string &_timestamp) {
            entity_.timestamp = _timestamp;
        }
        std::string set_msg_timestamp(const LogTimeStyle _style) {
            const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm tm = *std::localtime(&now);
            localtime_r(&now, &tm);

            std::ostringstream oss;
            oss << '(' << std::put_time(&tm, log_time_style_strs_[static_cast<uint8_t>(_style)].c_str()) << ')';
            entity_.timestamp = oss.str();
            return oss.str();
        }

        private:
#ifdef EXTENSIONS_ALLOWED
            void decoder_config_file(const std::string& _file_config);
            void if_found_config_file_failed();
#endif // EXTENSIONS_ALLOWED

    public:
        /// @brief 记录日志的主要接口函数, 无参数版本提交当前entity_结构体
        void log() {
            sink_manager_.update(entity_);
            entity_.clean();
        }
        /// @brief 记录日志的主要接口函数
        /// @param _level 日志级别
        /// @param _tittle 日志标题
        /// @param _info 日志信息
        /// @param _time_style 时间格式, 默认YYYY_mm_dd_HH_MM_SS_withDash
        void log(const LogLevel _level, const std::string& _tittle, const std::string& _info, const LogTimeStyle _time_style = YYYY_md_HMS_withDash) {
            entity_.clean();
            entity_ = {
                .level = _level,
                .timestamp = set_msg_timestamp(_time_style),
                .title = _tittle,
                .information = _info,
            };
            this->log();
        }

        /// @brief 记录日志的主要接口函数, 默认日志级别为DEBUG
        /// @param _tittle 日志标题
        /// @param _info 日志信息
        /// @param _time_style 时间格式, 默认YYYY_mm_dd_HH_MM_SS_withDash
        void debug(const std::string& _tittle, const std::string& _info, const LogTimeStyle _time_style = YYYY_md_HMS_withDash) {
            this->log(XLIB_LOG_LEVEL_DEBUG, _tittle, _info, _time_style);
        }
        /// @brief 记录日志的主要接口函数, 默认日志级别为INFO
        /// @param _tittle 日志标题
        /// @param _info 日志信息
        /// @param _time_style 时间格式, 默认YYYY_mm_dd_HH_MM_SS_withDash
        void info(const std::string& _tittle, const std::string& _info, const LogTimeStyle _time_style = YYYY_md_HMS_withDash) {
            this->log(XLIB_LOG_LEVEL_INFO, _tittle, _info, _time_style);
        }
        /// @brief 记录日志的主要接口函数, 默认日志级别为WARNING
        /// @param _tittle 日志标题
        /// @param _info 日志信息
        /// @param _time_style 时间格式, 默认YYYY_mm_dd_HH_MM_SS_withDash
        void warning(const std::string& _tittle, const std::string& _info, const LogTimeStyle _time_style = YYYY_md_HMS_withDash) {
            this->log(XLIB_LOG_LEVEL_WARNING, _tittle, _info, _time_style);
        }
        /// @brief 记录日志的主要接口函数, 默认日志级别为ERROR
        /// @param _tittle 日志标题
        /// @param _info 日志信息
        /// @param _time_style 时间格式, 默认YYYY_mm_dd_HH_MM_SS_withDash
        void error(const std::string& _tittle, const std::string& _info, const LogTimeStyle _time_style = YYYY_md_HMS_withDash) {
            this->log(XLIB_LOG_LEVEL_ERROR, _tittle, _info, _time_style);
        }
        /// @brief 记录日志的主要接口函数, 默认日志级别为FATAL
        /// @param _tittle 日志标题
        /// @param _info 日志信息
        /// @param _time_style 时间格式, 默认YYYY_mm_dd_HH_MM_SS_withDash
        void fatal(const std::string& _tittle, const std::string& _info, LogTimeStyle _time_style = YYYY_md_HMS_withDash) {
            this->log(XLIB_LOG_LEVEL_FATAL, _tittle, _info, _time_style);
        }
    };
}

#endif // LOGGER_IMPL_HPP