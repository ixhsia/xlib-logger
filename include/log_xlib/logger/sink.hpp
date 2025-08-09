#ifndef SINK_HPP
#define SINK_HPP

#include <cstdint>
#include <iostream>
#include <mutex>
#include <vector>
#include <fstream>
#include "interface.hpp"
#include "utilities/thread_pool.hpp"

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
        void set_config(SinkDataStructure *_configs) override {return void();}
        void update(const LoggerEntity &_entity) override{
            std::cout << _entity.format() << std::endl;
        }
    };

    class Sink_Files final : public ILogSink {
        std::ofstream* file_ = new std::ofstream();

        bool is_rolling_ = false;
        uint32_t log_rolling_size_ = 0;
        std::string log_name_rolling_ = "log_${time}";
        size_t log_rolling_counter_{};
    public:
        void set_config(SinkDataStructure* _configs) override;
        void update(const LoggerEntity &_entity) override;

        static std::string get_timestamp_now();

    private:

        [[nodiscard]] std::string log_style_name_analysis(const std::string& _styled_name, LoggerEntity _entity) const;
    };

    /// DISABLED
    class Sink_Network final : public ILogSink {
        std::string url_{};
    public:
        void set_config(SinkDataStructure* _configs) override;
        void update(const LoggerEntity &_entity) override{return void();}
    };

}

#endif // SINK_HPP