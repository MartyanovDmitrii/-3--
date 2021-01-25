#pragma warning(disable : 4996)
#include <iostream>
#include <ctime>
#include <cstring>

using namespace std;

struct point
{
    char* data;//информационное поле
    point* left;//адрес левого поддерева
    point* right;//адрес правого поддерева
};
//построение дерева поиска
point* first(char *d)//формирование первого элемента дерева
{
    point* p = new point;

    if (p)
    {
        p->data = new char[strlen(d)+1];

        if (p->data)
        {
            strcpy(p->data, d);
            p->left = 0;
            p->right = 0;
            return p;
        }
        else
            delete p;
    }
    return nullptr;
}
point* add(point* root, char *d)
{
    if (!root || !d) return nullptr;

    point* p = root;//корень дерева
    point* r = p;
    //флаг для проверки существования элемента d в дереве
    bool ok = false;
    int compare;

    while (p && !ok)
    {
        r = p;
        compare = strcmp(d, p->data);

        if (!compare)ok = true;//элемент уже существует
        else
            if (compare < 0)p = p->left;//пойти в левое поддерево
            else p = p->right;//пойти в правое поддерево
    }
    if (ok) return p;//найдено, не добавляем
    //создаем узел
    point* New_point = new point();//выделили память

    if (New_point)
    {
        New_point->data = new char[strlen(d) + 1];
        if (New_point->data)
        {
            strcpy(New_point->data, d);
            New_point->left = 0;
            New_point->right = 0;

            if (compare < 0)r->left = New_point;
            // если d>r->key, то добавляем его в правое поддерево
            else r->right = New_point;
            return New_point;

        }
        else
            delete New_point;
    }       
    return nullptr;
}
//построение

point* tree(int n, point* p)
{
    point* r;
    int nl, nr;
    if (n == 0) { p = NULL; return p; }
    nl = n / 2;
    nr = n - nl - 1;
    r = new point;
    char s[50];
    cout << "Значение: ";
    cin >> s;
    r->data = new char[strlen(s)+1];
    strcpy(r->data, s);
    r->left = tree(nl, r->left);
    r->right = tree(nr, r->right);
    p = r;
    return p;
}

void run(point* p)
//обход слева направо
{
    if (p)
    {
        p->data;
        run(p->left);//переход к левому поддереву
        run(p->right);//переход к правому поддереву
    }
}

void treeprint(point* p, int u) {

    {
        if (p == NULL) return; //Если дерево пустое, то отображать нечего, выходим
        else //Иначе
        {
            treeprint(p->left, ++u);//С помощью рекурсивного посещаем левое поддерево
            for (int i = 0; i < u; ++i) cout << "|";
            cout << p->data << endl; //И показываем элемент
            u--;
        }
        treeprint(p->right, ++u); //С помощью рекурсии посещаем правое поддерево
    }
}

void createFindThree(point* s, point* d)
{
    if ((s == nullptr) || (d == nullptr)) return;

    add(d, s->data);
    createFindThree(s->left, d);
    createFindThree(s->right, d);
}

void PrintTree(point* p, int &count)
{
    if (!p)return;

    if (p->right == NULL && p->left == NULL)
    {
        cout << "|" << p->data << endl;
        ++count;
    }

    PrintTree(p->right, count);       
    PrintTree(p->left, count);   
}

    int main()
{
    setlocale(LC_ALL, "russian");
    srand(NULL);
    int n = 0, k = 0, count = 0;
    point* beg = nullptr;
    point* findThree = nullptr;
    do
    {
        cout << "1. Посторить бинарное дерево\n";
        cout << "2. Показать бинарное дерево\n";
        cout << "3. Посчитать Количество листьев\n";
        cout << "4. Преобразовать в дерево поиска\n";
        cout << "5. Выход\n";
        cin >> k;
        switch (k)
        {
        case 1:
            cout << "Введите количество элементов" << endl;
            cin >> n;
            beg = tree(n, beg);
            cout << endl;
            break;
        case 2:
            treeprint(beg, count);
            cout << endl;
            
            break;
        case 3:
            PrintTree(beg, count);
                cout << "Листьев в дереве: " << count <<  endl;
                count = 0;
               
            break;
        case 4:
            if (beg)
            {
                if (findThree) delete findThree;
                
                findThree = first(beg->data);

                if (findThree)
                {
                    createFindThree(beg, findThree);
                }

                treeprint(findThree, count);
                cout << "Листьев в дереве: " << count << endl;
                count = 0;
            }
            else
            {
                cout << "Дерево не создано" << endl;
            }
            break;
        }
    } while (k != 5);
    system("pause");
    return 0;
}