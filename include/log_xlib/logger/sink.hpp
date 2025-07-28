#ifndef SINK_HPP
#define SINK_HPP

#include <cstdint>
#include <iostream>
#include <ostream>

#include "interface.hpp"

//TODO: sink module for logger, need to finish it
namespace xlib::logger {
    class Sink : public ILogSink {
    protected:
        uint8_t show_flag_ = 0x00;
        static LoggerEntity entity_;

    public:
        Sink() = default;
        ~Sink() override = default;

        void write_in() override = 0;
        static void update_entity(const LoggerEntity &_entity) {
            entity_.clean();
            entity_ = _entity;
        }

        void read_in_entity(const LoggerEntity &_entity) override;
        void func_flags(const uint8_t _flag) override {
            show_flag_ = _flag;
        }
    };

    class Sink_Command final : private Sink {
    public:
        void write_in() override {
            std::cout << entity_.format() << std::endl;
        }
    };

    class Sink_Files final : private Sink {
    public:
        void write_in() override;
    };

}

#endif // SINK_HPP