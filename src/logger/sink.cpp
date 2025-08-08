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

void xlib::logger::Sink_Command::set_config(SinkDataStructure *_configs) {
    return void();
}

void xlib::logger::Sink_Files::set_config(SinkDataStructure *_configs) {
    if (const auto config = dynamic_cast<SinkDataStructure_File*>(_configs)) {
        const std::string file_file = config->file_path + config->file_name;
        file_->open(file_file, std::ios::out | std::ios::app);
        if (!file_->is_open())
            std::cerr << "Failed to open file: " << file_file << std::endl;
    }
    else std::cerr << "Invalid configuration for Sink_Files." << std::endl;
}

void xlib::logger::Sink_Network::set_config(SinkDataStructure *_configs) {
    if (const auto config = dynamic_cast<SinkDataStructure_Network*>(_configs)) {
        //TODO: Implement network connection logic
        url_ = "https://" + config->host + ":" + std::to_string(config->port);
    }
    else std::cerr << "Invalid configuration for Sink_Network." << std::endl;
}
