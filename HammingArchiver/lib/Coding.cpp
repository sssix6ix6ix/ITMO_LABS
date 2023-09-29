#include "Coding.h"

uint16_t Code(const uint8_t& byte){
    std::bitset<kBitsInByte> first_form = byte;
    std::bitset<kCountCodedBits> change_to(0);
    uint16_t changed_form = 0;
    size_t control_bit = kFirstControlBit;
    size_t pos = kFirstCurrentPos;

    for (size_t bit = 0; bit < kCountCodedBits; bit++) {
        if (bit + kShiftBetweenIndex == control_bit) {
            control_bit *= kBase;
        } else {
            change_to[bit] = first_form[pos];
            pos++;
        }
    }

    for(size_t main_bit = 1; main_bit < kCountCodedBits; main_bit *= kBase){
        size_t true_index = main_bit - kShiftBetweenIndex;
        for (size_t to_xor = true_index; to_xor < kCountCodedBits; to_xor += kBase * main_bit) {
            for (size_t k = 0; k < main_bit && (k + to_xor) < kCountCodedBits; k++) {
                change_to[true_index] = change_to[true_index] ^ change_to[k + to_xor];
            }
        }
    }

    for (size_t bit = 0; bit < kCountCodedBits; bit++) {
        changed_form += (change_to[bit] << bit);
    }

    return changed_form;
}

int8_t Check(const std::bitset<kCountCodedBits>& to) {
    int8_t ans = 0;
    std::bitset<kBitsInByte> checker;

    for(size_t main_bit = 1; main_bit < kCountCodedBits; main_bit *= kBase){
        int16_t true_index = main_bit - kShiftBetweenIndex;
        for (size_t to_xor = true_index; to_xor < kCountCodedBits; to_xor +=  kBase * main_bit) {
            for (size_t xored = 0; xored < main_bit && (xored + to_xor) < kCountCodedBits; xored++) {
                if (true_index != xored + to_xor) {
                    checker[true_index] = checker[true_index] ^ to[xored + to_xor];
                }
            }
        }
        if (checker[true_index] != to[true_index]) {
            ans += main_bit;
        }
    }

    return ans - kShiftBetweenIndex;
}

uint8_t Decode(const uint16_t& coded_byte) {
    std::bitset <kCountCodedBits> bits_of_coded_byte = coded_byte;
    int8_t wrong = Check(bits_of_coded_byte);

    if (wrong != -1) {
        bits_of_coded_byte[wrong] = !bits_of_coded_byte[wrong];
    }

    uint8_t code = 0;
    uint8_t pos = 0;
    uint8_t pow = 1;

    for (size_t bit = 0; bit < kCountCodedBits; bit++) {
        if (bit == pow - kShiftBetweenIndex) {
            pow*=kBase;
        } else {
            code += bits_of_coded_byte[bit] << pos;
            pos++;
        }
    }
    return code;
}

void Coding(std::ofstream& archive, size_t var, size_t count_byte) {
    for (size_t rank = 0; rank < count_byte; rank++, var /= kByte) {
        uint8_t byte = var % kByte;
        uint16_t coded_byte = Code(byte);
        archive.write(reinterpret_cast<char*>(&coded_byte), sizeof(coded_byte));
    }
}

size_t Decoding(std::ifstream& coded_archive, size_t count_byte) {
    size_t var = 0;
    size_t shift = kFirstControlBit;

    for (size_t rank = 0; rank < count_byte; rank++, shift *= kByte) {
        uint16_t coded_byte;
        coded_archive.read(reinterpret_cast<char*>(&coded_byte), sizeof(coded_byte));
        uint8_t decoded_byte = Decode(coded_byte);
        var += shift * decoded_byte;
    }

    return var;
}

void CodingFile(std::ofstream& archive, FileInfo& file_info) {
    CodingFileName(archive, file_info.file_name);
    Coding(archive, file_info.size, sizeof(file_info.size));
    CodingData(archive, file_info);
}

void CodingFileName(std::ofstream& archive, std::string file_name) {
    file_name += kSeparator;
    for (char char_of_file_name : file_name) {
        uint16_t coded_byte = Code(char_of_file_name);

        archive.write(reinterpret_cast<char*>(&coded_byte), sizeof(coded_byte));
    }
}

void CodingData(std::ofstream& archive, const FileInfo& file_info){
    std::ifstream data(file_info.file_name, std::ios::binary);
    data.seekg(0, std::ios::end);
    size_t size_of_data = data.tellg();
    data.seekg(0, std::ios::beg);

    while (data.tellg() < size_of_data) {
        char byte;
        data.get(byte);
        uint16_t coded_byte = Code(byte);
        archive.write(reinterpret_cast<char*>(&coded_byte), sizeof(coded_byte));
    }
}

void FileInfo::CodingFileInfo(std::ofstream& coded_archive) {
    CodingFileName(coded_archive, file_name);
    Coding(coded_archive, size, sizeof(size));
}

void FileInfo::DecodingFileInfo(std::ifstream& coded_archive) {
    file_name = DecodingFileName(coded_archive);
    size = Decoding(coded_archive, sizeof(size));
}

std::string DecodingFileName(std::ifstream& coded_archive) {
    std::string file_name;
    uint8_t decoded_byte = Decoding(coded_archive, sizeof(decoded_byte));

    while (decoded_byte != kSeparator) {
        file_name += static_cast<char>(decoded_byte);
        decoded_byte = Decoding(coded_archive, sizeof(decoded_byte));
    }

    return file_name;
}

void DecodingData(std::ifstream& coded_archive, const FileInfo& file_info) {
    std::ofstream file(file_info.file_name, std::ios::binary);
    for (size_t pos_of_data = 0; pos_of_data < file_info.size; pos_of_data++) {
        uint16_t coded_byte;
        coded_archive.read(reinterpret_cast<char*>(&coded_byte), sizeof(coded_byte));
        uint8_t decoded_byte = Decode(coded_byte);
        file.write(reinterpret_cast<char*>(&decoded_byte), sizeof(decoded_byte));
    }
}
