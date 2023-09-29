#include "parser.h"

#include <map>

void Parser::Parsing(int argc, char **argv) {
    std::map<std::string, std::string> options = {{"-l", ""}, {"--lenght", ""},
                                                  {"-w", ""}, {"--width" , ""},
                                                  {"-i", ""}, {"--input" , ""},
                                                  {"-o", ""}, {"--output", ""},
                                                  {"-m", ""}, {"--max-iter", ""},
                                                  {"-f", "0"}, {"--freq", "0"}
    };

    for (size_t i = 1; i < argc; i++) {
        if (options.find(std::string(argv[i])) != options.end()) {
            options[std::string(argv[i])] = std::string(argv[i+1]);
            i++;
        }
    }

    if (!options["-l"].empty()) {
        height = std::atoi(options["-l"].c_str());
    } else {
        height = std::atoi(options["--length"].c_str());
    }

    if (!options["-w"].empty()) {
        width = std::atoi(options["-w"].c_str());
    } else {
        width = std::atoi(options["--width"].c_str());
    }

    if (!options["-i"].empty()) {
        input = options["-i"] ;
    } else {
        input = options["--input"];
    }

    if (!options["-o"].empty()) {
        output = options["-o"];
    } else {
        output = options["--output"];
    }

    if (!options["-m"].empty()) {
        max_iter = std::atoi(options["-m"].c_str());
    } else {
        max_iter = std::atoi(options["--max-iter"].c_str());
    }

    if (options["-f"] != "0") {
        freq = std::atoi(options["-f"].c_str());
    } else {
        freq = std::atoi(options["--freq"].c_str());
    }
}
