#pragma once
#include <vector>
#include <iostream>
#include "Object.h"

namespace std {
    _NODISCARD inline vector<string> split(string str, char delim) {
        vector<string> result1;
        string buff = "";

        for (int i = 0; i < str.length(); i++) {
            if (str[i] == delim) {
                result1.push_back(buff);
                buff.clear();
                continue;
            }

            buff += str[i];
        }
        result1.push_back(buff);
        return result1;
    }

    _NODISCARD inline string to_string(Object object) {
        string str = "";
        for (pair<string, string> field : object.fields) {
            for (int i = field.second.size() - 1; i--; i >= 0) {
                if (field.second[i] != ' ') {
                    str += field.second.substr(0, i + 1) + ' ';
                    break;
                }
            }
        }
        return str;
    }

    _NODISCARD inline string to_lowercase(string str) {
        for (int i = 0; i < str.size(); i++) {
            if (str[i] >= 'A' && str[i] <= 'Z')
                str[i] = str[i] + 33;
        }
        return str;
    }

    _NODISCARD inline string trim(string str) {
        int indexB = str.find_first_not_of(' ');
        int indexE = str.find_last_not_of(' ');
        str = str.substr(indexB, indexE - indexB);
        return str;
    }
}