#ifndef CONFIGER_HPP
#define CONFIGER_HPP
#include <unordered_map>
#include "toml.hpp"

//TODO: finish xlib::configuration

namespace xlib::configuration {
    class Configure {
    protected:
        using species = std::variant<std::string, float, int, bool>;

    public:
        virtual ~Configure() = default;
        virtual void load(const std::string& _file) = 0;
        virtual void save() = 0;
        [[nodiscard]] virtual std::optional<species> get(const std::string& _param) = 0;
    };

    class Configure_Toml final : public Configure {
        std::unordered_map<std::string, species> config_settings_value_;

        void parser_node(const toml::node& _node, const std::string& _parent,
                    std::unordered_map<std::string, species>& _map);
    public:
        void load(const std::string &_file) override;

        void save() override;

        [[nodiscard]] std::optional<species> get(const std::string &_param) override;
    };

}
#endif // CONFIGER_HPP
