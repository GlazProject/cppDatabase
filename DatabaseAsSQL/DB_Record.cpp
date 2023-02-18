#include "DataBase.h"
//#include "STDFunctions.h"

void DataBase::writeRecordInDB(std::string object, size_t seekPosition, std::streamoff start) {
    std::fstream file(this->path);
    file.seekg(seekPosition, start);
    std::vector<std::string> object_fields = std::split(object, ';');

    std::string record = "";
    for (int i = 0; i < fields.size(); i++) {
        record += object_fields[i];
        record += std::string(fields[fields_names[i]].size - object_fields[i].size(), ' ');
    }

    file << record << std::endl;
    this->size += recordSize;

    file.close();
};


Object DataBase::readRecordFromDB(long long seekPosition, std::streamoff start) {
    std::ifstream file(this->path);
    if (start == std::ios::beg)
        seekPosition += this->infoSize + 1;
    file.seekg(seekPosition, start);
    Object record;
    std::string recordString;
    std::getline(file, recordString);
    field currentField;
    for (int i = 0; i < fields.size(); i++) {
        currentField = fields[fields_names[i]];
        record.fields[currentField.name] = recordString.substr(currentField.startFrom, currentField.size);
    }

    record.positionInFile = file.tellg();
    file.close();
    return record;
};


void DataBase::removeRecordFromDB(Object object) {
    std::ofstream newFile(this->path + "temp");
    std::ifstream oldFile(this->path);

    std::string temp;
    std::getline(oldFile, temp, '#');
    newFile << temp + '#';
    oldFile.seekg(1, std::ios::cur);

    while (!oldFile.eof()) {
        std::getline(oldFile, temp);
        if (oldFile.tellg() != object.positionInFile and oldFile.tellg() != -1)
            newFile << temp << std::endl;
    }

    oldFile.close();
    newFile.close();

    this->size -= recordSize;

    remove(this->path.c_str());
    rename((this->path + "temp").data(), this->path.c_str());
};


void DataBase::updateRecordInDB(Object object, std::string attribute) {
    std::string updatedRecord = "";
    std::vector<std::string> allAttributes = std::split(attribute, ';');
    std::vector<std::string> buff;

    for (int i = 0; i < allAttributes.size(); i++) {
        buff = std::split(allAttributes[i], '=');
        object.fields[buff[0]] = buff[1] + std::string(fields[buff[0]].size - buff[1].size(), ' ');
    }

    for (std::string field : fields_names) {
        updatedRecord += object.fields[field];
    }

    std::fstream file(this->path);
    file.seekg(object.positionInFile - recordSize, std::ios::beg);
    file << updatedRecord << std::endl;
    file.close();
}