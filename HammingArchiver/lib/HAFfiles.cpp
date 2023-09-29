#include "HAFfiles.h"

void CreateHAF(const std::string& archive_name,
               const std::unordered_set<std::string>& file_names ) {
    std::ofstream archive(archive_name, std::ios::binary);
    for (const std::string& file_name : file_names) {
        FileInfo file;
        file.Create(file_name);
        file.Info();
        CodingFile(archive, file);
    }
    for (const std::string& name : file_names) {
        remove(name.c_str());
    }
}

void ExtractHAF(const std::string& archive_name,
                const std::unordered_set<std::string>& files) {
    std::ifstream coded_archive(archive_name, std::ios::binary);
    coded_archive.seekg(0, std::ios::end);
    size_t archive_size = coded_archive.tellg();
    coded_archive.seekg(0, std::ios::beg);

    while (coded_archive.tellg() != archive_size) {
        FileInfo file_info;
        file_info.DecodingFileInfo(coded_archive);
        if (files.find(file_info.file_name) != files.end() || files.empty()) {
            DecodingData(coded_archive, file_info);
            std::cout << "Extracted " << file_info.file_name << std::endl;
        } else {
            size_t shift_of_next = kBase * file_info.size;
            coded_archive.seekg(shift_of_next, std::ios::cur);
        }
    }
}

void DeleteHAF(const std::string& archive_name,
               const std::unordered_set<std::string>& deleted_files) {

    std::ifstream coded_archive(archive_name, std::ios::binary);
    std::string changed_archive_name = archive_name + kAdditionForName;
    std::ofstream changed_archive(changed_archive_name, std::ios::binary);
    coded_archive.seekg(0, std::ios::end);
    size_t archive_size = coded_archive.tellg();
    coded_archive.seekg(0, std::ios::beg);

    while (coded_archive.tellg() != archive_size) {
        FileInfo file_info;
        file_info.DecodingFileInfo(coded_archive);

        if (deleted_files.find(file_info.file_name) != deleted_files.end()) {
            file_info.Info();
            size_t shift_of_next =  kBase * file_info.size;
            coded_archive.seekg(shift_of_next, std::ios::cur);
        } else {
            file_info.CodingFileInfo(changed_archive);
            for (size_t pos = 0; pos < file_info.size; pos++) {
                char coded_byte[kBase];
                coded_archive.read(coded_byte, sizeof(coded_byte));
                changed_archive.write(coded_byte, sizeof(coded_byte));
            }
        }
    }
    coded_archive.close();
    changed_archive.close();
    remove(archive_name.c_str());
    rename(changed_archive_name.c_str(), archive_name.c_str());
}

void ListHAF(const std::string& archive_name){
    std::ifstream coded_archive(archive_name, std::ios::binary);
    coded_archive.seekg(0, std::ios::end);
    size_t archive_size = coded_archive.tellg();
    coded_archive.seekg(0, std::ios::beg);
    std::cout << "Files in archive :\n";

    while (coded_archive.tellg() < archive_size) {
        FileInfo file_info;
        file_info.DecodingFileInfo(coded_archive);
        std::cout << file_info.file_name << std::endl;
        coded_archive.seekg(kBase * file_info.size, std::ios::cur);
    }
}

void AppendHAF(const std::string& archive_name,
               const std::unordered_set<std::string>& files) {
    std::ofstream coded_archive(archive_name, std::ios::binary|std::ios::app);
    FileInfo file;

    for (const std::string& file_name : files) {
        file.Create(file_name);
        CodingFile(coded_archive, file);
        remove(file_name.c_str());
    }
}

void Concatenate(const std::string& lhs_name,
                 const std::string& rhs_name,
                 const std::string& result_name) {
    std::ofstream result_archive(lhs_name, std::ios::binary|std::ios::app);
    std::ifstream added_haf(rhs_name, std::ios::binary);

    char buf[kBufLen];

    while(!added_haf.eof()){
        added_haf.read(buf, kBufLen);
        if (added_haf.gcount()) {
            result_archive.write(buf, added_haf.gcount());
        }
    }
    result_archive.close();
    added_haf.close();
    rename(lhs_name.c_str(), result_name.c_str());
    remove(rhs_name.c_str());
}

void DoAction(Parser& pars){
    if (pars.options["-c"] == "used" || pars.options["--create"] == "used") {
        CreateHAF(pars.archive_name, pars.files);
    }
    if (pars.options["-l"] == "used" || pars.options["list"] == "used") {
        ListHAF(pars.archive_name);
    }
    if (pars.options["-x"] == "used" || pars.options["--extraxt"] == "used") {
        ExtractHAF(pars.archive_name, pars.files);
    }
    if (pars.options["-a"] == "used" || pars.options["--append"] == "used") {
        AppendHAF(pars.archive_name, pars.files);
    }
    if (pars.options["-d"] == "used" || pars.options["--delete"] == "used") {
        DeleteHAF(pars.archive_name, pars.files);
    }
    if (pars.options["-A"] == "used" || pars.options["--concatenate"] == "used") {
        auto it = pars.files.begin();
        std::string lhs_archive = *(it++);
        std::string rhs_archive = *it;
        Concatenate(lhs_archive, rhs_archive, pars.archive_name);
    }
}
