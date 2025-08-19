#ifndef SINK_HPP
#define SINK_HPP

#include <iostream>
#include <fstream>
#include <toml.hpp>
#include "sink/sink_dataStructure.hpp"
#include "interface.hpp"
#include "core/loggerEntity.hpp"
namespace xlib::logger {
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