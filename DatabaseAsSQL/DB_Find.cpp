#include "DataBase.h"
//#include "STDFunctions.h"

std::list<Object> DataBase::findAll() {
    std::ifstream file(this->path);
    size_t seekPosition = 0;

    Object record;
    std::list<Object> allRecords;
    while (seekPosition != this->size - (this->infoSize + 1)) {
        record = readRecordFromDB(seekPosition);
        allRecords.push_back(record);
        seekPosition += recordSize;
    }

    file.close();
    return allRecords;
};

std::list<Object> DataBase::find(std::string attribute) {
    std::list<Object> correctRecords = findAll();
    std::vector<std::string> allAttributes = std::split(attribute, ';');
    std::vector<std::pair<std::string, std::string>> queueOfSelect;
    std::vector<std::string> buff;

    for (int i = 0; i < allAttributes.size(); i++) {
        buff = std::split(allAttributes[i], '=');
        queueOfSelect.push_back(std::make_pair(buff[0], buff[1]));
    }

    for (int i = 0; i < queueOfSelect.size(); i++) {
        correctRecords = find(queueOfSelect[i].first, queueOfSelect[i].second, correctRecords);
    }
    std::reverse(correctRecords.begin(), correctRecords.end());
    return correctRecords;
};

std::list<Object> DataBase::find(std::string attributeName, std::string attributeValue, std::list<Object> records) {
    std::list<Object> correctRecords;
//    std::string type;

//    type = this->fields[attributeName].type;
    attributeValue += std::string(this->fields[attributeName].size - attributeValue.size(), ' ');

    for (Object record : records) {
        if (record.fields[attributeName] == attributeValue)
            correctRecords.push_back(record);
    };
    return correctRecords;
}