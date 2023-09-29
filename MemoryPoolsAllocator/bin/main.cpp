#include <iostream>
#include "lib/PoolAllocator.h"
#include <vector>
#include <list>
#include <array>

int main() {
    std::vector<int, PoolAllocator<int, default_number_of_pools , default_size_of_pool , default_size_of_block>> v;

    for (int i = 0; i < 5; i++) {
        v.push_back(i);
    }

    for (int x : v) std::cout << x << '\n';
}
