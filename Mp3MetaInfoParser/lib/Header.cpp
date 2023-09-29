#include "Header.h"
#include "frames.h"

void ID3v2Header::ReadIdentifier(std::istream& file) {
    file.read(identifier, kIdHeadLength);
}

void ID3v2Header::ReadSize(std::istream &file) {
    char temp_char;
    file.get(temp_char);
    size += temp_char;
    for (size_t i = 0; i < 3; i++) {
        size = size << 7;
        file.get(temp_char);
        size += temp_char;
    }
}

void ID3v2Header::ReadFlags(std::istream& file) {
    file.read(flags, kFlagsHeadLength);
}

void ID3v2Header::ReadVersion(std::istream& file) {
    file.read(version, kVersionHeadLength);
}

void ID3v2Header::Read(std::istream& file) {
    ReadIdentifier(file);
    ReadVersion(file);
    ReadFlags(file);
    ReadSize(file);
}

void ID3v2Header::Write(std::ostream& file) const{
    file << "Identifier: ";
    file.write(identifier, kIdHeadLength);
    file << std::endl << "Version: ";
    file.write(version, kVersionHeadLength);
    file << std::endl << "Flags: " << std::bitset<8>(*flags) << std::endl;
    file << "Size: " << size << std::endl;
}