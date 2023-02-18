#pragma once

class field {
public:
    field() {
        this->number = 0;
        this->size = 0;
        this->type = "";
        this->startFrom = 0;
        this->name = "";
    }

    field(std::string name, int number, int startFrom, int size, std::string type) {
        this->number = number;
        this->size = size;
        this->type = type;
        this->startFrom = startFrom;
        this->name = name;
    }

    int number;
    int size;
    int startFrom;
    std::string name;
    std::string type;
};