#include "logger/logger_impl.hpp"

#include <utility>

std::string xlib::logger::_get_log_level_str(const uint8_t _level) {
    switch (_level) {
        case 0:  return "[DEBUG]";
        case 1:  return "[INFO]";
        case 2:  return "[WARNING]";
        case 3:  return "[ERROR]";
        case 4:  return "[FATAL]";
        default: return "[UNKNOWN]";
    }
}

void xlib::logger::_command_print(const LoggerEntity &_entity) {
    const std::string level_str = _get_log_level_str(static_cast<uint8_t>(_entity.level));
    std::cout << _entity.format() << std::endl;
}

std::optional<std::fstream> xlib::logger::_load_file(const std::string &_file_name, const std::ios_base::openmode _mode) {
    std::fstream file(_file_name, _mode);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << _file_name << std::endl;
        return std::nullopt;
    }
    return file;
}

void xlib::logger::LogWriter::write() {
    if (show_flag_ & 0b01) {
        _command_print(entity_);
    }
    if (show_flag_ & 0b10) {
        file_ << entity_.format() << std::endl;;
        file_.flush();
    }
    entity_.clean();
}

xlib::logger::LogWriter::LogWriter(const std::string &_file_log) {
    show_flag_ |= 0b11;
    file_.open(_file_log, std::ios::out | std::ios::app);
    if (!file_.is_open())
        std::cerr << "Failed to open file: " << _file_log << std::endl;
}

xlib::logger::LogWriter::LogWriter() {
    show_flag_ |= 0b01;
}

xlib::logger::LogWriter::~LogWriter() {
    file_.close();
}

std::string xlib::logger::LogWriter::set_timestamp(const LogTimeStyle _style) {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now);
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << '(' << std::put_time(&tm, log_time_style_strs[static_cast<uint8_t>(_style)].c_str()) << ')';
    entity_.timestamp = oss.str();
    return oss.str();
}

void xlib::logger::LogWriter::log(const LogLevel _level,const std::string& _tittle, const std::string& _info, const LogTimeStyle _time_style) {
    entity_.clean();
    entity_ = {
        .level = _level,
        .title = _tittle,
        .information = _info,
        .timestamp = set_timestamp(_time_style),
    };
    write();
}

void xlib::logger::LogWriter::debug(const std::string &_tittle, const std::string &_info, LogTimeStyle _time_style) {
    this->log(XLIB_LOG_LEVEL_DEBUG, _tittle, _info, _time_style);
}

void xlib::logger::LogWriter::info(const std::string &_tittle, const std::string &_info, const LogTimeStyle _time_style) {
    this->log(XLIB_LOG_LEVEL_INFO, _tittle, _info, _time_style);
}

void xlib::logger::LogWriter::warning(const std::string &_tittle, const std::string &_info, const LogTimeStyle _time_style) {
    this->log(XLIB_LOG_LEVEL_WARNING, _tittle, _info, _time_style);
}

void xlib::logger::LogWriter::error(const std::string &_tittle, const std::string &_info, const LogTimeStyle _time_style) {
    this->log(XLIB_LOG_LEVEL_ERROR, _tittle, _info, _time_style);
}

void xlib::logger::LogWriter::fatal(const std::string &_tittle, const std::string &_info, const LogTimeStyle _time_style) {
    this->log(XLIB_LOG_LEVEL_FATAL, _tittle, _info, _time_style);
}
