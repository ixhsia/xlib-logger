#ifndef SINK_HPP
#define SINK_HPP

//TODO: sink module for logger, need to finish it
namespace xlib::logger {
    struct LoggerEntity;

    class Sink_Base {
    public:
        Sink_Base() = default;
        virtual ~Sink_Base() = default;
        virtual void write() = 0;
    };

    class Sink_Console final : public Sink_Base {
    public:
        void write() override;
    };
    class Sink_File final : public Sink_Base {
    public:
        void write() override;
    };

}

#endif // SINK_HPP