#include "logger.hpp"

int main(){
    auto writer = xlib::logger::LogWriter();
    // writer.load_config("config.toml");
    writer.log(xlib::logger::XLIB_LOG_LEVEL_DEBUG, "Test Logger", "This is a test log message.", xlib::logger::yy_md_HMS_withDash);
    writer.log(xlib::logger::XLIB_LOG_LEVEL_WARNING, "Test Logger 2", "This is a test log messageeee.", xlib::logger::yy_md_HMS_withDash);
}