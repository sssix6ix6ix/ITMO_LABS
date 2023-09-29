#pragma once

#include <iostream>
#include <map>
#include <string>
#include <unordered_set>

class Parser {
public:
    std::string archive_name;
    std::unordered_set<std::string> files;
    std::map<std::string, std::string> options = {{"-c", ""}, {"--create", ""},
                                                  {"-f", ""}, {"--file=", ""},
                                                  {"-l", ""}, {"--list", ""},
                                                  {"-x", ""}, {"--extract", ""},
                                                  {"-a", ""}, {"--append", ""},
                                                  {"-d", ""}, {"--delete", ""},
                                                  {"-A", ""}, {"--concatenate", ""}
    };

    void Parsing(int argc, char** argv);

};
