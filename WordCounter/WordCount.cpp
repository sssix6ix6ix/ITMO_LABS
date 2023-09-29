#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iostream>

size_t lines_counter(const std::string& file_name) {
    std::ifstream file(file_name);
    std::string str;
    int cnt = 0;

    while(getline(file, str)) {
        cnt++;
    }

    return cnt;
}

size_t bytes_counter(const std::string& file_name) {
    std::ifstream file(file_name);
    file.seekg(0, std::ios::end);
    std::streamoff size = file.tellg();

    return size;
}

size_t chars_counter(const std::string& file_name) {
    std::ifstream file(file_name);
    char c;
    int cnt = 0;

    while (file >> c) {
            cnt++;
    }

    return cnt;
}

size_t words_counter(const std::string& file_name) {
    std::ifstream file(file_name);
    std::string str;
    int cnt = 0;

    while (getline(file, str)) {
        int now = 0;

        for ( char i : str) {
            if(i==' ' || i == '\r' || i == '\n' || i == '\t') {
                now = 0;
            } else {
                now++;
            }
            if (now == 1)
                cnt++;
        }
    }
    return cnt;
}

std::map<std::string, int> options={ {"l", 0}, {"--lines", 0},
                                     {"c", 0}, {"--bytes", 0},
                                     {"w", 0}, {"--words", 0},
                                     {"m", 0}, {"--chars", 0}
};

bool all_options = true;

int main(int argc, char** argv) {
    std::vector <std::string> files;
    int i = 1 ;

    for (int i = 1 ; i < argc; i++) {
        if (options.find(std::string(argv[i])) != options.end() || argv[i][0] == '-') {
            if (argv[i][0] == '-' && argv[i][1] != '-') {
                for (char option: (std::string) argv[i]) {
                    options[std::string() + (option)] = 1;
                }
            } else {
                options[argv[i]] = 1;
            }
            all_options = false;
        } else {
            files.emplace_back(std::string(argv[i]));
        }
    }
    for(std::string file_name : files) {
        if(options["l"] || options["--lines"] || all_options)
            std::cout << "lines : " << lines_counter(file_name) << "  ";

        if(options["w"] || options["--words"] || all_options)
            std::cout << "words : " << words_counter(file_name) << "  ";

        if (options["c"] || options["--bytes"] || all_options)
            std::cout << "bytes : " << bytes_counter(file_name) << "  ";

        if(options["m"] || options["--chars"] || all_options)
            std::cout << "chars : " << chars_counter(file_name) << "  ";

        std::cout << file_name << '\n';

     }

}


/*
-l, --lines вывод только количества строк

-c, --bytes вывод размера файла в байтах

-w, --words вывод количества слов

-m, --chars вывод количества букв*

 */
