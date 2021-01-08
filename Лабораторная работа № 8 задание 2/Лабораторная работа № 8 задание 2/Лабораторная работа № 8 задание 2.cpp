#include <iostream>       
#include <string>         
#include <fstream>        

using namespace std;

int countItems;

struct node
{
    double data;   // Информационное поле.
    node* next;
    node* prev;     // Адресное поле. 
};
typedef node* list;

list build_list(double data)             // Формирование двунаправленного списка. Создание одного элемента.
{
    list new_list = new(node);
    new_list->data = data;
    new_list->prev = 0;
    new_list->next = 0;
    return new_list;
}

list make_list()               // Создание списка.
{
    list Head, p, tmp;
    double data;
    do
    {
        cout << "Введите количество записей в списке: ";
        cin >> countItems;
        cout << endl;

        if (countItems <= 0) cout << " Столько элементов быть не может..." << endl;
    } while (countItems <= 0);

    cin >> data;
    tmp = build_list(data);
    Head = tmp;
    for (int i = 1; i < countItems; i++)
    {
        cin >> data;
        p = build_list(data);
        p->prev = tmp;
        tmp->next = p;
        tmp = p;
    }
    return Head;
}

void Out(list u)           // Печать списка.
{
    list p = u;
    int i = 1;
    if (p == 0)
    {
        cout << " Список пустой..." << endl;
        return;
    }
    while (p != 0)
    {

        cout << "[" << i++ << "] = " << p->data << endl;
        p = p->next;
    }
}

void addNode(list& Head, int nom, double newItem)     // Добавление элемента после заданного номера

{

    if ((nom > countItems) || (nom <= 0))
    {
        cout << "Неверный номер элемента" << endl;
    }
    //текущая нода
    list current = Head, p, u;

    for (int i = 1; i < nom; i++)
    {
        current = current->next;
    }

    p = build_list(newItem);

    p->next = current->next;
    current->next = p;
}


void Delete_list(list& Head)          // Уничтожение списка.
{
    list p = Head;
    list temp;
    if (p == 0)
    {
        cout << "Список пустой...\n";

        return;
    }
    while (p != 0)
    {

        temp = p;

        p = p->prev;

        delete(temp);

    }
}


int main()
{
    setlocale(LC_ALL, "Russian");

    cout << " Записи в линейном списке содержат ключевое поле типа double." << endl;
    list Head;
    int nom;
    double newItem;

    Head = make_list();

    cout << " Исходный список:" << endl;
    Out(Head);
    cout << " Введите новый элемент: ";
    cin >> newItem;
    cout << " Введите номер элемента, после которого добавить новый элементент: ";
    cin >> nom;
    addNode(Head, nom, newItem);

    cout << " Итоговый список: " << endl;
    Out(Head); cout << endl;

    cout << " Очистка списка " << endl;
    Delete_list(Head);


    system("pause");
    return 0;
}