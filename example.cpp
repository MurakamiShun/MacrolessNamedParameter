#include <iostream>
#include "MacrolessNamedParameter.hpp"

using namespace MacroLessNamedParameter;

void func(Arg<"first"_arg, int> first, Arg<"second"_arg, int> second){
    std::cout << std::string_view(first.name) << ":" << first.value
       << "," << std::string_view(second.name) << ":" << second.value << std::endl;
}
void func(Arg<"second"_arg, int> first, Arg<"first"_arg, int> second){
    std::cout << std::string_view(first.name) << ":" << first.value
       << "," << std::string_view(second.name) << ":" << second.value << std::endl;
}

int main() {
    const int a = 99;
    func("first"_arg = a, "second"_arg = 88);
}