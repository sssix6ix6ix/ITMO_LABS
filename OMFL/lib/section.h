#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <vector>

#ifndef LAB6_SECTION_H
#define LAB6_SECTION_H

class Section;

class Argument{
public:
    std::string comment_;
    std::any value_;
    std::string type_;

    Argument operator[](int i) {
        if (type_ != "array") {
            return {};
        }

        std::vector<Argument> arr = std::any_cast<std::vector<Argument>>(value_);

        if (i >= arr.size()) {
            return {};
        }

        return arr[i];
    }

    std::string GetType();

    std::any GetValue();

    bool IsArray();

    bool IsBool();

    bool IsFloat();

    bool IsInt();

    bool IsString();

    int32_t AsInt();

    int32_t AsIntOrDefault(int32_t int_value);

    float AsFloat();

    float AsFloatOrDefault(float float_value);

    std::string AsString();

    std::string AsStringOrDefault(std::string string_value);

    bool AsBool();

    Argument MakeSection(Section value);

    Argument(std::any value, std::string type){
        value_ = value;
        type_ = type;
    };

    Argument(){};

    bool IsEmpty() const;

    Section AsSection();

    Argument Get(std::string);
};

class Section {
public:
    bool is_valid = true;
    std::unordered_map<std::string, Argument> arguments;

    Section& add_section(std::string names);

    Argument Get(std::string key);

    bool valid();
};

#endif //LAB6_SECTION_H
