#include "DataBase.h"
//#include "STDFunctions.h"

DataBase::DataBase(){
    this->infoSize = 0;
    this->path = "";
    this->size = 0;
    this->divider = ',';
    isFileOpen = false;
}

DataBase::DataBase(std::string path, std::string fields) {
    this->path = path;
    this->divider = ',';

    std::ifstream file(path);
    if (!file.is_open()) {
        if (fields.empty() || fields == "") {
            isFileOpen = false;
            //throw "Cannot open file";
            return;
        }
        createDB(fields);
        isFileOpen = true;
        return;
    }

    std::getline(file, fields, '#');
    this->infoSize = fields.size() + 2;
    setFields(std::split(fields, divider));
    file.seekg(0, std::ios::end);
    this->size = file.tellg();

    isFileOpen = true;

    file.close();
}


void DataBase::createDB(std::string fields) {
    std::ofstream file(this->path);

    file << fields + '#' << std::endl;
    this->infoSize = fields.length() + 2;
    this->size = infoSize + 1;
    setFields(std::split(fields, divider));

    file.close();
}


int DataBase::getLastId() {
    int id = 0;
    std::ifstream file(this->path);
    file.seekg(-1, std::ios::end);
    int sizeOfFile = file.tellg();
    file.close();
    if (sizeOfFile != this->infoSize) {
        Object lastRecord = readRecordFromDB(-recordSize, std::ios::end);
        id = std::stoi(lastRecord.fields["~id"]);
    }
    return id;
}


void DataBase::append(std::string object) {
    object = std::to_string(getLastId() + 1) + ';' + object;
    writeRecordInDB(object, 0, std::ios::end);
}


//void DataBase::updateDB(std::string attribute) {
//
//    std::vector<std::string> allAttributes = std::split(attribute, ' ');
//    std::vector<std::string> buff;
//
//    std::string fieldsAttributes;
//    createDB(this->path + "temp");
//
//
//    std::ofstream newFile();
//    std::ifstream oldFile(this->path);
//}
