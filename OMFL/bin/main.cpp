#include "lib/parser.h"

#include <any>
#include <iostream>
#include <vector>

int& f(std::vector<int>& v) {
    return v[0];
}




int main() {
    std::vector<int> v = {1,23};
    int& a = const_cast<int &>(f(v));
    a++;
    std::cout << v[0];
}
//
//Argument* Section::add_section(std::string names) {
//    std::cout << "added section : ........ " <<'\n';
//    size_t pos_point = names.find('.');
//    std::string name_sub = names.substr(0,pos_point);
//    std::string remaining_sub = names.substr(pos_point + 1);
//
//    if (arguments.count(name_sub) == 0) {
//        arguments[name_sub] = Argument().Make_Section(Section());
//    }
//
//    Argument* arg = &arguments[name_sub];
//
//    Argument* returned_arg = arg;
//    Section tmp_section;
//
//    if (arg->Is_empty()) {
//        if (pos_point != std::string::npos) {
//            returned_arg = tmp_section.add_section(remaining_sub);
//        }
//    } else {
//        tmp_section = std::any_cast<Section>(arg->value_);
//        tmp_section.add_section(remaining_sub);
//        arg->value_ = tmp_section;
//    }
//
//    if (!check_name(name_sub) || remaining_sub.empty()) {
//        returned_arg->type_ = "";
//    }
//
//    return returned_arg;
//}