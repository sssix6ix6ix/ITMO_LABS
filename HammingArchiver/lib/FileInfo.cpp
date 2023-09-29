#include "FileInfo.h"

void FileInfo::Create(const std::string& name) {
    file_name = name;
    std::ifstream file(name, std::ios::binary);
    file.seekg(0,std::ios::end);
    size = file.tellg();
}

void FileInfo::Info() const {
    std::cout << "file name : " << file_name << '\n';
    std::cout << "size : " << size << '\n';
}
