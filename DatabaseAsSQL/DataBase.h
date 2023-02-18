#pragma once
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "STDFunctions.h"
#include "Object.h"
#include "Field.h"


class DataBase {
    friend class UI;
public:
    DataBase();
    DataBase(std::string path, std::string fields = "");

    void append(std::string object);
    void updateRecordInDB(Object object, std::string attribute);
    void removeRecordFromDB(Object object);
    // void updateDB(std::string attribute);

    std::list<Object> find(std::string attribute);
    std::list<Object> findAll();
    
    

private:
    std::string path;
    std::vector<std::string> fields_names;
    std::map<std::string, field> fields;
    
    int infoSize;
    int recordSize;
    size_t size;
    char divider;

    bool isFileOpen;

    int getLastId();
    void createDB(std::string field);
    void setField(std::string name, int id, int startSize, int size, std::string type);
    void setFields(std::vector<std::string> field);
    void writeRecordInDB(std::string object, size_t seekPosition, std::streamoff start = std::ios::beg);
    Object readRecordFromDB(long long seekPosition, std::streamoff start = std::ios::beg);
public:
    std::list<Object> find(std::string attributeName, std::string attributeValue, std::list<Object>);
};