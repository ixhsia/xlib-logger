#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "loggerEntity.hpp"

namespace xlib::logger {
    class ILogSink {
    public:
        ILogSink() = default;
        virtual ~ILogSink() = default;

        virtual void read_in_entity(const LoggerEntity& _entity) = 0;
        virtual void write_in() = 0;

        virtual void func_flags(uint8_t _flag) = 0;
    };

    //TODO: FINISH it
    class ILoggerSinkParam {
    };

}

#endif // INTERFACE_HPP