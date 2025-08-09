#include "logger/sink.hpp"
#include "logger/logger_impl.hpp"

void xlib::logger::SinkManager::registration(const std::tuple<uint8_t, std::any> &_sink_param) {
    const auto flag = std::get<0>(_sink_param);
    if (flag == xlib::Flags::ShowSinkFlag::Show_None)
        return void();
    if (flag & xlib::Flags::ShowSinkFlag::Sink_CommandLine) {
        auto config_commandLine = std::any_cast<SinkDataStructure_CommandLine>(std::get<1>(_sink_param));
        this->sink_list_.emplace_back(new Sink_Command());
        this->sink_list_.back()->set_config(&config_commandLine);
    }
    if (flag & xlib::Flags::ShowSinkFlag::Sink_File) {
        auto config_file = std::any_cast<SinkDataStructure_File>(std::get<1>(_sink_param));
        this->sink_list_.emplace_back(new Sink_Files());
        this->sink_list_.back()->set_config(&config_file);
    }
    if (flag & xlib::Flags::ShowSinkFlag::Sink_Network) {
        auto config_network = std::any_cast<SinkDataStructure_Network>(std::get<1>(_sink_param));
        this->sink_list_.emplace_back(new Sink_Network());
        this->sink_list_.back()->set_config(&config_network);
    }
}

// void xlib::logger::SinkManager::start_thread() {
//     this->thread_forRunning_ = std::thread(&SinkManager::update, this);
// }

void xlib::logger::SinkManager::update(const LoggerEntity& _entity) const {
    for (const auto it : this->sink_list_)
        it->update(_entity);
}

void xlib::logger::SinkManager::clean_sink_pool() {
    for (const auto it : this->sink_list_)
        delete it;
    this->sink_list_.clear();
}

void xlib::logger::Sink_Files::set_config(SinkDataStructure *_configs) {
    if (const auto config = dynamic_cast<SinkDataStructure_File*>(_configs)) {
        const std::string file_file = config->file_path + config->file_name;
        file_->open(file_file, std::ios::out | std::ios::app);
        if (!file_->is_open())
            throw std::runtime_error("Failed not open file " + file_file);

        if (config->is_rolling) {
            this->is_rolling_ = true;
            this->log_rolling_size_ = config->log_rolling_size;
            this->log_name_rolling_ = config->log_name;
            this->log_rolling_counter_ = 0;
        }
    }
    else
        std::cerr << "Invalid configuration for Sink_Files." << std::endl;
}

void xlib::logger::Sink_Files::update(const LoggerEntity &_entity) {
    if (is_rolling_) {
        //TODO: finish it
    }
    if (file_->is_open())
        *file_ << _entity.format() << std::endl;;
}

std::string xlib::logger::Sink_Files::get_timestamp_now() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now);
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    return oss.str();
}

std::string xlib::logger::Sink_Files::log_style_name_analysis(const std::string &_styled_name, LoggerEntity _entity) const {
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

void xlib::logger::Sink_Network::set_config(SinkDataStructure *_configs) {
    if (const auto config = dynamic_cast<SinkDataStructure_Network*>(_configs)) {
        //TODO: Implement network connection logic
        url_ = "https://" + config->host + ":" + std::to_string(config->port);
    }
    else std::cerr << "Invalid configuration for Sink_Network." << std::endl;
}
