#include <iostream>
#include <Windows.h> 
#include "DataBase.h"
#include "UI.h"

/*void CreateDataBase() {
    UI UI;
    UI.DoCommand("open db.cdb;");

    for (int i = 0; i < 10; i++) {
        std::string name, surname, group, time;
        group = "1102";
        std::mt19937 gen(std::time(0));
        std::uniform_int_distribution<> SYMVOL(48, 57);
		for (int i = 0; i < 2; i++)
			group += SYMVOL(gen);

        name = "Eva";
        surname = "Bodnya";
        time = "02.02.2003";
        UI.DoCommand("insert (name, surname, group, time) values (" + name + ", " + surname + ", " + group + ", " + time + ");");
        
    }

}
*/

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //CreateDataBase();
    UI UI;
    UI.Start();
}