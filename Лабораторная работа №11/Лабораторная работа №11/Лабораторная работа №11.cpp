#pragma warning(disable : 4996)
#include <iostream>
#include <iomanip>
#include <fstream>
#include <conio.h>
#include<time.h>
#include<stdlib.h>
#include <string>
#include <sstream>
using namespace std;

const int NAME = 10;         // количество имен в массиве
const char* names[::NAME] = { "Sasha", "Pasha", "Dasha", "Kolya", "Olya",
                                "Yra", "Semen", "Artem", "Petya", "Lena" };

#define Size (100)//Размер массива

typedef struct {//Структура
    string FIO;
    int Schet;
    float sum;
}Info;

int calcSum(string str);//ключ
int Global_Coalize = 0;//общее количество коализий
Info generation();//генерация случайной информации
void InitHash(Info*& hashTable, bool*& used);//Инициализация таблицы
int hashF(int v);//Хеш-фция
int ReHash(int idx, int i);//ф-ция рехеширования
int AddInTable(Info*& hashTable, bool*& used, Info n);//добавление в таблицу
bool FindInTable(Info* hashTable, bool* used, string n, int& Count);//поиск в таблице
void printTable(Info* hashTable, bool* used);//печать таблицы
void load_from_file(const char* fn, Info*& hashTable, bool*& used);//загрузка из файла
void save_to_file(const char* fn, Info* hashTable, bool* used);//сохранение в файл
void DeleteTable(Info*& hashTable, bool*& used, string n);//удаление из таблицы
int main()
{

    Info* hashTable;
    bool* used;
    char ch;
    InitHash(hashTable, used);
    srand(time(NULL));
    while (true) {
        cout << "1- Generation\n";
        cout << "2- Add Table\n";
        cout << "3- Delete key \n";
        cout << "4- Find\n";
        cout << "5- Print Table and count coalize\n";
        cout << "6- load from file\n";
        cout << "0- Exit\n";
        ch = getch();
        switch (ch) {
        case '1': {//генерация случайных данных
            int count;
            cout << "Count element(>=100)=";
            cin >> count;//вводим количество
            InitHash(hashTable, used);//Очищаем таблицы
            for (int i = 0; i < count; i++) {//заполняем случайными значениями
                Info val = generation();
                AddInTable(hashTable, used, val);
            }
            break;
        }
        case '2': {//добавление в таблицу
            cout << "FIO:";
            Info  val;
            cin >> val.FIO;
            cout << "Schet:";
            cin >> val.Schet;
            cout << "Price:";
            cin >> val.sum;
            cout << "Count coalize=" << AddInTable(hashTable, used, val) << endl;
            break;
        }
        case '3': {//удаление из таблицы
            cout << "Delete Value key(FIO):";
            string val;
            cin >> val;
            DeleteTable(hashTable, used, val);
            break;
        }
        case '4': {//поиск
            cout << "Find Value key(FIO):";
            int Count = 0;
            string val;
            cin >> val;
            if (FindInTable(hashTable, used, val, Count)) {
                cout << "Find! Count coalize=" << Count << endl;
            }
            else
                cout << "Not Find!\n";
            break;
        }
        case '5': {//вывод таблицы
            printTable(hashTable, used);
            cout << "\n--------------------------------------------------------------------------\n" <<
                "Global coalize=" << Global_Coalize << endl;
            break;
        }
        case '6': {//загрузка таблицы
            load_from_file("data.fl", hashTable, used);
            cout << "\nload data in file\n";
            printTable(hashTable, used);
            cout << "\n--------------------------------------------------------------------------\n" <<
                "Global coalize=" << Global_Coalize << endl;
            break;
        }
        case '0': {//выход из программы
            save_to_file("data.fl", hashTable, used);//сохраняем таблицу в файл
            delete hashTable;
            delete used;
            return 0;
            break;
        }
        default: {
            cout << "Uncknow command\n";
            break;
        }
        }
    }

    return 0;
}
//------------------------------------------------------------------------------------
int calcSum(string str) {//сумма символов в строке(ключ)
    int sum = 0;
    for (int i = 0; i < str.size(); i++) {
        sum += str[i];
    }
    return sum;
}
//-----------------------------------------------------------------------------------
string RandomString(int len)//генерация случайной строки
{
    string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
    string newstr;
    int pos;
    while (newstr.size() != len) {
        pos = ((rand() % (str.size() - 1)));
        newstr += str.substr(pos, 1);
    }
    return newstr;
}
//-------------------------------------------------------------
Info generation() {//генерация случайной информации
    Info new_d;
    new_d.FIO = names[rand() % ::NAME] + string(" ")+ RandomString(rand() % 10);
    new_d.Schet = rand() % 100 + 300;
    new_d.sum = 1.0 * (rand() % 10000 + 300) / (rand() % 100 + 30);
    return new_d;
}
//-----------------------------------------------------------
void DeleteTable(Info*& hashTable, bool*& used, string n) {//удаление из таблицы
    int index = hashF(calcSum(n));//вычисляем хеш
    int i = 0;
    while (used[index] && hashTable[index].FIO != n) {//поиск по ключу
        index = ReHash(index, i);
        i++;
    }
    if (used[index] && hashTable[index].FIO == n) {//если нашли удаляем
        used[index] = false;
    }
}
//--------------------------------------------------------
void save_to_file(const char* fn, Info* hashTable, bool* used) {//сохранение в файл
    fstream f;
    f.open(fn, fstream::out);

    if (!f.is_open())
    {
        cerr << "error save to file!";
        return;
    }
    for (int i = 0; i < Size; i++) {
        if (used[i]) {
            f << hashTable[i].FIO << endl;
            f << hashTable[i].Schet << endl << hashTable[i].sum << endl;
        }
    }
    f.close();
}
//-------------------------------------------------
template<typename T>
T fromString(const std::string& s)//перобразование строки в (int float, double,...)
{
    std::istringstream iss(s);
    T res;
    iss >> res;
    return res;
}
//---------------------------------------------------------
void load_from_file(const char* fn, Info*& hashTable, bool*& used) {//загрузка из файла
    InitHash(hashTable, used);
    fstream f;
    f.open(fn, fstream::in);

    if (!f.is_open())
    {
        cerr << "error load from file!";
        return;
    }
    Global_Coalize = 0;
    Info buf;
    string str;
    while (!f.eof()) {
        getline(f, buf.FIO);
        getline(f, str);
        buf.Schet = fromString<int>(str);
        getline(f, str);
        buf.sum = fromString<float>(str);
        AddInTable(hashTable, used, buf);
    }
    f.close();
}
//-------------------------------------------------------
void printTable(Info* hashTable, bool* used) {//вывод таблицы
    cout << setw(10) << "Key" << " " << setw(14) << "FIO" << " " << setw(6) << "Schet" << " " << setw(10) << "Suma" << endl;
    for (int i = 0; i < Size; i++) {
        if (used[i]) {
            cout << setw(10) << calcSum(hashTable[i].FIO) << " " << setw(14) << hashTable[i].FIO << " " <<
                setw(6) << hashTable[i].Schet << " " << setw(10) << hashTable[i].sum << endl;
        }
        else {
            cout << setw(10) << "NULL" << " " << setw(14) << "NULL" << endl;
        }
    }
}
//-------------------------------------------------------
bool FindInTable(Info* hashTable, bool* used, string n, int& Count) {//поиск в таблице
    int index = hashF(calcSum(n));
    int i = 0;
    Count = 0;//счетчик коализий устанавливаем в 0
    while (used[index] && hashTable[index].FIO != n) {//если ячейка занята
        index = ReHash(index, i);//вычисляем новый хеш
        i++;
        Count++;//увеличиваем счетчик коализий
    }
    return used[index] && hashTable[index].FIO == n;//возвращаем результат поиска
}
//-------------------------------------------------------
int AddInTable(Info*& hashTable, bool*& used, Info n) {//добавление в таблицу
    int index = hashF(calcSum(n.FIO));//вычисляем хеш
    int i = 0;
    int Count = 0;//счетчик коализий устанавливаем в 0
    while (i <= Size - 1 && used[index] && hashTable[index].FIO != n.FIO) {//если ячейка занята и значения не равны
        index = ReHash(index, i);//вычисляем новый хеш
        i++;
        Count++;//увеличиваем счетчик коализий
        ++Global_Coalize;//увеличиваем счетчик общих коализий
    }
    if (!used[index]) {//если нашли пустое место
        used[index] = true;//устанавливаем что там усть данные
        hashTable[index] = n;//заносим в таблицу
    }
    else {
        return -1;//нет места для добавления данных
    }
    return Count;//возврат количества коализий при добавлении
}
//-------------------------------------------------------
void InitHash(Info*& hashTable, bool*& used) {//Инициализация таблиц
    hashTable = new Info[Size];//выделяем память
    used = new bool[Size];
    Global_Coalize = 0;
    for (int i = 0; i < Size; i++) {
        used[i] = false;//устанавливаем что ячейки в таблицы все пустые
    }
}
//------------------------------------------------------
int hashF(int v) {//хеш -фнкция
    return v % Size;
}
//---------------------------------------------------------
int ReHash(int idx, int i) {//функция рехеширования в случаии коализий
    return  ((hashF(idx) + i) % Size);
}
