#include "parser.h"
#include "section.cpp"
#include <memory>
#include <iterator>
#include <iostream>
#include <stack>

namespace omfl {

    bool parse(const std::filesystem::path &path);

}

Section omfl::parse(const std::string& str) {

    Section main_section;
    std::string way_to_section;

    std::string now_str;
    std::string key;
    std::string value;
    std::istringstream string_stream(str);

    while (std::getline(string_stream, now_str,'\n')) {

        std::cout << now_str << '\n';

        Section& now_section = main_section.add_section(way_to_section);

        while (isspace(now_str[0])) now_str = now_str.substr(1);

        if (now_str[0] == '#' || now_str.empty()) continue;

        auto pos_eq = now_str.find('=');

        if (pos_eq == std::string::npos) {
            if (now_str[0] == '[' && now_str.back() == ']') {
                way_to_section = now_str.substr(1, now_str.size() - 2);
               if (way_to_section.empty()) {
                   now_section.is_valid = false;
               }

            } else {
                now_section.is_valid = false;
            }

        } else {
            key = now_str.substr(0, pos_eq);
            value = now_str.substr(pos_eq + 1);
            now_section.is_valid &= (check_name(key) && check_value(value).first);

            if (!now_section.arguments[key].IsEmpty()) {
                now_section.is_valid = false;

            } else {
                Argument arg = check_value(value).second;
                now_section.arguments[key] = arg;
            }
        }

        main_section.is_valid &= now_section.is_valid;
    }

    Section check_way = main_section.add_section(way_to_section);

    main_section.is_valid &= check_way.is_valid;

    return main_section;
}

Section& Section::add_section(std::string to) {

    std::cout << to << '\n';
    auto pos_point = to.find('.');
    std::string name = to.substr(0, pos_point);
    std::string remaining;

    if (pos_point != std::string::npos){
        remaining = to.substr(pos_point + 1);
    }

    if (to.empty()) {
        return *this;
    }

    if (arguments[name].IsEmpty()) {
        arguments[name].MakeSection({});
    }

    Section& tmp_section = std::any_cast<Section&>(arguments[name].value_);

    if ((!check_name(name)) || (to.back() == '.') || (to[0] == '.')) {
        std::cout << "error : " << to << '\n';
        tmp_section.is_valid = false;
    }

    Section& returned_section = tmp_section.add_section(remaining);
    returned_section.is_valid &= tmp_section.is_valid;

    return returned_section;
};

bool check_name(std::string& name){
    bool flag = true;
    size_t first_char = 0;

    while (name[first_char] == ' ') {
        first_char++;
    }

    name = name.substr(first_char);
    size_t last_char = name.size();

    while (name[last_char-1] == ' ') {
        last_char--;
    }

    name = name.substr(0, last_char);

    for (char c : name){
        if (!std::isalpha(c) && c != '_' && !std::isdigit(c) && c!='-') {
            flag = false;
        }
    }

    if (name.empty()) flag = false;

    return flag;
}

Argument check_string(std::string& value) {
    std::string string_value;
    if (value[0] != '\"' || value.back() != '\"') {
        return {};
    }

    size_t cnt_anvil = 0;
    for (char c: value) {
        if (c == '\"' || c == '\n') {
            ++cnt_anvil;
        }
        else {
            string_value += c;
        }
    }

    if (cnt_anvil != 2) {
        return {};
    }

    return {std::make_any<std::string>(string_value), "string"};
}

Argument check_double(std::string& value) {
    std::string float_str;
    std::unordered_map<char, bool> sign_char = {{'-', true},
                                                {'+', true}};
    size_t cnt_point = 0;
    bool sign = true;
    size_t pos_point = value.find('.');
    if (pos_point == std::string::npos
    || (pos_point == value.size() - 1)
    || (pos_point == 0)
    || (sign_char[value[0]] && pos_point == 1)) {
        return {};
    }

    for (char c : value) {
        float_str+=c;

        if (sign && (c == '+' || c == '-') && value.size() > 1) {
            sign = false;
            continue;
        }

        if (std::isdigit(c)) {
            continue;
        }

        ++cnt_point;
        sign = false;
    }

    if (cnt_point > 1) {
        return {};
    }

    return {std::stof(float_str), "float"};
}

Argument check_int(std::string& value) {
    std::string int_value;
    bool sign = true;
    for (char digit : value) {
        int_value += digit;
        if (sign && (digit == '+' || digit == '-') && value.size() > 1) {
            sign = false;
        } else if (!std::isdigit(digit)) {
            return {};
        }

        sign = false;
    }

    if (int_value[0] == '+') int_value = int_value.substr(1);

    return {std::stoi(int_value), "int"};
}

Argument check_bool(std::string& value) {
    if (value == "true") {
        return {true, "bool"};
    }
    if (value == "false") {
        return {false, "bool"};
    }

    return {};
}

Argument check_array(std::string& value) {
    std::vector<Argument> array;
    Argument arg;
    if (value[0] != '[' || value.back() != ']') {
        return {};
    }
    if (value[0] == '[' && value[1] == ']' && value.size() == 2) {
        return {array, "array"};
    }
    std::string current_value;
    std::stack<char> bracket;
    bool is_array = true;
    size_t last_char = value.size() -1;

    for (size_t i = 1; i < last_char; i++) {
        if (value[i] == ',') {
            arg = check_value(current_value).second;
            is_array &= !(arg.IsEmpty());
            current_value = "";
            array.emplace_back(arg);

        } else {
            current_value += value[i];
        }

        if (value[i] == '\"') {
            while (value[i] != '\"' && i < last_char) {
                current_value += value[i];
            }
        }

        if (value[i] == '[') {
            int cnt_bracket = 1;
            i++;
            while (cnt_bracket !=0 && i < last_char) {
                if (value[i] == ']') {
                    cnt_bracket--;
                }

                if (value[i] == '[') {
                    cnt_bracket++;
                }

                current_value += value[i];
                i++;
            }
            i--;
        }
    }

    arg = check_value(current_value).second;
    is_array &= !(arg.IsEmpty());
    array.emplace_back(arg);

    if (is_array) {
        return {array, "array"};
    }

    return {};
}

std::pair<bool, Argument> check_value(std::string& value){
    Argument arg;
    size_t first_char = 0;

    if (value.find('#') != std::string::npos) {
        value = value.substr(0, value.find('#'));
    }

    while(value[first_char] == ' ') {
        first_char++;
    }

    value = value.substr(first_char);
    size_t last_char = value.size();

    while (value[last_char-1] == ' ') {
        last_char--;
    }

    value = value.substr(0, last_char);

    if (value.empty()) {
        return {false, {}};
    }

    arg = check_string(value);
    if (!arg.IsEmpty()) {
        std::cout << value << "is : " << arg.GetType() << '\n';
        return {true, arg};
    }

    arg = check_double(value);
    if (!arg.IsEmpty()) {
        std::cout << value << "is : " << arg.GetType() << '\n';
        return {true, arg};
    }

    arg = check_int(value);
    if (!arg.IsEmpty()) {
        std::cout << value << "is : " << arg.GetType() << '\n';

        return {true, arg};
    }

    arg = check_bool(value);
    if (!arg.IsEmpty()) {
        std::cout << value << "is : " << arg.GetType() << '\n';

        return {true, arg};
    }

    arg = check_array(value);
    if (!check_array(value).IsEmpty()) {
        std::cout << value << "is : " << arg.GetType() << '\n';

        return {true, arg};
    }

    std::cout << value << " : is invalid" << '\n';
    return {false, {}};
}

std::pair<Argument,std::string> Get_key(const std::string& init_key, Section& section){
    Argument some_argument;
    std::string type;
    std::string name;
    std::string value;
    auto iter = init_key.begin();

    while (isspace(*iter)) {
        iter++;
    }

    for(; *iter != '='; iter++) {
            name+= *iter;
            if (!check_name(name)) section.is_valid = false;
    }

    while (isspace(*iter)) {
        iter++;
    }

    while (iter != init_key.end()) {
        value += *iter;
    }

    return {some_argument, type};
}