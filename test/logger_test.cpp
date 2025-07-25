#include "logger.hpp"

int main(){
    xlib::logger::LogWriter writer("log_test.log");
    writer.set_show_flag(3);
    writer.set_title("Test Logger");
    writer.set_log_level(xlib::logger::LogLevel::XLIB_LOG_LEVEL_DEBUG);
    writer.set_information("This is a test log message.");
    writer.set_timestamp(xlib::logger::yy_md_HMS_withDash);
    writer.write();
    writer.set_title("Test Logger");
    writer.set_log_level(xlib::logger::LogLevel::XLIB_LOG_LEVEL_DEBUG);
    writer.set_information("This is a test log message.");
    writer.set_timestamp(xlib::logger::yy_md_HMS_withDash);
    writer.write();
}