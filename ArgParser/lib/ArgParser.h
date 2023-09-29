#pragma once

#include <cinttypes>
#include <string>
#include <unordered_map>
#include <vector>

namespace ArgumentParser {
    class ArgParser;

    class Argument {
    public:
        std::string cut_name;
        std::string  name;
        bool is_store_value = false;
        bool is_help = false;
        std::string description;

        bool default_value = false;
        bool declaration;
        bool is_string = false;
        std::string* string_value = nullptr;

        bool is_flag = false;
        bool* boolean = nullptr;

        bool is_int = false;
        int32_t* int_value = nullptr;

        bool is_multi_value = false;
        size_t min_cnt_args;
        std::vector<int32_t>*  int_values = nullptr;
        std::vector<std::string>* string_values = nullptr;

        ArgParser* parser = nullptr;

        Argument* Default(const char* value);

        Argument* Default(bool logic_value);

        Argument& MultiValue(size_t min_args_count = 0);

        void StoreValue(std::string& value);

        void StoreValue(bool& logic_value);

        void StoreValues(std::vector<int32_t>& values);

        void StoreValues(std::vector<std::string>& values);

        std::string Info() const ;

        std::string InfoHelp() const;

        Argument& Positional();

        void RemoveArg();

    };

    class ArgParser {
    public:

        Argument* position;
        Argument* help;

        explicit ArgParser(const std::string& name)
                : name_(name)
                , position(nullptr)
                , help(nullptr)
        {}

        ~ArgParser() {
            for (const std::string& name: names_of_arguments_) {
                Argument* arg = arguments_[name];

                if (arg != nullptr && !arg->is_store_value) {
                    delete arg->boolean;
                    arg->boolean = nullptr;

                    delete arg->int_value;
                    arg->int_value = nullptr;

                    delete arg->int_values;
                    arg->int_values = nullptr;

                    delete arg->string_value;
                    arg->string_value = nullptr;

                    delete arg->string_values;
                    arg->string_values = nullptr;

                    arg->is_store_value = true;
                }

                delete arg;
            }
        }

        bool Parse(int argc, char** argv);

        bool Parse(const std::vector<std::string>& arguments);

        Argument& AddFlag(const std::string& long_name, const std::string& description = "");

        Argument& AddFlag(char short_name,
                          const std::string& long_name,
                          const std::string& description = "");

        Argument& AddHelp(char short_name,
                          const std::string& long_name,
                          const std::string& description = "");

        Argument& AddIntArgument(const std::string& name,
                                 const std::string& description = "");

        Argument& AddIntArgument(char short_name,
                                 const std::string& name,
                                 const std::string& description = "");

        Argument& AddStringArgument(const std::string& name,
                                    const std::string& description = "");

        Argument& AddStringArgument(char short_name,
                                    const std::string& name,
                                    const std::string& description = "");

        bool GetFlag(const std::string& long_name);

        bool GetFlag(char short_name);

        int GetIntValue(const std::string& param);

        int GetIntValue(char short_name);

        int GetIntValue(const std::string& name, const size_t& index);

        const std::string& GetStringValue(const std::string& name);

        const std::string& GetStringValue(char short_name);

        bool Help() const;

        void AddValue(Argument* arg, const std::string& value);

        std::string HelpDescription();

        bool Validation();

    private:
        std::string name_;
        std::vector<std::string> names_of_arguments_;
        std::unordered_map<std::string, Argument*> arguments_;
    };

} // namespace ArgumentParser
