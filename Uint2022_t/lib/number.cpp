#include "number.h"


uint2022_t from_uint(uint32_t i) {
    return {i};
}

uint2022_t from_string(const char* buff) {
    return uint2022_t(buff);
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {

    uint2022_t res;
    if (rhs.max_rank > lhs.max_rank) {
        res.max_rank = rhs.max_rank;
    } else {
        res.max_rank = lhs.max_rank;
    }
    uint64_t temp = 0;
    for (uint64_t i = 0; i < MAX_RANK; i++) {
        temp += lhs.rank[i] + rhs.rank[i];

        res.rank[i] += temp % BASE_OF_RANK;
        if (res.rank[i] != 0) res.max_rank = i;
        temp /= BASE_OF_RANK;
    }

    if (res.max_rank > 68) {
        res = uint32_t(0);
        res.max_rank = 0;
    }

    return res;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {

    uint2022_t res;
    bool tmp = 0;
    for (uint64_t i = 0; i < MAX_RANK; i++) {
        if (rhs.rank[i] > lhs.rank[i] + tmp) {
            res.rank[i] = BASE_OF_RANK + lhs.rank[i] - rhs.rank[i] - tmp;
            tmp = 1;
        } else {
            res.rank[i] = lhs.rank[i] - rhs.rank[i] - tmp;
            tmp = 0;
        }

    }
    res.max_rank = MAX_RANK;
    while (res.rank[res.max_rank] == 0 && res.max_rank) {
        res.max_rank--;
    }

    return res;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {

    uint2022_t res;
    uint64_t temp = 1;
    if (lhs.max_rank + rhs.max_rank > 68) {
        res.max_rank = 0;
        return res;
    }
    for (uint64_t i = 0; i < MAX_RANK && i <= lhs.max_rank; i++) {
        for (uint64_t j = 0; j < MAX_RANK && j <= rhs.max_rank; j++) {
            temp = lhs.rank[i];
            temp *= rhs.rank[j];

            if (temp != 0 && i + j < MAX_RANK) {
                res.rank[i + j + 1] += uint64_t(temp / BASE_OF_RANK);
                res.rank[i + j] += uint64_t(temp % BASE_OF_RANK);
                res.rank[i + j + 1] += res.rank[i + j] / BASE_OF_RANK;
                res.rank[i + j] %= BASE_OF_RANK;
            }
        }
    }
    res.max_rank = MAX_RANK;
    while (res.rank[res.max_rank] == 0 && res.max_rank) {
        res.max_rank--;
    }

    return res;
}


uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (rhs > lhs) return uint2022_t();
    uint2022_t temp;
    std::string res;
    uint64_t pow_10 = 0;
    pow_10 += LEN_OF_RANK * lhs.max_rank;
    for(size_t i = 1; i <= lhs.rank[lhs.max_rank];i *= 10) {
        pow_10++;
    }

    while (pow_10) {
            temp = temp * 10;
            uint64_t mod_pow_10 = pow_10 % 9;
            if (mod_pow_10 == 0) {
                mod_pow_10 = 9;
            }
            uint64_t up_pow = pow(10, mod_pow_10);
            uint64_t less_pow = pow(10, mod_pow_10 - 1);
            temp += (lhs.rank[(pow_10-1) / 9] % up_pow / less_pow);
            pow_10--;

        uint32_t temp_div = 9;
        while (temp_div * rhs > temp) {
            temp_div--;
        }

        temp = temp - (temp_div * rhs);

        res += char('0' + temp_div);
    }

    return uint2022_t( res.c_str() );
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (uint64_t i = 0; i < MAX_RANK; i++) {
        if (lhs.rank[i] != rhs.rank[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (uint64_t i = 0; i < MAX_RANK; i++) {
        if (lhs.rank[i] != rhs.rank[i]) {
            return true;
        }
    }

    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {

    for (uint64_t i = value.max_rank; i >= 0; i--) {
        if (i != value.max_rank) {
            size_t len_of_rank = 0;
            size_t one_of_rank = value.rank[i];
            while (one_of_rank /= 10) {
                len_of_rank++;
            }
            for (uint64_t j = len_of_rank + 1; j < 9; j++) {
                stream << 0;
            }
        }
        stream << value.rank[i];
    }

    return stream;
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (uint64_t i = MAX_RANK; i >= 0; i--) {
        if (lhs.rank[i] > rhs.rank[i]) {
            return true;
        }

        if (lhs.rank[i] < rhs.rank[i]) {
            return false;
        }
    }

    return false;
}


bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs < rhs);
}

bool operator<=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs > rhs);
}

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs) {

    for (uint64_t i = MAX_RANK; i >= 0; i--) {
        if (lhs.rank[i] > rhs.rank[i]) {
            return false;
        }
        if (lhs.rank[i] < rhs.rank[i]) {
            return true;
        }
    }

    return false;
}

uint2022_t operator+=(uint2022_t& lhs, const uint2022_t& rhs) {
    return lhs = lhs + rhs ;
}

uint2022_t operator-=(uint2022_t& lhs, const uint2022_t& rhs) {
    return lhs = lhs - rhs ;
}

uint2022_t operator*=(uint2022_t& lhs, const uint2022_t& rhs) {
    return lhs = lhs * rhs ;
}

uint2022_t operator/=(uint2022_t& lhs, const uint2022_t& rhs) {
    return lhs = lhs / rhs ;
}
