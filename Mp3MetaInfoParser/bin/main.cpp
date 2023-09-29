#include "lib/Mp3.h"

#include <filesystem>
#include <stdexcept>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        throw std::runtime_error("Uncorrect input");
    }

    std::filesystem::path file_name(argv[1]);
    std::ifstream input(file_name, std::ios_base::binary);


    if (!input.is_open()) {
        throw std::runtime_error("Bad filepath");
    }

    Mp3 mp3;
    input >> mp3;

    std::string out_file = argv[2];
    std::ofstream output(out_file);
    output << mp3;
}
