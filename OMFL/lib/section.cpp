//
// Created by 123 on 19.12.2022.
//

#include "section.h"
#include <utility>
#include <iostream>

Argument Argument::MakeSection(Section value) {
    value_ = value;
    type_ = "section";

    return *this;
}

Section Argument::AsSection() {
    return std::any_cast<Section>(value_);
}

bool Section::valid() {
    return is_valid;
}

Argument Section::Get(std::string key) {
    std:: cout << "remaining : " << key << '\n';

    if (key.find('.') != std::string::npos) {
        size_t pos_point = key.find('.');
        std::string name = key.substr(0, pos_point);
        std::string remaining = key.substr(pos_point + 1);
        Argument arg = arguments[name];
        std::cout << arg.type_ << '\n';
        if (arg.type_ == "section") {
            std::cout << key << '\n';
            Section tmp_section = std::any_cast<Section>(arg.value_);
            return tmp_section.Get(remaining);
        }
    }

    std::cout << key << " " << arguments[key].type_ << " : type " << '\n';

    return arguments[key];
}

bool Argument::IsEmpty() const {
    return type_.empty();
}

std::string Argument::GetType() {
    return type_;
}

std::any Argument::GetValue() {
    return value_;
}

bool Argument::IsInt() {
    std::cout << type_ << " " << "type\n";
    return (type_ == "int");
}

int32_t Argument::AsInt() {
    return std::any_cast<int32_t>(value_);
}

int32_t Argument::AsIntOrDefault(int32_t int_value) {
    if (type_ != "int") return int_value;

    return AsInt();
}

bool Argument::IsFloat() {
    return (type_ == "float");
}

float Argument::AsFloat() {
    return std::any_cast<float>(value_);
}

float Argument::AsFloatOrDefault(float float_value) {
    if (type_ != "float") {
        return float_value;
    }

    return AsFloat();
}

bool Argument::IsString() {
    return (type_ == "string");
}

std::string Argument::AsString() {
    return std::any_cast<std::string>(value_);
}

std::string Argument::AsStringOrDefault(std::string string_value) {
    if (IsString()) {
        return AsString();
    }

    return string_value;
}

bool Argument::IsArray() {
    return (type_ == "array");
}

bool Argument::IsBool() {
    return (type_ == "bool");
}

bool Argument::AsBool() {
    return std::any_cast<bool>(value_);
}

Argument Argument::Get(std::string name) {
    if (type_ == "section") {
        return AsSection().Get(name);
    } else {
        return {};
    }
}
