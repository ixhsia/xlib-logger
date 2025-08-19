#ifndef SINK_MANAGER_HPP
#define SINK_MANAGER_HPP
#include "definations.hpp"
#include <iostream>
#include <vector>
#include "core/loggerEntity.hpp"

namespace xlib::logger {
    struct SinkDataStructure {
        virtual ~SinkDataStructure() = default;
    };
    struct SinkDataStructure_CommandLine final: SinkDataStructure {
        ~SinkDataStructure_CommandLine() override = default;
    };

    class ILogSink {
    public:
        ILogSink() = default;
        virtual ~ILogSink() = default;

        virtual void set_config(SinkDataStructure* _configs) = 0;
        virtual void update(const LoggerEntity& _entity) = 0;
    };

    class Sink_Command final : public ILogSink {
    public:
        void set_config(SinkDataStructure *_configs) override {return void();}
        void update(const LoggerEntity &_entity) override{
            std::cout << _entity.format() << std::endl;
        }
    };

    struct SinkManagerInit {
        bool is_thread_enabled = true;
        LogLevel level_filter = XLIB_LOG_LEVEL_DEBUG;
    };

    class SinkManager {
        LogLevel level_filter_ = XLIB_LOG_LEVEL_DEBUG;
        bool is_thread_enabled_ = false;
        std::vector<std::unique_ptr<ILogSink>> sink_list_{};

    public:
        static SinkManager& get_instance() {
            static SinkManager instance;
            return instance;
        }
        void init_manager(const SinkManagerInit &_init) {
            is_thread_enabled_ = _init.is_thread_enabled;
            level_filter_ = _init.level_filter;
        }
        // void registration(const std::tuple<uint8_t, std::any> &_sink_param);
        void registration(std::unique_ptr<ILogSink> _sink) {
            this->sink_list_.push_back(std::move(_sink));
        }

        void update(const LoggerEntity& _entity) const {
            for (const auto& it : this->sink_list_)
                it->update(_entity);
        }

        void clean_sink_pool() {
            this->sink_list_.clear();
        }
    };

}

#endif //SINK_MANAGER_HPP