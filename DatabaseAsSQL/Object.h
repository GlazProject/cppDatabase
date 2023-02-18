#pragma once
#include <iostream>
#include <map>

class Object {
public:
    size_t positionInFile;
    std::map<std::string, std::string> fields;

    void print() {
        for (auto x : fields)
            std::cout << x.second << " ";
        std::cout << std::endl;
    }
};