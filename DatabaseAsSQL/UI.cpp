#include "UI.h"


UI::UI() {
	db = nullptr;
	strcpy_s(_help, "help;  ");
	strcpy_s(_open,"open   ");
	strcpy_s(_close,"close; ");
	strcpy_s(_create,"create ");
	strcpy_s(_select,"select ");
	strcpy_s(_delete,"delete ");
	strcpy_s(_deletet,"delete;");
	strcpy_s(_update,"update ");
	strcpy_s(_insert,"insert ");
	strcpy_s(_exit,"exit;  ");
	strcpy_s(_where,"where");
	strcpy_s(_db,"db");
	strcpy_s(_selecetd,"selected");
	strcpy_s(_from,"from");
	_divider = ";,";
}

UI::~UI() {
	Close();
}

int UI::Start() {
	std::cout << "C++ Database by M&N\n\n";
	while (true) {
		std::string command;
		std::cout << "DB:~$ ";
		getline(std::cin, command);
		int code = DoCommand(command);
		if (code == 2) break;
	}
	return 0;
}

int UI::DoCommand(std::string command) {
	if (command[command.size() - 1] != ';') command += ';';
	std::vector<std::string> commands = std::split(command, ';');
	for (auto c : commands) {
		int code = 0;
		if (c!="")
			code = DO(c);
		if (code == 2) return 2;
	}
	return 0;
}

int UI::DO(std::string command) {
	if (command[command.size() - 1] != ';') command += ';';
	char buf[] = "       ";
	for (int i = 0; i < 7 && i < command.length() ; i++) {
		if (command[i] == ' ')
			break;
		buf[i] = command[i];
	}

	if (std::strcmp(buf, _help) == 0) {
		Help();
		return 0;
	}
	if (std::strcmp(buf, _open) == 0){
		Open(command.substr(5, command.length()-6));
		return 0;
	}
	if (std::strcmp(buf, _close) == 0){
		Close();
		return 0;
	}
	if (std::strcmp(buf, _create) == 0){
		Create(command.substr(7, command.length()-8));
		return 0;
	}
	if (std::strcmp(buf, _select) == 0){
		Select(command.substr(7, command.length()-8), true);
		return 0;
	}
	if (std::strcmp(buf, _delete) == 0 || std::strcmp(buf, _deletet) == 0) {
		Delete(command.substr(7, command.length()-8));
		return 0;
	}
	if (std::strcmp(buf, _insert) == 0){
		Insert(command.substr(7, command.length()-8));
		return 0;
	}
	if (std::strcmp(buf, _update) == 0){
		Update(command.substr(6, command.length() - 7));
		return 0;
	}
	if (std::strcmp(buf, _exit) == 0) {
		return 2;
	}
	Error("Data entry error. Enter \"help\" to see info");
	return 1;
}


void UI::Error(std::string info) {
	std::cerr << "DB:~$ [ERROR] " << info << "\n";
}
void UI::Info(std::string info) {
	std::cout << "DB:~$ [INFO] " << info << "\n";
}


void UI::PrintCollums() {
	int count = 0;
	std::cout << "\n";
	for (auto a : db->fields_names) {
		std::cout << db->fields[a].name;
		for (int i = 0; i < db->fields[a].size - db->fields[a].name.length(); i++)
			std::cout << " ";
		std::cout << " | ";
		count += db->fields[a].size + 3;
	}
	std::cout << "\n";
	for (int i = 0; i < count; i++)
		std::cout << "-";
	std::cout << "\n";
}

void UI::PrintSelected() {
	int count = 0;
	for (auto o : selected) {
		count = 0;
		for (auto a : db->fields_names) {
			std::cout << o.fields[a];
			std::cout << " | ";
			count += db->fields[a].size + 3;
		}
		std::cout << "\n";
	}
	for (int i = 0; i < count; i++)
		std::cout << "-";
	std::cout << "\n\n";
}


int UI::Open(std::string args) {
	db = new DataBase(args);

	if (!db->isFileOpen) {
		Error("DataBase was not found");
		delete db;
		db = nullptr;
		return 1;
	}

	Info("DB is opened");
	return 0;
}

int UI::Close() {
	if (db!=nullptr)
		delete db;
	db = nullptr;
	selected.clear();
	Info("DB is closed");
	return 0;
}

int UI::Create(std::string args) {
	std::vector<std::string> arguments = std::split(args, '(');
	arguments[0] = arguments[0].substr(0, arguments[0].length() - 1);
	arguments[1] = arguments[1].substr(0, arguments[1].length() - 1);
	db = new DataBase(arguments[0], "~id 10 int," + arguments[1]);
	
	if (!db->isFileOpen) {
		Error ("DataBase was not created");
		delete db;
		db = nullptr;
		return 1;
	}

	Info("Database " + arguments[0] + " successfully created");
	return 0;
}

int UI::Select(std::string args, bool print)
{
	if (db == nullptr) {
		Error ("DataBase is not opening");
		return 1;
	}

	if(print) PrintCollums();

	std::vector<std::string> elems = std::split(args, ' ');

	if (elems[0] == _from) {
		if (elems[1] == _db) {
			SelectFromDB(elems, 2);
			return 0;
		}
		if (elems[1] == _selecetd) {
			SelectFromSelected(elems, 2, print);
			return 0;
		}
		Error("Data entry error.Enter \"help\" to see help");
		return 1;
	}

	SelectFromSelected(elems, 0, print);
	return 0;
}

int UI::SelectFromDB(std::vector<std::string> args, int bPoint) {
	if (args[bPoint] == "*") {
		selected = db->findAll();
		PrintSelected();
		return 0;
	}
	if (args[bPoint] == _where) bPoint++;
	std::string findArgs = CreateFindStr(args, bPoint);
	if (findArgs == "") return 1;

	selected = db->find(findArgs);
	PrintSelected();
	return 0;
}

int UI::SelectFromSelected(std::vector<std::string> args, int bPoint, bool print) {
	if (args[bPoint] == _where) bPoint++;
	if (args[bPoint] == "*") {
		selected = db->findAll();
		PrintSelected();
		return 0;
	}
	std::string findArgs = CreateFindStr(args, bPoint);
	if (findArgs == "") return 1;

	if(selected.empty()) selected = db->findAll();
	std::vector<std::string> find_args = std::split(findArgs, ';');
	for (int i = 0; i < find_args.size(); i++) {
		std::vector<std::string> tmp = std::split(find_args[i], '=');
		selected = db->find(tmp[0], tmp[1], selected);
	}
	if(print) PrintSelected();
	return 0;
}

std::string UI::CreateFindStr(std::vector<std::string> args, int bPoint) {
	std::string findArgs = "";
	for (; bPoint < args.size(); bPoint++) {
		if (_divider.find(args[bPoint][args[bPoint].size() - 1]) != -1)
			args[bPoint] = args[bPoint].substr(0, args[bPoint].size() - 1);
		
		std::vector<std::string> tmp = std::split(args[bPoint], '=');
		bool flag = false;
		for (auto a : db->fields_names)
			if (a == tmp[0]) {
				flag = true;
				break;
			}

		if (flag)
			findArgs += args[bPoint] + ";";
		else {
			Error ("Incorrect field name \"" + tmp[0] + "\"");
			return "";
		}
	}
	findArgs = findArgs.substr(0, findArgs.size() - 1);
	return findArgs;
}

int UI::Insert(std::string args) {
	if (db == nullptr) {
		Error ("Database was not open");
		return 1;
	}

	std::map<std::string, std::string> inserting_data;
	for (auto f : db->fields_names) {
		if (f!="~id")
			inserting_data[f] = "";
	}

	int iter_first = 0, iter_second = 0;
	for (; iter_first < args.size(); iter_first++)
		if (args[iter_first] == '(')
			break;
	for (iter_second = iter_first+1;iter_second<args.size();iter_second++)
		if (args[iter_second] == '(')
			break;
	iter_first++; iter_second++;
	std::string buf_field="", buf_value="";
	while (iter_second < args.size()) {
		if (args[iter_first] == ' ' && buf_field.empty()) iter_first++;
		if (args[iter_second] == ' ' && buf_value.empty()) iter_second++;
		if (args[iter_first] != ',' && args[iter_first] != ')') {
			buf_field += args[iter_first];
			iter_first++;
		}
		if (args[iter_second] != ',' && args[iter_second] != ')') {
			buf_value += args[iter_second];
			iter_second++;
		}
		if (args[iter_first] == ',' && args[iter_second] == ',') {
			inserting_data[buf_field] = buf_value;
			buf_field.clear();
			buf_value.clear();
			iter_first++;
			iter_second++;
		}
		if (args[iter_first] == ')' && args[iter_second] == ')') {
			inserting_data[buf_field] = buf_value;
			break;
		}
	}

	if (inserting_data.size() + 1 != db->fields_names.size()) {
		Error ("Some fields do not exist in the DB");
		return 1;
	}

	std::string result = "";

	for (auto f : db->fields_names) {
		if (f != "~id")
			result += inserting_data[f] + ";";
	}
	result = result.substr(0, result.length() - 1);
	db->append(result);
	Info("Successfully recorded");
	selected.clear();
	return 0;
}

int UI::Delete(std::string args) {
	if (db == nullptr) {
		Error("Database was not open");
		return 1;
	}

	if (args.empty() || args == "selected") {
		for (auto o : selected)
			db->removeRecordFromDB(o);
		selected.clear();
		Info("Selected record(s) successfully deleted");
		return 0;
	}

	Select(args, false);
	for (auto o : selected)
		db->removeRecordFromDB(o);
	selected.clear();
	Info("Suitable record(s) successfully deleted");
	return 0;
}

int UI::Update(std::string args) {
	if (db == nullptr) {
		Error("Database was not open");
		return 1;
	}

	std::map<std::string, std::string> inserting_data;
	int iter_first = 0, iter_second = 0;
	for (; iter_first < args.size(); iter_first++)
		if (args[iter_first] == '(')
			break;
	for (iter_second = iter_first + 1; iter_second < args.size(); iter_second++)
		if (args[iter_second] == '(')
			break;
	iter_first++; iter_second++;
	std::string buf_field = "", buf_value = "";
	while (iter_second < args.size()) {
		if (args[iter_first] == ' ' && buf_field.empty()) iter_first++;
		if (args[iter_second] == ' ' && buf_value.empty()) iter_second++;
		if (args[iter_first] != ',' && args[iter_first] != ')') {
			buf_field += args[iter_first];
			iter_first++;
		}
		if (args[iter_second] != ',' && args[iter_second] != ')') {
			buf_value += args[iter_second];
			iter_second++;
		}
		if (args[iter_first] == ',' && args[iter_second] == ',') {
			inserting_data[buf_field] = buf_value;
			buf_field.clear();
			buf_value.clear();
			iter_first++;
			iter_second++;
		}
		if (args[iter_first] == ')' && args[iter_second] == ')') {
			inserting_data[buf_field] = buf_value;
			break;
		}
	}

	std::string update_str = "";

	for (auto my_field : inserting_data) {
		bool flag = false;
		for (auto db_field : db->fields_names) {
			if (my_field.first == db_field)
				flag = true;
		}
		if (!flag) {
			Error("No " + my_field.first + " column in the table");
			return 1;
		}
		update_str += my_field.first + "=" + my_field.second + ";";
	}
	update_str = update_str.substr(0, update_str.size() - 1);
	

	for (auto object : selected) {
		db->updateRecordInDB(object, update_str);
	}

	selected.clear();
	return 0;
}

void UI::Help() {
	std::cout << "\nThis is help:\n";
	std::cout << "	open db.txt;\n";
	std::cout << "	create db.txt (name 50 string, group 10 int); (Do not specify an iterator)\n";
	std::cout << "	select from db where user=Nikita, id=5;\n";
	std::cout << "	select from db *;\n";
	std::cout << "	select where group = 110208; (from selected)\n";
	std::cout << "	select from selected where group = 110208;\n";
	std::cout << "	insert (name, group, age) values (Nikita, 110208, 24.03.2004);\n";
	std::cout << "	delete; (selected record(s)\n";
	std::cout << "	delete selected; (selected record(s)\n";
	std::cout << "	delete where group=110208, name=Nikita;\n";
	std::cout << "	update selected (group, age) values (110209, 00.00.0000);\n";
	std::cout << "	close;\n";
	std::cout << "	exit;\n";
	std::cout << "	help\n\n";
}
