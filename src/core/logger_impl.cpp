#include "core/logger_impl.hpp"
#include <iostream>
#include <string>
#include "sink/sink_dataStructure.hpp"
#include "definations.hpp"

// void xlib::logger::LogWriter::load_config(const std::string &_file_config) {
//     this->decoder_config_file(_file_config);
// }

// std::string xlib::logger::LogWriter::set_msg_timestamp(const LogTimeStyle _style) {

// }

//void xlib::logger::LogWriter::decoder_config_file(const std::string &_file_config) {
//    try {
//        parse_ = toml::parse_file(_file_config);
//        if (parse_["version"].value_or(0) > VERSION || parse_["version"].value_or(0) < 1) {
//            std::cerr << "Failed to parse config file: " << _file_config    << "\n"
//                      << "Log will be written in JUST CommandLine..."     << std::endl;
//
//            if_found_config_file_failed();
//            return void();
//        }
//    } catch (...) {throw std::runtime_error("Illegal config file: " + _file_config);}
//
//    if (parse_["logger"].as_table() != nullptr) {
//        SinkManagerInit sink_manager_init{};
//        toml::table* logger = parse_["logger"].as_table();
//        sink_manager_init.level_filter = static_cast<LogLevel>(logger->at("level").value_or(1));
//        sink_manager_init.is_thread_enabled = logger->at("thread").value_or(true);
//        sink_manager_.init_manager(sink_manager_init);
//    }
//    else {
//        std::cerr << "Failed to parse config file: " << _file_config << "\n"
//                  << "Log will be written in JUST CommandLine..."    << std::endl;
//        if_found_config_file_failed();
//        return void();
//    }
//
//    uint8_t show_flag = xlib::Flags::ShowSinkFlag::Show_None;
//
//    if (parse_["cmd"].as_table()) {
//        show_flag = xlib::Flags::ShowSinkFlag::Sink_CommandLine;
//        SinkDataStructure_CommandLine data{};
//        sink_manager_.registration(std::make_tuple(show_flag, data));
//    }
//    if (parse_["file"].as_table()) {
//        show_flag = xlib::Flags::ShowSinkFlag::Sink_File;
//        SinkDataStructure_File data{};
//        toml::table* file = parse_["file"].as_table();
//        data.file_name = file->at("log_file").value_or("log.log");
//        data.file_path = file->at("log_dir").value_or("./");
//        if (parse_["file"]["rolling"].as_table()) {
//            toml::table* rolling = parse_["file"]["rolling"].as_table();
//            data.is_rolling = parse_["file"]["rolling"]["is_rolling"].value_or(true);
//            data.log_rolling_size = rolling->at("log_rolling_size").value_or(0);
//            data.log_name = rolling->at("log_name").value_or("log_${time}");
//        }
//        else data.is_rolling = false;
//        sink_manager_.registration(std::make_tuple(show_flag, data));
//    }
//    if (parse_["net"].as_table()) {
//        show_flag = xlib::Flags::ShowSinkFlag::Sink_Network;
//        SinkDataStructure_Network data{};
//        toml::table* net = parse_["net"].as_table();
//        data.host = net->at("host").value_or("127.0.0.1");
//        data.port = net->at("port").value_or(8080);
//        sink_manager_.registration(std::make_tuple(show_flag, data));
//    }
//}

// void xlib::logger::LogWriter::if_found_config_file_failed() {
//     SinkManagerInit sink_manager_init{
//         .is_thread_enabled = true,
//         .level_filter = XLIB_LOG_LEVEL_INFO
//     };
//     sink_manager_.init_manager(sink_manager_init);
//     SinkFactory::create_sinks(xlib::Flags::ShowSinkFlag::Show_None, SinkDataStructure(), sink_manager_);
//     std::cerr << "Configure by file failed, logger will be silently..." << std::endl;
// }
