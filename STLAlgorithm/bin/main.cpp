#include <iostream> 
//#include <algorithm> 
#include <vector>
#include <set>
#include <list>

#include "lib/my_algorithms.h"

bool f(int a) { 
    return a % 2;
}

int main() {
std::list l = {1, 2, 3, 4, 5};
std::vector<char> v = {'a', 'b', 'c', 'd'};
std::string s = "gdsggs";
/*
1 a
2 b
3 c
4 d
*/

for(auto value : algo::zip(s, v)) {
  std::cout << value.first << " " << value.second << std::endl;
}    
}