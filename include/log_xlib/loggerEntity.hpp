#ifndef LOGGER_ENTITY_HPP
#define LOGGER_ENTITY_HPP

namespace xlib::logger {
    enum LogLevel {
        XLIB_LOG_LEVEL_DEBUG = 0,
        XLIB_LOG_LEVEL_INFO,
        XLIB_LOG_LEVEL_WARNING,
        XLIB_LOG_LEVEL_ERROR,
        XLIB_LOG_LEVEL_FATAL,
    };

    inline std::string xlib::logger::_get_log_level_str(const uint8_t _level) {
        switch (_level) {
            case 0:  return "[DEBUG]";
            case 1:  return "[INFO]";
            case 2:  return "[WARNING]";
            case 3:  return "[ERROR]";
            case 4:  return "[FATAL]";
            default: return "[UNKNOWN]";
        }
    }

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
}

#endif // LOGGER_ENTITY_HPP