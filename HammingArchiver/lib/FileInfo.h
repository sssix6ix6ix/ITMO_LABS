#pragma once

#include <fstream>
#include <string>
#include <iostream>


class FileInfo {
public:
    std::string file_name;
    uint64_t size;

    FileInfo(){
        file_name = "";
        size = 0;
    };

    void Create(const std::string& name);

    void Info() const;

    void CodingFileInfo(std::ofstream& coded_archive);

    void DecodingFileInfo(std::ifstream& coded_archive);
};
