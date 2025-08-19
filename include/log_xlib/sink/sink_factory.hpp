#ifndef SINK_FACTORY_HPP
#define SINK_FACTORY_HPP
#include <any>

#include "sink.hpp"
#include "core/sink_manager.hpp"
#include "sink_dataStructure.hpp"

namespace xlib::logger {
    /// @brief SinkFactory类用于创建和管理日志接收器（Sink）的实例。
    class SinkFactory {
    public:
        static void create_sinks(const uint8_t _flag,const std::any& _sink_param,SinkManager& _manager) {
            if (_flag == xlib::Flags::ShowSinkFlag::Show_None)
                return void();
            if (_flag & xlib::Flags::ShowSinkFlag::Sink_CommandLine) {
                auto config_commandLine = std::any_cast<SinkDataStructure_CommandLine>(_sink_param);
                std::unique_ptr<ILogSink> sink = std::make_unique<Sink_Command>();
                sink->set_config(&config_commandLine);
;               _manager.registration(std::move(sink));
            }
            if (_flag & xlib::Flags::ShowSinkFlag::Sink_File) {
                auto config_file = std::any_cast<SinkDataStructure_File>(_sink_param);
                std::unique_ptr<ILogSink> sink = std::make_unique<Sink_Files>();
                sink->set_config(&config_file);
                _manager.registration(std::move(sink));
            }
            if (_flag & xlib::Flags::ShowSinkFlag::Sink_Network) {
                auto config_network = std::any_cast<SinkDataStructure_Network>(_sink_param);
                std::unique_ptr<ILogSink> sink = std::make_unique<Sink_Network>();
                sink->set_config(&config_network);
                _manager.registration(std::move(sink));
            }
        }
    };

}
#endif // SINK_FACTORY_HPP