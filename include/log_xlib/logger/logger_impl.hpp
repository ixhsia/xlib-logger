#ifndef LOGGER_IMPL_HPP
#define LOGGER_IMPL_HPP
#include <string>
#include <fstream>
#include <chrono>
#include <iostream>
#include <format>

namespace xlib::logger {
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

    std::optional<std::fstream> _load_file(const std::string& _file_name,
        std::ios_base::openmode _mode = std::ios::in | std::ios::out | std::ios::app);

    class LogWriter {
        /// 0b00 - no show, 0b01 - show_cmd, 0b10 - show_file, 0b11 - both
        uint8_t show_flag_ = 0b00;
        LoggerEntity entity_ {};
        std::ofstream file_{};

        std::string log_time_style_strs[18] = {
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
        explicit LogWriter(const std::string& _file_log);
        LogWriter();
        ~LogWriter();

        void set_log_level(const LogLevel _level) {
            entity_.level = _level;
        }

        void set_timestamp(LogTimeStyle _style);
        void set_timestamp(const std::string& _timestamp) {
            entity_.timestamp = _timestamp;
        }
        void set_title(const std::string& _title) {
            entity_.title = _title;
        }
        void set_information(const std::string& _information) {
            entity_.information = _information;
        }
        void set_show_flag(const uint8_t _flag) {
            show_flag_ = _flag;
        }

        void write();
        LoggerEntity& get_entity() {return entity_;}
    };
}
#endif // LOGGER_IMPL_HPP