#pragma once

#include "Coding.h"
#include "Parser.h"

#include <vector>
#include <unordered_set>

const std::string kAdditionForName = "tmp";
constexpr size_t kBufLen = 4096;

void CreateHAF(const std::string& archive_name,
               const std::unordered_set<std::string>& file_names);

void ExtractHAF(const std::string& archive_name,
                const std::unordered_set<std::string>& files);

void DeleteHAF(const std::string &archive_name,
               const std::unordered_set<std::string> &deleted_files);

void ListHAF(const std::string& archive_name);

void AppendHAF(const std::string& archive_name,
               const std::unordered_set<std::string>& files);

void Concatenate(const std::string& lhs_archive_name,
                 const std::string &rhs_archive_name,
                 const std::string& result_name);

void DoAction(Parser& pars);
