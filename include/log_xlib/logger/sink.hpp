#ifndef SINK_HPP
#define SINK_HPP

#include <cstdint>
#include <iostream>
#include <mutex>
#include <vector>
#include <fstream>
#include <toml.hpp>

#include "interface.hpp"
#include "utilities/thread_pool.hpp"
namespace xlib::logger {
    struct SinkManagerInit {;
        bool is_thread_enabled = true;
        LogLevel level_filter = XLIB_LOG_LEVEL_DEBUG;
    };

    class SinkManager {
        //TODO: class ThreadPool in new file

        LogLevel level_filter_ = XLIB_LOG_LEVEL_DEBUG;
        bool is_thread_enabled_ = true;
        std::vector<ILogSink*> sink_list_{};

    public:
        static SinkManager& get_instance() {
            static SinkManager instance;
            return instance;
        }
        void init_manager(SinkManagerInit &_init);
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
        toml::table log_sys_information_{};
        SinkDataStructure_File config_{};

        size_t log_rolling_counter_ = 0;
        size_t file_counter_ = 0;
    public:
        void set_config(SinkDataStructure* _configs) override;
        void update(const LoggerEntity &_entity) override;

        static std::string get_timestamp_now();

        std::optional<bool> log_sys_info_load(const std::string &_file_path);
        void log_sys_info_save(const std::string& _saved_name);
    private:
        [[nodiscard]] bool is_rolling_next() const;
        void rolling_next(const LoggerEntity& _entity);
        [[nodiscard]] std::string log_style_name_analysis(const std::string &_styled_name, const LoggerEntity &_entity) const;
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