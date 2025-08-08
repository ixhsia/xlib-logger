#ifndef SINK_HPP
#define SINK_HPP

#include <atomic>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <thread>
#include <expected>
#include <vector>
#include "interface.hpp"
#include <fstream>

//TODO: sink module for logger, need to finish it
namespace xlib::logger {
    class SinkManager {
        //TODO: class ThreadPool in new file

        // // 多线程配置
        // std::deque<std::string> thread_queue_{};
        // std::mutex thread_mutex_{};
        // std::condition_variable thread_cv_{};
        // std::atomic<bool> thread_is_run_ = false;
        // std::thread thread_forRunning_{};

        std::vector<ILogSink*> sink_list_{};
    public:
        static SinkManager& get_instance() {
            static SinkManager instance;
            return instance;
        }
        void registration(const std::tuple<uint8_t, std::any> &_sink_param);
        // void start_thread();
        void update(const LoggerEntity& _entity) const;
        // void stop_thread();
        void clean_sink_pool();
    };

    class Sink_Command final : public ILogSink {

    public:
        void set_config(SinkDataStructure* _configs) override;
        void update(const LoggerEntity &_entity) override{
            std::cout << _entity.format() << std::endl;
        }
    };

    class Sink_Files final : public ILogSink {
        std::ofstream* file_ = new std::ofstream();
    public:
        void set_config(SinkDataStructure* _configs) override;
        void update(const LoggerEntity &_entity) override;
    };

    class Sink_Network final : public ILogSink {
        std::string url_{};
    public:
        void set_config(SinkDataStructure* _configs) override;
        void update(const LoggerEntity &_entity) override;
    };

}

#endif // SINK_HPP