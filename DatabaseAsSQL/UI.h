#pragma once
#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <list>

#include "STDFunctions.h"
#include "DataBase.h"
#include "Object.h"

class UI {
public:
	UI();
	~UI();

	int DoCommand(std::string command);
	int Start();

private:
	DataBase* db;
	std::list<Object> selected;

	char _help[8];
	char _open[8];
	char _close[8];
	char _create[8];
	char _select[8];
	char _delete[8];
	char _deletet[8];
	char _update[8];
	char _insert[8];
	char _exit[8];
	char _where[6];
	char _db[3];
	char _selecetd[9];
	char _from[5];
	std::string _divider;

	int DO(std::string command);

	int Open(std::string args);
	int Close();
	int Create(std::string args);
	int Select(std::string args, bool print);
	int SelectFromDB(std::vector<std::string> args, int bPoint);
	int SelectFromSelected(std::vector<std::string> args, int bPoint, bool print);
	int Insert(std::string args);
	int Delete(std::string args);
	int Update(std::string args);

	void Help();
	void Error(std::string info);
	void Info(std::string info);
	void PrintCollums();
	void PrintSelected();

	std::string CreateFindStr(std::vector<std::string> args, int bPoint);
};

/*

+	OPEN db.txt;
+	CREATE db.txt (id 10 int;name 50 string);
+	SELECT FROM db WHERE user=Nikita, id=5 ;
+	SELECT WHERE group=110208;
+	DELETE WHERE user=Nikita, id=5;
+	DELETE selected;
+	INSERT (name, group, age) values (Nikita, 110208, 24.03.2004);
	UPDATE selected (name) values (Andrey);
+	CLOSE;
+	help

*/