#include "ArgParser.h"

#include <iostream>

using namespace ArgumentParser;

bool ArgParser::GetFlag(const std::string& long_name) {
    return *arguments_[long_name]->boolean;
}

bool ArgParser::GetFlag(char short_name) {
    return *arguments_[std::string(sizeof(short_name), short_name)]->boolean;
}

int ArgParser::GetIntValue(const std::string& name) {
    return *arguments_[name]->int_value;
}

int ArgParser::GetIntValue(char short_name) {
    return *arguments_[std::string(sizeof(short_name), short_name)]->int_value;
}

int ArgParser::GetIntValue(const std::string& name, const size_t& index) {
    return (*arguments_[name]->int_values)[index];
}

const std::string& ArgParser::GetStringValue(const std::string& name){
    return *arguments_[name]->string_value;
}

const std::string& ArgParser::GetStringValue(char short_name) {
    return *arguments_[std::string(sizeof(short_name), short_name)]->string_value;
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> arguments;

    for (size_t i = 1; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }

    return Parse(arguments);
}

bool ArgParser::Parse(const std::vector<std::string>& arguments) {
    for (const std::string& arg : arguments) {
        if (arg[0] == '-') {
            std::string arg_name;

            if (arg[1] == '-') {
                arg_name = arg.substr(2, arg.find('=') - 2);

                if (!arguments_.count(arg_name)) {
                    return false;
                }
            } else {
                size_t pos = 1;
                arg_name = arg[pos];

                if (!arguments_.count(arg_name)) {
                    return false;
                }

                while (pos < arg.size() && arguments_[arg_name]->is_flag) {
                    arg_name = arg[pos];
                    *arguments_[arg_name]->boolean = true;
                    arguments_[arg_name]->declaration = true;
                    pos++;
                }
            }

            Argument* this_arg = arguments_[arg_name];

            if (arg.find('=') != std::string::npos) {
                std::string value = arg.substr(arg.find('=') + 1);
                AddValue(this_arg, value);
            }

            if (this_arg->is_flag) {
                *this_arg->boolean = true;
                this_arg->declaration = true;
            }

            if (this_arg->is_help) {
                return true;
            }
        } else if (arg != "app") {
            AddValue(position, arg);
        }
    }

    return Validation();
}

void ArgParser::AddValue(Argument* arg, const std::string& value) {
    if (arg->is_multi_value) {
        if (arg->is_int) {
            (*arg->int_values).push_back(stoi(value));
            size_t cnt_of_args_now = (*arg->int_values).size();

            if (arg->min_cnt_args <= cnt_of_args_now) {
                arg->declaration = true;
            }
        }

        if (arg->is_string) {
            (*arg->string_values).push_back(value);
            size_t cnt_of_args_now = (*arg->string_values).size();

            if (arg->min_cnt_args <= cnt_of_args_now) {
                arg->declaration = true;
            }
        }
    } else {
        if (arg->is_int) {
            *arg->int_value = stoi(value);
        }

        if (arg->is_string) {
            *arg->string_value = value;
        }

        arg->declaration = true;
    }
}

bool ArgParser::Validation() {
    bool all_declaration = true;

    for (const auto& param : arguments_) {
        if (!param.second->declaration) {
            all_declaration = false;
        }
    }

    return all_declaration;
}

Argument& ArgParser::AddFlag(const std::string& long_name,
                             const std::string& description) {
    Argument* arg = new Argument;
    arg->is_flag = true;
    arg->parser = this;
    arg->name = long_name;
    arg->description = description;
    arg->boolean = new bool(false);
    arguments_[long_name] = arg;
    names_of_arguments_.push_back(long_name);

    return *arg;
}

Argument& ArgParser::AddFlag(char short_name,
                             const std::string& long_name,
                             const std::string& description) {
    Argument* arg = new Argument;
    arg->parser = this;
    arg->is_flag = true;
    arg->cut_name = short_name;
    arg->name = long_name;
    arg->description = description;
    arg->boolean = new bool(false);
    arguments_[long_name] = arg;
    arguments_[std::string(1,short_name)] = arg;
    names_of_arguments_.emplace_back(long_name);

    return *arg;
}

Argument& ArgParser::AddHelp(char short_name,
                             const std::string& long_name,
                             const std::string& description) {
    Argument* arg = new Argument;
    arg->is_help = true;
    arg->description = description;
    arg->declaration = true;
    arg->parser = this;
    arg->cut_name = short_name;
    arg->name = long_name;
    arguments_[std::string(sizeof(short_name),short_name)] = arg;
    arguments_[long_name] = arg;
    names_of_arguments_.push_back(long_name);
    help = arg;

    return *arg;
}

Argument& ArgParser::AddIntArgument(const std::string& name,
                                    const std::string& description) {
    Argument* arg = new Argument;
    arg->parser = this;
    arg->is_int = true;
    arg->name = name;
    arg->description = description;
    arg->int_value = new int32_t;
    arguments_[name] = arg;
    names_of_arguments_.push_back(name);

    return *arg;
}

Argument& ArgParser::AddIntArgument(char short_name,
                                    const std::string& long_name,
                                    const std::string& description){
    Argument* arg = new Argument;
    arg->is_int = true;
    arg->parser = this;
    arg->cut_name = short_name;
    arg->name = long_name;
    arg->int_value = new int32_t;
    arg->description = description;
    arguments_[long_name] = arg;
    arguments_[std::string(sizeof(short_name), short_name)] = arg;
    names_of_arguments_.push_back(long_name);

    return *arg;
}


Argument& ArgParser::AddStringArgument(const std::string& long_name,
                                       const std::string& description) {
    Argument* arg = new Argument;
    arg->parser = this;
    arg->name = long_name;
    arg->is_string = true;
    arg->string_value = new std::string;
    arg->description = description;
    arguments_[long_name] = arg;
    names_of_arguments_.push_back(long_name);

    return *arg;
}

Argument& ArgParser::AddStringArgument(char short_name,
                                       const std::string& long_name,
                                       const std::string& description) {
    Argument* arg = new Argument;
    arg->parser = this;
    arg->cut_name = short_name;
    arg->name = long_name;
    arg->is_string = true;
    arg->string_value = new std::string;
    arg->description = description;
    arguments_[long_name] = arg;
    arguments_[std::string(sizeof(short_name), short_name)] = arg;
    names_of_arguments_.push_back(long_name);

    return *arg;
}

bool ArgParser::Help() const {
    if (help == nullptr) {
        return false;
    }

    std::cout << help->description;

    return true;
}

Argument* Argument::Default(const char* value) {
    default_value = true;
    *string_value = std::string(value);
    declaration = true;

    return this;
}

Argument* Argument::Default(bool logic_value) {
    default_value = true;
    *boolean = logic_value;
    declaration = true;

    return this;
}

void Argument::StoreValue(bool& logic_value) {
    delete boolean;
    is_store_value = true;
    boolean = &logic_value;
    declaration = true;
}

void Argument::StoreValue(std::string& value) {
    delete string_value;
    is_store_value = true;
    string_value = &value;
    declaration = true;
}

void Argument::StoreValues(std::vector<int32_t>& values) {
    delete int_values;
    is_store_value = true;
    int_values = &values;
}

void Argument::StoreValues(std::vector<std::string>& values) {
    delete string_values;
    is_store_value = true;
    string_values = &values;
}

Argument& Argument::MultiValue(size_t min_args_count) {
    is_multi_value = true;
    min_cnt_args = min_args_count;

    if (is_int) {
        int_values = new std::vector<int32_t>;
    }

    if (is_string) {
        string_values = new std::vector<std::string>;
    }

    return *this;
}

Argument& Argument::Positional() {
    parser->position = this;

    return *this;
}

std::string Argument::Info() const {
    std::string out;

    if (is_help) {
        return "";
    }

    if (!cut_name.empty()) {
        out += "-" + cut_name + ",  ";
    } else {
        out += "     ";
    }

    if (!name.empty()) {
        out += "--" + name;

        if (is_int) {
            out += "=<int>";
        }

        if (is_string) {
            out += "=<string>";
        }

        out += ",  ";
    }

    out += description;

    if (default_value) {
        out += " [default = ";

        if (is_int) {
            out += std::to_string(*int_value);
        }

        if (is_string) {
            out += *string_value;
        }

        if (is_flag) {
            if (*boolean) {
                out += "true";
            } else {
                out += "false";
            }
        }

        out += "]";
    }

    if (is_multi_value) {
        out += " [repeated, min args = " + std::to_string(min_cnt_args) + "]";
    }

    out += "\n";

    return out;
}

std::string Argument::InfoHelp() const {
    std::string out = "\n";
    if (!cut_name.empty()) {
        out += "-" + cut_name + ", ";
    } else {
        out += "     ";
    }
    if (!name.empty()) {
        out += "--" + name + " ";
    }
    out += "Display this help and exit\n";

    return out;
}

std::string ArgParser::HelpDescription() {
    std::string out;
    out += name_ + "\n";
    out += help->description + "\n\n";
    for (const std::string& name : names_of_arguments_) {
        Argument* argument = arguments_[name];
        out += argument->Info();
    }
    out += help->InfoHelp();

    return out;
}
