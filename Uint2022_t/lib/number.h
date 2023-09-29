#pragma once

#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

const int64_t BASE_OF_RANK = 1000 * 1000 * 1000;    //10e9
const int64_t MAX_RANK = 73 ;
const int64_t MAX_DIGIT_SYS10 = 9;
const int64_t LEN_OF_RANK = 9;
struct uint2022_t {
    int8_t max_rank;
    int32_t rank[MAX_RANK+1];

    explicit uint2022_t() {
        for (int32_t& i: rank) {
            i = 0;
        }
        max_rank = 0;

    }

     uint2022_t(uint32_t n) {
        for (int32_t& i: rank) {
            i = 0;
        }
        rank[0] = n % BASE_OF_RANK;
        rank[1] = n / BASE_OF_RANK;
        max_rank = (rank[1] != 0);
    }

     uint2022_t(const char* buff) {
        uint32_t now_low_rank = 0;
        int64_t p = 1;
        max_rank = 0;
        for (int32_t& i: rank) {
            i = 0;
        }
        uint32_t size = 0;
        auto it = buff;
        while (*it != '\0'){
            size++;
            it++;
        }
        for (it = buff + size - 1; it >= buff; it--) {
            if (now_low_rank == LEN_OF_RANK) {
                p = 1;
                max_rank++;
                now_low_rank = 0;
            }

            rank[max_rank] += p * (*it - '0');
            now_low_rank++;

            p *= 10;
        }
        max_rank = MAX_RANK;
        while (rank[max_rank] == 0 && max_rank) {
            max_rank--;
        }
    }
};


static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);

uint2022_t from_string(const char* buff);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator<=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);

uint2022_t operator+=(uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-=(uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*=(uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/=(uint2022_t& lhs, const uint2022_t& rhs);
