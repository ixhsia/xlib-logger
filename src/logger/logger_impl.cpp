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
    std::cout << _entity.format() << std::endl;
}

void xlib::logger::LogWriter::init_from_file_log(const std::string &_file_log) {
    show_flag_ |= xlib::Flags::ShowSinkFlag::Sink_All;
    file_.open(_file_log, std::ios::out | std::ios::app);
    if (!file_.is_open())
        std::cerr << "Failed to open file: " << _file_log << std::endl;
}

void xlib::logger::LogWriter::init_from_file_config(const std::string &_file_config) {
    show_flag_ |= xlib::Flags::ShowSinkFlag::Sink_All;
    parse_ = toml::parse_file(_file_config);
    try {
        if (parse_["version"].value_or(0) > VERSION || parse_["version"].value_or(0) < 1) {
            std::cerr << "Failed to parse config file: " << _file_config    << "\n"
                      << "Log will be written in path './error.log'..."     << std::endl;
            init_from_file_log("./error.log");
            return void();
        }
        std::cout << "OK"<< std::endl;
    } catch (...) {throw std::runtime_error("Illegal config file: " + _file_config);}
    const std::string file_log = parse_["log_file"].value_or("log.log");
    file_.open(file_log, std::ios::out | std::ios::app);
    if (!file_.is_open())
        std::cerr << "Failed to open file: " << file_log << std::endl;

    //TODO: Configure
    //config logger
    auto *logger = parse_["logger"].as_table();
    this->set_logger_level_line(static_cast<LogLevel>(logger->at("level").value_or(1)));
    this->set_logger_thread_isEnabled(logger->at("thread").value_or(true));
    this->set_show_flag(logger->at("show_flag").value_or(0x03));
}

void xlib::logger::LogWriter::write_ipt_impl() {
    if (show_flag_ & 0b01) {
        _command_print(entity_);
    }
    if (show_flag_ & 0b10) {
        file_ << entity_.format() << std::endl;;
        file_.flush();
    }
    entity_.clean();
}

void xlib::logger::LogWriter::thread_write_ipt_impl() {
    if (show_flag_ & xlib::Flags::ShowSinkFlag::Sink_CommandLine)
        _command_print(entity_);
    if (show_flag_ & xlib::Flags::ShowSinkFlag::Sink_File)
        while (thread_is_run_ || !thread_queue_.empty()) {
            std::unique_lock<std::mutex> lock(thread_mutex_);
            thread_cv_.wait(lock, [this] {return !thread_queue_.empty() || !thread_is_run_;});
            while (!thread_queue_.empty()) {
                std::string tmp = std::move(thread_queue_.front());
                thread_queue_.pop_front();
                lock.unlock();
                if (file_.is_open())
                    file_ << tmp << std::endl;
                lock.lock();
            }
        }
}

std::unique_ptr<xlib::logger::LogWriter> xlib::logger::LogWriter::from_file_config(const std::string &_file_config) {
    auto ptr = std::make_unique<LogWriter>();
    ptr->init_from_file_config(_file_config);
    ptr->set_thread_ifEnabled();
    return ptr;
}

std::unique_ptr<xlib::logger::LogWriter> xlib::logger::LogWriter::from_file_log(const std::string &_file_log) {
    auto ptr = std::make_unique<LogWriter>();
    ptr->init_from_file_log(_file_log);
    ptr->set_thread_ifEnabled();
    return ptr;
}

void xlib::logger::LogWriter::set_thread_ifEnabled() {
    if (is_thread_log_enabled_) {
        thread_is_run_ = true;
        thread_forWriting_ = std::thread(&LogWriter::thread_write_ipt_impl, this);
    }
}

xlib::logger::LogWriter::~LogWriter() {
    if (is_thread_log_enabled_) {
        thread_is_run_ = false;
        thread_cv_.notify_all();
        thread_forWriting_.join();
    }
    file_.close();
}

std::string xlib::logger::LogWriter::set_msg_timestamp(const LogTimeStyle _style) {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now);
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << '(' << std::put_time(&tm, log_time_style_strs_[static_cast<uint8_t>(_style)].c_str()) << ')';
    entity_.timestamp = oss.str();
    return oss.str();
}

void xlib::logger::LogWriter::log() {
    if (is_thread_log_enabled_) {
        {
            std::lock_guard<std::mutex> lock(thread_mutex_);
            thread_queue_.push_back(entity_.format());
        }
        thread_cv_.notify_one();
    }
    else
        write_ipt_impl();
}

void xlib::logger::LogWriter::log(const LogLevel _level,const std::string& _tittle, const std::string& _info, const LogTimeStyle _time_style) {
    entity_.clean();
    entity_ = {
        .level = _level,
        .timestamp = set_msg_timestamp(_time_style),
        .title = _tittle,
        .information = _info,
    };
    this->log();
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
