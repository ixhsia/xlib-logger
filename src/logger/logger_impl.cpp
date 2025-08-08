#include "logger/logger_impl.hpp"
#include <iostream>
#include <string>
#include <unordered_map>


void xlib::logger::LogWriter::init_for_base_config() {
    ///TODO: Configure

    std::string log_name{};
    //log rolling
    const auto size_tmp = parse_["logger"]["log_rolling_size"].value<size_t>();
    if (size_tmp.has_value()) {
        is_rolling_ = true;
        rolling_size_ = size_tmp.value();
        log_file_name_styled_raw_ = parse_["file"]["log_name"].value_or<std::string>("log_${time}");
        log_name = log_style_name_analysis(log_file_name_styled_raw_);
    }
    else {
        is_rolling_ = false;
        log_name = parse_["file"]["log_file"].value_or("log.log");
    };

    // get log_file
    log_file_path_ = parse_["file"]["log_dir"].value_or("./");
    const std::string file_log = log_file_path_ + log_name;
    // open log_file
    file_->open(file_log, std::ios::out | std::ios::app);
    if (!file_->is_open())
        std::cerr << "Failed to open file: " << file_log << std::endl;

    //config logger
    set_logger_level_line(static_cast<LogLevel>(parse_["logger"]["level"].value_or(1)));
    set_logger_thread_isEnabled(parse_["logger"]["thread"].value_or(true));
    set_show_flag(parse_["logger"]["show_flag"].value_or(0x03));
}

void xlib::logger::LogWriter::init_from_file_log(const std::string &_file_log) {
    show_flag_ |= xlib::Flags::ShowSinkFlag::Sink_All;
    file_->open(_file_log, std::ios::out | std::ios::app);
    if (!file_->is_open())
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
    } catch (...) {throw std::runtime_error("Illegal config file: " + _file_config);}
   this->init_for_base_config();
}

//TODO: DEL it
void xlib::logger::LogWriter::write_ipt_impl() {
    if (show_flag_ & 0b01) {
        std::cout << entity_.format() << std::endl;
    }
    if (show_flag_ & 0b10) {
        *file_ << entity_.format() << std::endl;;
        file_->flush();
    }
    entity_.clean();
}

//TODO: DEL it
void xlib::logger::LogWriter::thread_write_ipt_impl() {
    while (thread_is_run_ || !thread_queue_.empty()) {
        std::unique_lock<std::mutex> lock(thread_mutex_);
        thread_cv_.wait(lock, [this] {return !thread_queue_.empty() || !thread_is_run_;});
        while (!thread_queue_.empty()) {
            std::string tmp = std::move(thread_queue_.front());
            thread_queue_.pop_front();
            lock.unlock();
            if (show_flag_ & xlib::Flags::ShowSinkFlag::Sink_CommandLine)
                std::cout << tmp << std::endl;
            if (show_flag_ & xlib::Flags::ShowSinkFlag::Sink_File)
                if (file_->is_open())
                    *file_ << tmp << std::endl;
            lock.lock();
        }
    }
}

std::string xlib::logger::LogWriter::log_style_name_analysis(std::string _styled_name) const {
    const std::unordered_map<std::string, int> paramsMap = {
        {"time" , 0},
        {"level", 1},
        {"title", 2},
        {"counter", 3},
    };
    auto rule = [&](const std::string& param) {
        try {
            switch (paramsMap.at(param)) {
                case 0:     return set_timestamp_name();
                case 1:     return _get_log_level_str(entity_.level);
                case 2:     return entity_.title;
                case 3:     return std::to_string(log_rolling_counter_);
                default:    return param;
            }
        } catch (...) {return param;}
    };

    std::vector<std::string> final{};
    for (auto it = _styled_name.begin(); it != _styled_name.end(); ++it) {
        if (*it == '$') {
            ++it;
            if (*it == '{') {
                ++it;
                std::vector<char> tmp{};
                while (*it != '}') {
                    tmp.push_back(*it);
                    ++it;
                }
                auto param = rule(std::string(tmp.begin(), tmp.end()));
                final.push_back(param);
            }
        }
        else
            final.emplace_back(1, *it);
    }
    std::string tmp{};
    for (const auto& str : final)
        tmp += str;
    return tmp;
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
    if (file_ != nullptr && file_->is_open())
        file_->close();
    delete file_;
    file_ = nullptr;
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

std::string xlib::logger::LogWriter::set_timestamp_name() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now);
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    return oss.str();
}

void xlib::logger::LogWriter::close_and_open_next_rolling_logFile() const {
    const std::string log_name = log_style_name_analysis(log_file_name_styled_raw_);
    const std::string file_log = log_file_path_ + log_name;
    if (file_ != nullptr && file_->is_open()) {
        file_->close();
        file_->open(file_log, std::ios::out | std::ios::app);
        if (!file_->is_open())
            std::cerr << "Failed to open file: " << file_log << std::endl;
    }
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
