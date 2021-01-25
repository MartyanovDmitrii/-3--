// Лабораторная работа №10.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

#define MAX_NAME_LEN 64

class MenyClass
{
private:
	typedef struct DataBase_t
	{
		int groupNum;
		int studentsCount;
		int isNeedDelete;
		struct DataBase_t(int groupNum = 0, int studentsCount = 0, int isNeedDelete = 0) : groupNum(groupNum), studentsCount(studentsCount), isNeedDelete(isNeedDelete) {};
	}DataBase_t;

	typedef struct Students_t
	{
		int studentNum;
		char name[MAX_NAME_LEN];
		char surname[MAX_NAME_LEN];
	}Students_t;

	enum class State
	{
		MAIN_STATE = 0,
		OPEN_STATE,
		CREATE_STATE,
		VIEW_STATE,
		CHANGE_STATE,
		ADD_STATE,
		DELETE_STATE, 
		CREATE_FILE_ABC,
		CREATE_FILE_NUM_GROUP,
		EXIT_STATE
	};

	State state = State::MAIN_STATE;
	bool isBaseOpened = false;
	fstream fileBase;
	list <DataBase_t> baseList;
	string curBaseName;

	bool isNumGroupFree(int num)
	{
		for (auto const &item : baseList)
		{
			if (item.groupNum == num)
				return false;
		}

		return true;
	}

	void syncBase()
	{
		if (isBaseOpened)
		{
			fileBase.open(curBaseName, ios::out | ios::binary);

			if (fileBase.is_open())
			{
				for (auto const& st : baseList)
				{
					fileBase.write((const char*)&st, sizeof(DataBase_t));
				}

				fileBase.close();
			}
		}
	}

	void setState(State nextState)
	{
		state = nextState;
	}

public:
	MenyClass() {};
	~MenyClass() {};

	void mainMeny()
	{
		if (!isBaseOpened)
			cout << "База не открыта!" << endl;

		cout << "Главное меню:" << endl;
		cout << " 1. Открыть базу студентов" << endl;
		cout << " 2. Создать базу студентов" << endl;

		if (isBaseOpened)
		{
			cout << " 3. Посмотр базы студентов" << endl;
			cout << " 4. Корректировка базы студентов" << endl;
			cout << " 5. Добавление в базу студентов" << endl;
			cout << " 6. Удаление из базы студентов" << endl;
			cout << " 7. Создать отсортированный список студентов по алфавиту" << endl;
			cout << " 8. Создать отсортированный список студентов по номерам групп" << endl;
			
		}
		cout << " 9. Выход из программы" << endl;
	}

	void viewBase()
	{
		if (isBaseOpened)
		{
			if (baseList.size())
			{
				cout << "Номер группы\tКоличество студентов" << endl;

				for (auto const& item : baseList)
				{
					if (!item.isNeedDelete)
						cout << item.groupNum << "\t\t" << item.studentsCount << endl;
				}

				int printListStudents;

				cout << "Напечатать список студентов в гуппах? (0-нет, 1-напечатать определенную группу, 2-печать всех студентов групп)" << endl;
				cin >> printListStudents;

				if (printListStudents == 1)
				{ 
					int groupNum;
					cout << "Введите номер группы" << endl;
					cin >> groupNum;
					
					for (auto const& bs : baseList)
					{
						if (bs.groupNum == groupNum)
						{
							ifstream f(::to_string(groupNum), ios::binary | ios::_Nocreate);

							if (!f.is_open())
							{
								cout << "Ошибка открытия файла студентов" << endl;
								break;
							}
							else
							{
								Students_t student; 
								while (f.read((char*)&student, sizeof(student)))
								{
									cout << student.studentNum << ". Имя: " << student.name << " Фамилия: " << student.surname << endl;
								}
								f.close();
							}
						}
					}

				}
				else if(printListStudents == 2)
				{
					for (auto const& bs : baseList)
					{
						ifstream f(::to_string(bs.groupNum), ios::binary | ios::_Nocreate);

						if (!f.is_open())
						{
							cout << "Ошибка открытия файла студентов" << endl;
							continue;
						}
						else
						{
							Students_t student;
							int count = 1;
							cout << "Группа №" << bs.groupNum << endl;
							cout << " \tИмя\t" << " Фамилия"<< endl;
							while (f.read((char*)&student, sizeof(student)))
							{
								cout << " " << student.studentNum << "\t" << student.name << "\t" << student.surname << endl;
							}
							f.close();
						}
					}
				}

			}
			else
			{
				cout << "База пуста" << endl;
			}

		}else
			cout << "База не открыта" << endl;

		
		setState(State::MAIN_STATE);
	}

	void openBase()
	{
		//закроем старую базу, если открывали
		if (isBaseOpened)
		{
			isBaseOpened = false;
			fileBase.close();
		}

		if (baseList.size())
			baseList.clear();

		cout << "Введите имя файла базы студентов" << endl;
		cin >> curBaseName;
		fileBase.open(curBaseName, ios::in | ios::_Nocreate | ios::binary);

		if (!fileBase.is_open())
			cout << "Ошибка открытия файла" << endl;
		else
		{
			DataBase_t base;
			isBaseOpened = true;

			while (fileBase.read((char*)&base, sizeof(DataBase_t)))
			{
				baseList.push_back(base);
			}

			cout << "База успешно открыта! Напечатать содержимое? (0-нет, 1-да)" << endl;
			int printBase;
			cin >> printBase;

			if (printBase == 1)
			{
				viewBase();
			}
		}

		fileBase.close();

		setState(State::MAIN_STATE);
	}

	void createBase()
	{
		if (isBaseOpened)
		{
			isBaseOpened = false;
		}

		if (baseList.size())
			baseList.clear();

		cout << "Введите имя файла базы студентов" << endl;
		cin >> curBaseName;

		fileBase.open(curBaseName, ios::out | ios::_Noreplace | ios::binary);

		if (!fileBase.is_open())
		{
			int retryCreate;

			cout << "Ошибка создания файла (возможно файл уже существует)" << endl;
			cout << "Попробывать создать снова? (0-нет, 1-да)" << endl;
			cin >> retryCreate;

			if (retryCreate == 1)
				setState(State::CREATE_STATE);
			else
				setState(State::MAIN_STATE);

			return;
		}

		fileBase.close(); //закроем созданный файл

		isBaseOpened = true;

		int addGroup;
		cout << "База успешно создана! Добавить группу студентов? (0-нет, 1-да)" << endl;
		cin >> addGroup;
		if (addGroup == 1)
		{
			int numGroup, retry = 0;

			do
			{
				cout << "Введите номер группы" << endl;
				cin >> numGroup;

				if (isNumGroupFree(numGroup))
				{
					int addStudent;
					DataBase_t item = { 0 };
					list <Students_t > students;
					item.groupNum = numGroup;						
					int count = 1;

					do
					{						
						cout << "Добавить студента? (0-нет, 1-да)" << endl;
						cin >> addStudent;
						if (addStudent == 1)
						{
							string name;
							string surname;
							Students_t student = { 0 };

							cout << "Введите имя студента" << endl;
							cin >> name;
							cout << "Введите фамилию студента" << endl;
							cin >> surname;
								
							strncpy_s(student.name, MAX_NAME_LEN - 1, name.c_str(), MAX_NAME_LEN - 1);
							strncpy_s(student.surname, MAX_NAME_LEN - 1, surname.c_str(), MAX_NAME_LEN - 1);
							student.studentNum = count++;
							students.push_back(student);
						}
						else
							break;
					} while (1);
	
					ofstream fileStudents(::to_string(numGroup), ios::binary);

					if (!fileStudents.is_open())
					{
						cout << "Ошибка создания файла студентов" << endl;

						setState(State::MAIN_STATE);
						return;
					}

					for (auto const& st : students)
					{
						fileStudents.write((const char*)&st, sizeof(Students_t));
					}

					fileStudents.close();

					baseList.push_back(DataBase_t(numGroup, students.size(), 0));
					syncBase();

					int addMoreGroup;
					cout << "Добавить еще группу? (0-нет, 1-да)" << endl;
					cin >> addMoreGroup;

					if (addMoreGroup == 1)
					{
						retry = 1;
						continue;
					}
					setState(State::MAIN_STATE);

					return;
				}
				else
				{
					cout << "Такой номер группы уже существует. Попробывать снова? (0-нет, 1-да)" << endl;
					cin >> retry;
				}

			} while (retry == 1);

			setState(State::CREATE_STATE);
			return;
		}

		setState(State::MAIN_STATE);
	}

	void changeBase()
	{
		int groupNum;
		cout << "Введите номер группы, в которой нужно ввести изменения" << endl;
		cin >> groupNum;

		for (auto const& item : baseList)
		{
			if (groupNum == item.groupNum)
			{
				int stNum;
				cout << "Введите номер студента, в котором внести изменения" << endl;
				cin >> stNum;
				ifstream f(::to_string(item.groupNum), ios::binary | ios::_Nocreate);

				if(!f.is_open())
				{
					cout << "Ошибка открытия файла студентов " << ::to_string(item.groupNum) << endl;
					setState(State::MAIN_STATE);
					return;
				}
				list <Students_t> students;
				Students_t studentTmp;

				while (f.read((char*)&studentTmp, sizeof(studentTmp)))
				{
					students.push_back(studentTmp);
				}

				f.close();

				if (!students.size())
				{
					cout << "Список студентов пуст" << endl;
					int addSt;		
					int count = 1;
					do
					{
						cout << "Добавить студента? (0-нет, 1-да)" << endl;
						cin >> addSt;

						if (addSt == 1)
						{
							string name;
							string surname;
							Students_t student = { 0 };

							cout << "Введите имя студента" << endl;
							cin >> name;
							cout << "Введите фамилию студента" << endl;
							cin >> surname;

							strncpy_s(student.name, MAX_NAME_LEN - 1, name.c_str(), MAX_NAME_LEN - 1);
							strncpy_s(student.surname, MAX_NAME_LEN - 1, surname.c_str(), MAX_NAME_LEN - 1);
							student.studentNum = count++;
							students.push_back(student);
						}
						else
							break;

					} while (1);

					ofstream fileStudents(::to_string(groupNum), ios::binary);

					if (!fileStudents.is_open())
					{
						cout << "Ошибка создания файла студентов" << endl;

						setState(State::MAIN_STATE);
						return;
					}

					for (auto const& st : students)
					{
						fileStudents.write((const char*)&st, sizeof(Students_t));
					}

					fileStudents.close();

					baseList.push_back(DataBase_t(groupNum, students.size(), 0));
					syncBase();

					setState(State::MAIN_STATE);
					return;
				}
				else
				{
					bool isChange = false;
					for (auto &st : students)
					{
						if (st.studentNum == stNum)
						{
							int changeItem;
							cout << "Внести изменения (0 - в имени, 1 - в фамилии)" << endl;
							cin >> changeItem;

							if (changeItem == 1)//меняем фамилию
							{
								string newSurname;
								cout << "Введите новую фамилию" << endl;
								cin >> newSurname;
								strncpy_s(st.surname, MAX_NAME_LEN - 1, newSurname.c_str(), MAX_NAME_LEN - 1);
							}
							else
							{
								string name;
								cout << "Введите новое имя" << endl;
								cin >> name;
								strncpy_s(st.name, MAX_NAME_LEN - 1, name.c_str(), MAX_NAME_LEN - 1);
							}

							cout << "Изменения применены!" << endl;
							isChange = true;
						}
					}

					if (isChange)
					{
						ofstream fw(::to_string(item.groupNum), ios::binary);

						if (!fw.is_open())
						{
							cout << "Ошибка изменения файла студентов " << ::to_string(item.groupNum) << endl;
							setState(State::MAIN_STATE);
							return;
						}

						for (auto const& st : students)
						{
							cout << "Write change: " << st.name << " " << st.surname << endl;
							fw.write((char*)&st, sizeof(Students_t));
						}
						fw.close();
					}
					else
					{
						cout << "Неккоректный номер студента" << endl;
						
					}
					setState(State::MAIN_STATE);
					return;
				}

			}
		}
		cout << "Введен некорректный номер" << endl;
		setState(State::MAIN_STATE);
	}

	void addBase()
	{
		int addNewGroup;
		cout << "Добавить новую группу? (0-нет, 1-да)" << endl;
		cin >> addNewGroup;

		if (addNewGroup == 1)
		{
			int numGroup, retry = 0;

			do
			{
				cout << "Введите номер группы" << endl;
				cin >> numGroup;

				if (isNumGroupFree(numGroup))
				{
					int addStudent;
					DataBase_t item = { 0 };
					list <Students_t > students;
					item.groupNum = numGroup;
					int count = 1;

					do
					{
						cout << "Добавить студента? (0-нет, 1-да)" << endl;
						cin >> addStudent;
						if (addStudent == 1)
						{
							string name;
							string surname;
							Students_t student = { 0 };

							cout << "Введите имя студента" << endl;
							cin >> name;
							cout << "Введите фамилию студента" << endl;
							cin >> surname;

							strncpy_s(student.name, MAX_NAME_LEN - 1, name.c_str(), MAX_NAME_LEN - 1);
							strncpy_s(student.surname, MAX_NAME_LEN - 1, surname.c_str(), MAX_NAME_LEN - 1);
							student.studentNum = count++;
							students.push_back(student);
						}
						else
							break;
					} while (1);

					ofstream fileStudents(::to_string(numGroup), ios::binary);

					if (!fileStudents.is_open())
					{
						cout << "Ошибка создания файла студентов" << endl;

						setState(State::MAIN_STATE);
						return;
					}

					for (auto const& st : students)
					{
						fileStudents.write((const char*)&st, sizeof(Students_t));
					}

					fileStudents.close();

					baseList.push_back(DataBase_t(numGroup, students.size(), 0));
					syncBase();

					int addMoreGroup;
					cout << "Добавить еще группу? (0-нет, 1-да)" << endl;
					cin >> addMoreGroup;

					if (addMoreGroup == 1)
					{
						retry = 1;
						continue;
					}
					setState(State::MAIN_STATE);

					return;
				}
				else
				{
					cout << "Такой номер группы уже существует. Попробывать снова? (0-нет, 1-да)" << endl;
					cin >> retry;
				}

			} while (retry == 1);
		}
		else
		{
			int addNewStudent;
			cout << "Добавить нового студента в группу? (0-нет, 1-да)" << endl;
			cin >> addNewStudent;

			if (addNewStudent == 1)
			{
				int numGroup;
				cout << "Введите номер группы, в которую добавить студента" << endl;
				cin >> numGroup;

				if (!isNumGroupFree(numGroup))
				{
					ifstream f(::to_string(numGroup), ios::binary | ios::_Nocreate);

					if (!f.is_open())
					{
						cout << "Ошибка открытия файла студентов" << endl;
					}
					else
					{
						list <Students_t> students;
						Students_t studentTmp;

						while (f.read((char*)&studentTmp, sizeof(studentTmp)))
						{
							students.push_back(studentTmp);
						}
						f.close();

						string name;
						string surname;
						int addNextSt;

						do
						{
							cout << "Введите имя студента" << endl;
							cin >> name;
							cout << "Введите фамилию студента" << endl;
							cin >> surname;

							strncpy_s(studentTmp.name, MAX_NAME_LEN - 1, name.c_str(), MAX_NAME_LEN - 1);
							strncpy_s(studentTmp.surname, MAX_NAME_LEN - 1, surname.c_str(), MAX_NAME_LEN - 1);
							studentTmp.studentNum = students.size() + 1;
							students.push_back(studentTmp);

							cout << "Добавить еще? (0-нет, 1-да)" << endl;
							cin >> addNextSt;

						} while (addNextSt == 1);

						ofstream fw(::to_string(numGroup), ios::binary);

						if (!fw.is_open())
						{
							cout << "Ошибка изменения файла студентов " << ::to_string(numGroup) << endl;
							setState(State::MAIN_STATE);
							return;
						}

						for (auto const& st : students)
						{
							cout << "Write change: " << st.name << " " << st.surname << endl;
							fw.write((char*)&st, sizeof(Students_t));
						}
						fw.close();
					}
				}
				else
				{
					cout << "Такого номера группы не существует" << endl;
				}
			}
		}
		
		setState(State::MAIN_STATE);
	}

	void delBase()
	{
		int delItem;
		cout << "Что удалить? (0-группу, 1-студента из группа)" << endl;
		cin >> delItem;

		if (delItem == 1)
		{
			int numGroup, retry = 0;

			do
			{
				cout << "Введите номер группы" << endl;
				cin >> numGroup;

				if (!isNumGroupFree(numGroup))
				{
					int stNum;
					cout << "Введите номер студента, которого нужно удалить" << endl;
					cin >> stNum;
					ifstream f(::to_string(numGroup), ios::binary | ios::_Nocreate);

					if (!f.is_open())
					{
						cout << "Ошибка открытия файла студентов " << ::to_string(numGroup) << endl;
						setState(State::MAIN_STATE);
						return;
					}
					list <Students_t> students;
					Students_t studentTmp;

					while (f.read((char*)&studentTmp, sizeof(studentTmp)))
					{
						students.push_back(studentTmp);
					}

					f.close();

					if (!students.size())
					{
						cout << "Список студентов пуст" << endl;
						setState(State::MAIN_STATE);
						return;
					}
					else
					{
						bool isChange = false;
						for (auto  st = students.begin(); st != students.end(); st++)
						{
							if (st->studentNum == stNum)
							{
								students.erase(st);
								isChange = true;
								break;
							}
						}

						if (isChange)
						{
							ofstream fw(::to_string(numGroup), ios::binary);

							if (!fw.is_open())
							{
								cout << "Ошибка изменения файла студентов " << ::to_string(numGroup) << endl;
								setState(State::MAIN_STATE);
								return;
							}

							for (auto const& st : students)
							{
								cout << "Write change: " << st.name << " " << st.surname << endl;
								fw.write((char*)&st, sizeof(Students_t));
							}
							fw.close();
						}
						else
						{
							cout << "Неккоректный номер студента" << endl;

						}
						setState(State::MAIN_STATE);
						return;
					}
				}
				else
				{
					cout << "Такой номер группы не существует. Попробывать снова? (0-нет, 1-да)" << endl;
					cin >> retry;
				}

			} while (retry == 1);
		}
		else
		{
			int numGroup;
			cout << "Введите номер группы для удаления" << endl;
			cin >> numGroup;

			if (!isNumGroupFree(numGroup))
			{
				for (auto gr = baseList.begin(); gr != baseList.end(); gr++)
				{
					if (gr->groupNum == numGroup)
					{
						baseList.erase(gr);
						syncBase();
						break;
					}
				}
			}
			else
			{
				cout << "Такой номер группы не существует" << endl;
			}
		}
		setState(State::MAIN_STATE);
	}

	void sortABC()
	{
		if (isBaseOpened)
		{
			list <string> sortStudents;

			for (auto const& bs : baseList)
			{
				ifstream f(::to_string(bs.groupNum), ios::binary | ios::_Nocreate);

				if (!f.is_open())
				{
					cout << "Ошибка открытия файла студентов" << endl;
					continue;
				}
				else
				{
					Students_t student;

					//отсортируем по алфавиту

					while (f.read((char*)&student, sizeof(student)))
					{
						string fullName = string(student.surname) + string(" ") + string(student.name);

						if (sortStudents.empty())
							sortStudents.push_back(fullName);
						else
						{
							bool isInserted = false;
							for (auto item = sortStudents.begin(); item != sortStudents.end(); item++)
							{
								if (fullName < *item)//если меньше, то нужно положить до
								{
									sortStudents.insert(item, fullName); //вставим, остальные сдвинутся
									isInserted = true;
									break;
								}
							}

							if (!isInserted) //если не было вставки, добавим в конец
								sortStudents.push_back(fullName);
						}
					}
					f.close();
				}
			}

			cout << "Список студентов по алфавиту" << endl;

			ofstream fw("SortedABC.txt");

			int count = 1;

			for (auto const& item : sortStudents)
			{
				cout << item << endl;

				if (fw.is_open())
				{
					fw << count++;
					fw << ". ";
					fw << item;
					fw << "\n";
				}
			}

			if (fw.is_open())
			{
				cout << "Осортированный список студентов сохранен в файле SortedABC.txt" << endl;
				fw.close();
			}
		}

		setState(State::MAIN_STATE);
	}

	void sortNumGroup()
	{
		if (isBaseOpened)
		{
			list <string> sortStudents;
			list <int> sortNumGroup;

			//получим осортированный список номеров групп

			bool isInserted = false;

			for (auto const &item: baseList)
			{
				if (sortNumGroup.empty())
					sortNumGroup.push_back(item.groupNum);
				else
				{
					bool isInserted = false;
					for (auto it = sortNumGroup.begin(); it != sortNumGroup.end(); it++)
					{
						if (item.groupNum < *it)//если меньше, то нужно положить до
						{
							sortNumGroup.insert(it, item.groupNum); //вставим, остальные сдвинутся
							isInserted = true;
							break;
						}
					}

					if (!isInserted) //если не было вставки, добавим в конец
						sortNumGroup.push_back(item.groupNum);
				}
			}

			for (auto const& it : sortNumGroup)
			{
				ifstream f(::to_string(it), ios::binary | ios::_Nocreate);

				if (!f.is_open())
				{
					cout << "Ошибка открытия файла студентов" << endl;
					continue;
				}
				else
				{
					string strNumGroup = string("Group ") + ::to_string(it);
					sortStudents.push_back(strNumGroup);

					Students_t student;
					
					while (f.read((char*)&student, sizeof(student)))
					{
						string fullName = string(" ") + string(student.surname) + string(" ") + string(student.name);
						sortStudents.push_back(fullName);						
					}
					f.close();
				}
			}

			cout << "Список студентов по убыванию номеров групп" << endl;

			ofstream fw("SortedNumGroup.txt");


			for (auto const& item : sortStudents)
			{
				cout << item << endl;

				if (fw.is_open())
				{
					fw << item;
					fw << "\n";
				}
			}

			if (fw.is_open())
			{
				cout << "Осортированный список студентов сохранен в файле SortedNumGroup.txt" << endl;
				fw.close();
			}
		}

		setState(State::MAIN_STATE);
	}


	int stateMachine()
	{
		int nexState = static_cast<int>(State::MAIN_STATE);

		switch (state)
		{
			case State::MAIN_STATE:
				mainMeny();	

				do
				{
					cin >> nexState;
				} while (!nexState);

				setState(static_cast<State>(nexState));
				break;
			case State::OPEN_STATE:
				openBase();
				break;
			case State::CREATE_STATE:
				createBase();
				break;
			case State::VIEW_STATE:
				viewBase();
				break;
			case State::CHANGE_STATE:
				changeBase();
				break;
			case State::ADD_STATE:
				addBase();
				break;
			case State::DELETE_STATE:
				delBase();
				break;
			case State::CREATE_FILE_ABC:
				sortABC();
				break;
			case State::CREATE_FILE_NUM_GROUP:
				sortNumGroup();
				break;
			case State::EXIT_STATE:
				return 1;
				break;
			default: 
				cout << "Prev state: " << static_cast<int>(state) << endl;
				state = State::MAIN_STATE;
				break;
		}	

		return 0;
	}
};


int main()
{
	setlocale(LC_ALL, "russian");

	MenyClass *meny = new MenyClass;

	if (meny)
	{
		while (!meny->stateMachine());

		delete meny;
	}

	return 0;
}

