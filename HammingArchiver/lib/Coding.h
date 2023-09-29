#pragma once

#include "FileInfo.h"

#include <bitset>
#include <fstream>
#include <iostream>

constexpr size_t kCountCodedBits = 12;
constexpr size_t kBitsInByte = 8;
constexpr size_t kFirstCurrentPos = 0;
constexpr size_t kBase = 2;
constexpr size_t kByte = 256;
constexpr size_t kShiftBetweenIndex = 1;
constexpr size_t kFirstControlBit = 1;
constexpr char kSeparator = '\n';

uint16_t Code(const uint8_t& byte);

uint8_t Decode(const uint16_t& coded_byte);

int8_t Check(const std::bitset<kCountCodedBits>& to);

void Coding(std::ofstream& archive,size_t var, size_t count_byte);

size_t Decoding(std::ifstream& coded_archive, size_t count_byte);

void CodingFile(std::ofstream& archive, FileInfo& file_info);

void CodingFileName(std::ofstream& archive, std::string file_name);

void CodingData(std::ofstream& archive,const FileInfo& file_info);

std::string DecodingFileName(std::ifstream& coded_archive);

void DecodingData(std::ifstream& coded_archive,const FileInfo& file_info);
