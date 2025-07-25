#include "logger/logger_impl.hpp"

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
    std::cout
    <<  level_str              << " "
    <<  _entity.timestamp      << " "
    <<  _entity.title          << " > "
    <<  _entity.information    << std::endl;
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

    }
    entity_.clean();
}

void xlib::logger::LogWriter::set_timestamp(const LogTimeStyle _style) {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now);
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << '(' << std::put_time(&tm, log_time_style_strs[static_cast<uint8_t>(_style)].c_str()) << ')';
    entity_.timestamp = oss.str();
}