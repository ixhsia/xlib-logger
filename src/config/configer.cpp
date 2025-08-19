#include "configuration/configer.hpp"
#include <iostream>

void xlib::configuration::Configure_Toml::parser_node(const toml::node &_node, const std::string &_parent,
    std::unordered_map<std::string, species> &_map) {

}

void xlib::configuration::Configure_Toml::load(const std::string &_file) {
    const toml::table table = toml::parse_file(_file);
    parser_node(table, "Toml", config_settings_value_);
}
