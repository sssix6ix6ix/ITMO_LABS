#include "Parser.h"

void Parser::Parsing(int argc, char** argv) {
    for (size_t i = 1; i < argc; i++) {
        if (options.find(std::string(argv[i])) != options.end()) {
            options[std::string(argv[i])] = "used";
            if (std::string(argv[i]) == "-f") {
                options[std::string(argv[i])] = std::string(argv[i + 1]);
                i++;
            }
        } else if (argv[i][0] == '-' && argv[i][1] == '-') {
            bool char_of_name = false;
            for (char c: std::string(argv[i])) {
                if (c == '=') {
                    char_of_name = true;
                } else if (char_of_name) {
                    archive_name += c;
                    options["-f"] = archive_name;
                }
            }
        } else {
            files.insert(argv[i]);
        }
    }
    archive_name = options["-f"];
}
