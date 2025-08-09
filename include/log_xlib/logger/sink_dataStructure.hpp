/// @brief 日志写入器数据结构头文件
/// @author Harlan Xia
///
#ifndef SINK_DATA_STRUCTURE_HPP
#define SINK_DATA_STRUCTURE_HPP
#include <unordered_map>

namespace xlib::logger {
    struct SinkDataStructure {
        virtual ~SinkDataStructure() = default;
    };
    struct SinkDataStructure_CommandLine: SinkDataStructure {
        ~SinkDataStructure_CommandLine() override = default;
    };
    struct SinkDataStructure_File: SinkDataStructure {
        ~SinkDataStructure_File() override = default;

        std::string file_path;
        std::string file_name;

        //rolling config
        bool is_rolling = false;
        uint32_t log_rolling_size = 0;
        std::string log_name = "log_${time}";
    };
    struct SinkDataStructure_Network: SinkDataStructure {
        ~SinkDataStructure_Network() override = default;

        std::string host;
        uint16_t port;
    };


}

#endif // SINK_DATA_STRUCTURE_HPP