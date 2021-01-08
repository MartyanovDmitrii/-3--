// num_9.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma warning(disable : 4996)

#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>

using namespace std;

namespace
{
	const uint8_t MAX_LEN_NAME = 64;
	const char STATES_FILE_NAME[] = "states.txt";

	typedef struct State_t
	{
		char name[MAX_LEN_NAME];
		char cap[MAX_LEN_NAME];
		uint32_t pop;
		uint32_t area;
	}State_t;
};



int addNewState(const char* name, const char* cap, uint32_t pop, uint32_t area)
{
	if (!name || !cap)
		return -1;

	ofstream f(STATES_FILE_NAME, ios_base::app | ios_base::ate | ios_base::binary);

	if (!f.is_open()) // если файл не открыт
	{
		cout << "Файл не может быть открыт!\n";
		return -1;
	}

	State_t state;

	strncpy(state.name, name, MAX_LEN_NAME - 1);
	strncpy(state.cap, cap, MAX_LEN_NAME - 1);

	state.pop = pop;
	state.area = area;

	f.write((const char*)&state, sizeof(state));

	f.close();

	return 0;
}

void printStates(void)
{

	State_t state;

	ifstream f(STATES_FILE_NAME, ios_base::binary);

	if (!f.is_open()) // если файл не открыт
	{
		cout << "Файл не может быть открыт!\n";
		return;
	}

	int count = 1;

	while (f.read((char*)&state, sizeof(State_t)))
	{
		cout << count++ << ": Name: " << state.name << ", Capital: " << state.cap << ", pop: " << state.pop << ", area: " << state.area << endl;
	}

	f.close();
}

void deleteStates(uint32_t pop)
{
	if (rename(STATES_FILE_NAME, "tmp.txt") == 0)
	{
		ifstream fTmp("tmp.txt", ios_base::binary);

		if (!fTmp.is_open()) // если файл не открыт
		{
			cout << "Файл не может быть открыт!\n";
			return;
		}

		ofstream f(STATES_FILE_NAME, ios_base::binary);

		if (!f.is_open()) // если файл не открыт
		{
			cout << "Файл не может быть открыт!\n";
			fTmp.close();

			rename("tmp.txt", STATES_FILE_NAME);

			return;
		}

		State_t state;



		while (fTmp.read((char*)&state, sizeof(State_t)))
		{
			if (state.pop > pop)
			{
				f.write((const char*)&state, sizeof(state));
			}
		}
		fTmp.close();
		f.close();
		remove("tmp.txt");
	}
	else
		cout << "Error rename file" << endl;
}

int main()
{
	setlocale(LC_ALL, "russian");

	uint32_t pop;
	int createFlag;
	int addFlag;

	cout << "\nСоздать файл с государствами? (1 - да, 0 - нет)" << endl;

	cin >> createFlag;

	if (createFlag == 1)
	{
		remove(STATES_FILE_NAME);
		cout << "Create new file" << endl;
		addNewState("Russia", "Moscow", 12678079, 2561);
		addNewState("Germany", "Berlin", 3644826, 891);
		addNewState("France", "Paris", 2148327, 105);
		addNewState("England", "London", 8908081, 1572);
	}

	printStates();

	cout << "\nВведите заданную численность" << endl;
	cin >> pop;

	cout << "\nmin Pop " << pop << endl;

	deleteStates(pop);

	cout << "\nФайл обновлен" << endl;
	printStates();



	do
	{
		cout << "\nДобавить элемент? (1 - да, 0 - нет)" << endl;
		cin >> addFlag;

		if (addFlag != 1)
			break;

		State_t state;
		string tmpName;
		string tmpCap;

		cout << "\nВведите название государства" << endl;
		cin >> tmpName;
		strncpy(state.name, tmpName.c_str(), MAX_LEN_NAME - 1);
		cout << "Введите название столицы" << endl;
		cin >> tmpCap;
		strncpy(state.cap, tmpCap.c_str(), MAX_LEN_NAME - 1);
		cout << "Введите численность" << endl;
		cin >> state.pop;
		cout << "\n Введите площадь" << endl;
		cin >> state.area;

		addNewState(state.name, state.cap, state.pop, state.area);


	} while (1);

	cout << "\nФайл обновлен" << endl;

	printStates();

	return 0;
}
