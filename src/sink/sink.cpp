#include "sink/sink.hpp"
#include <sstream>
#include "core/logger_impl.hpp"
#include "interface.hpp"
void xlib::logger::Sink_Files::set_config(SinkDataStructure *_configs) {
    if (const auto config = dynamic_cast<SinkDataStructure_File*>(_configs)) {
        config_ = *config;

        if (config_.is_rolling) {
            config_.file_name = log_style_name_analysis(config_.log_name,{
                .level = XLIB_LOG_LEVEL_INFO,
                .timestamp = get_timestamp_now(),
                .title = "Initialization",
                .information = "Rolling log file initialization."
            });
        }

        const std::string file_info = config->file_path + "logger_system_information.toml";
        const auto tmp_a = log_sys_info_load(file_info);
        if (tmp_a.has_value() && tmp_a.value()) {
            config_.file_name.clear();
            config_.file_name = log_sys_information_["latest_log_file"].value_or<std::string>(std::move(config->file_name));
            file_counter_ = log_sys_information_["file_counter"].value_or<size_t>(0);
        }

        const std::string file_file = config_.file_path + config_.file_name;

        std::ifstream file_onlyr(file_file, std::ios::binary | std::ios::ate);
        if (file_onlyr.is_open()) {
            log_rolling_counter_ = file_onlyr.tellg(); // 直接获取末尾位置即文件大小
            file_onlyr.close();
        }

        file_->open(file_file, std::ios::out | std::ios::app);
        if (!file_->is_open())
            throw std::runtime_error("Failed not open file " + file_file);
        log_sys_info_save(config_.file_name);
    }
    else
        std::cerr << "Invalid configuration for Sink_Files." << std::endl;
}

void xlib::logger::Sink_Files::update(const LoggerEntity &_entity) {
    if (is_rolling_next())
        this->rolling_next(_entity);
    if (file_->is_open()) {
        *file_ << _entity.format() << std::endl;;
        log_rolling_counter_ += _entity.format().size() * sizeof(char);
    }
}

std::string xlib::logger::Sink_Files::get_timestamp_now() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now);
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    return oss.str();
}

std::optional<bool> xlib::logger::Sink_Files::log_sys_info_load(const std::string &_file_path) {
    bool rst = true;
    if (!std::filesystem::exists(_file_path)) {
        std::ofstream out(_file_path);
        if (!out.is_open())
            throw std::runtime_error("Failed to create file " + _file_path);
        out << "version = " << VERSION << "\n\n";
        out.close();
        rst = false;
    }

    try {
        log_sys_information_ = toml::parse_file(_file_path);
    } catch (const toml::parse_error& err) {
        std::cerr << "TOML parse error in " << _file_path << ": " << err.description() << "\n" << std::endl;
        return std::nullopt;
    }

    if (log_sys_information_["version"].value_or(0) > VERSION || log_sys_information_["version"].value_or(0) < 1) {
        std::cerr << "Failed to parse config file: " << _file_path    << "\n" << std::endl;
        return std::nullopt;
    }
    return rst;
}

void xlib::logger::Sink_Files::log_sys_info_save(const std::string& _saved_name) {
    log_sys_information_.insert_or_assign("latest_log_file", _saved_name);
    log_sys_information_.insert_or_assign("file_counter", static_cast<uint32_t>(file_counter_));
    std::ostringstream oss;
    oss << config_.file_path << "logger_system_information.toml";
    std::ofstream out(oss.str(), std::ios::out);
    if (out.is_open()) {
        out << "version = " << VERSION << "\n\n";
        log_sys_information_.erase("version");
        out << log_sys_information_;
        out.close();
    }
}

bool xlib::logger::Sink_Files::is_rolling_next() const {
    if (!this->config_.is_rolling)
        return false;
    return this->log_rolling_counter_ > this->config_.log_rolling_size;
}

void xlib::logger::Sink_Files::rolling_next(const LoggerEntity &_entity) {
    file_counter_ += 1;
    const std::string log_name = log_style_name_analysis(config_.log_name, _entity);
    const std::string file_file = config_.file_path + log_name;
    if (file_->is_open()) {
        file_->close();
        file_->open(file_file, std::ios::out | std::ios::app);
        log_rolling_counter_ = 0;
        if (!file_->is_open())
            std::cerr << "Failed to open file: " << file_file << std::endl;
    }
    config_.file_name = log_name;
    log_sys_info_save(log_name);
}

std::string xlib::logger::Sink_Files::log_style_name_analysis(const std::string &_styled_name, const LoggerEntity& _entity) const {
    const std::unordered_map<std::string, int> paramsMap = {
        {"time" , 0},
        {"level", 1},
        {"title", 2},
        {"counter", 3},
    };
    auto rule = [&](const std::string& param) {
        try {
            switch (paramsMap.at(param)) {
                case 0:     return get_timestamp_now();
                case 1:     return _get_log_level_str(_entity.level);
                case 2:     return _entity.title;
                case 3:     return std::to_string(file_counter_);
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

void xlib::logger::Sink_Network::set_config(SinkDataStructure *_configs) {
    if (const auto config = dynamic_cast<SinkDataStructure_Network*>(_configs)) {
        url_ = "https://" + config->host + ":" + std::to_string(config->port);
    }
    else std::cerr << "Invalid configuration for Sink_Network." << std::endl;
}
