#include "DataBase.h"
//#include "STDFunctions.h"


void DataBase::setFields(std::vector<std::string> fields) {
    int size; std::string type, name;
    std::vector<std::string> field_settings;

    int startFrom = 0;

    for (int i = 0; i < fields.size(); i++) {
        std::string a = std::trim(fields[i]);
        field_settings = std::split(std::trim(fields[i]), ' ');
        name = field_settings[0];
        size = std::stoi(field_settings[1]);
        type = field_settings[2];
        setField(name, i, startFrom, size, type);
        startFrom += size;
    }

    this->recordSize = startFrom + 2;
}


void DataBase::setField(std::string name, int id, int startFrom, int size, std::string type) {
    this->fields_names.push_back(name);
    this->fields[name] = field(name, id, startFrom, size, type);
}