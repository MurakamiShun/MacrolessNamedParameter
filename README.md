# MacrolessNamedParameter

C++20 Macro less named parameter library.

```
#include <iostream>
#include "MacrolessNamedParameter.hpp"

using namespace MacrolessNamedParameter;

void func(Arg<"first", int> first, Arg<"second", int> second){
    std::cout << std::string_view(first.name) << ":" << first.value
       << "," << std::string_view(second.name) << ":" << second.value << std::endl;
}
void func(Arg<"second", int> second, Arg<"first", int> first){
    std::cout << std::string_view(first.name) << ":" << first.value
       << "," << std::string_view(second.name) << ":" << second.value << std::endl;
}

int main() {
    const int a = 99;
    func("first"_arg = a, "second"_arg = 88);
}
```

# License
This software is released under the MIT License, see [LICENSE](LICENSE).