#pragma once

#include <filesystem>
#include <istream>

#include "section.h"

namespace omfl {

    bool parse(const std::filesystem::path& path);

    Section parse(const std::string& str);

    Section parse(std::istringstream string_stream, Section main_section =  {});

}// namespace

Argument check_array(std::string& value);

Argument check_string(std::string& value);

Argument check_double(std::string& value);

Argument check_int(std::string& value);

Argument check_bool(std::string& value);

bool check_name(std::string& name);

std::pair<bool, Argument> check_value(std::string& value);