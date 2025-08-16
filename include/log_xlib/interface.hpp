#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include <any>

#include "loggerEntity.hpp"
#include "logger/sink_dataStructure.hpp"

/// 仅仅是想实现类似独热编码的效果，只是不愿意 '#define'
namespace xlib::Flags::ShowSinkFlag {
    constexpr uint8_t Show_None            = 0b0000'0000;
    constexpr uint8_t Sink_CommandLine     = 0b0000'0001;
    constexpr uint8_t Sink_File            = 0b0000'0010;
    constexpr uint8_t Sink_Network         = 0b0000'0100;
    constexpr uint8_t Sink_All             = Sink_CommandLine | Sink_File | Sink_Network;
}

namespace xlib::logger {
    class ILogSink {
    public:
        ILogSink() = default;
        virtual ~ILogSink() = default;

        virtual void set_config(SinkDataStructure* _configs) = 0;
        virtual void update(const LoggerEntity& _entity) = 0;
    };
}

#endif // INTERFACE_HPP