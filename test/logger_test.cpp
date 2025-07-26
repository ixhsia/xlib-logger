#include "logger.hpp"

int main(){
    auto writer = xlib::logger::LogWriter::from_file_config("config.toml");
    writer->set_show_flag(3);
    writer->set_msg_title("Test Logger");
    writer->set_msg_level(xlib::logger::LogLevel::XLIB_LOG_LEVEL_DEBUG);
    writer->set_information("This is a test log message.");
    writer->set_msg_timestamp(xlib::logger::yy_md_HMS_withDash);
    writer->log();
    writer->set_msg_title("Test Log");
    writer->set_msg_level(xlib::logger::LogLevel::XLIB_LOG_LEVEL_DEBUG);
    writer->set_information("This is a test log mese.");
    writer->set_msg_timestamp(xlib::logger::yy_md_HMS_withDash);
    writer->log();
}