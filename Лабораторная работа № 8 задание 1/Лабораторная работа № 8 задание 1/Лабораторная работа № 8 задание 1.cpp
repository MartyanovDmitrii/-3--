#pragma warning(disable : 4996)
#include <iostream>
#include <string.h>
#include <conio.h>

using namespace std;
//описание структуры

struct point
{
    int num;//информационное поле
    point* next;//адресное поле следующего элемента
};
point* make_point()
//создание одного элемента
{
    point* p = new(point);//выделить память
    cout << "\nВведите число: ";
    cin >> p->num;
    p->next = 0;//сформировать адресное поле 
    return p;
}
void print_point(point* p)
//печать информационных полей одного элемента списка
{
    cout << "\nЧисло: " << p->num;
}

point* make_list(int n)
//формирование списка из n элементов
{
    point* tmp = make_point();//сформировать первый элемент
    point* beg = tmp;
    point* r;
    for (int i = 1; i < n; i++)
    {
        r = make_point();//сформировать следующий элемент
        tmp->next = r;//сохраним указатель на следующий элемент
        tmp = r;//изменить адрес первого элемента списка
    }
    return beg;//вернуть адрес начала списка
}
int print_list(point* beg)
//печать списка, на который указывает указатель beg
{
    point* p = beg;//р присвоить адрес первого элемента списка
    int k = 0;//счетчик количества напечатанных элементов 
    while (p)//пока нет конца списка
    {
        print_point(p);//печать элемента, на который указывает элемент p
        p = p->next;//переход к следующему элементу
        k++;
    }
    return k;//количество элементов в списке
}

void delete_evens(point*& beg)

{
    point* p = beg;
    point* prev = NULL;

    while (p)
    {
        if (p->num % 2 == 0)
        {
            if (prev)
            {
                prev->next = p->next; break;
                p = p->next;
                continue;
            }
            else
                beg = p->next;


        }
        prev = p;
        p = p->next;
    }
}
int main()
{
    setlocale(LC_ALL, "rus");
    int n;
    cout << "\nВведите размер списка: ";
    cin >> n;
    point* beg = make_list(n);//формирование списка
    if (!print_list(beg)) cout << "\nСписок пуст";
    delete_evens(beg);
    cout << "\nИзменённый список: ";
    print_list(beg);
    _getch();//печать списка
    return 0;

}