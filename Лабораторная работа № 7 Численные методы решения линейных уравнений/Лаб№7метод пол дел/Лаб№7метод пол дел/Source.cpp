#pragma warning(disable : 4996)
#include <conio.h>
#include <math.h>
#include <iostream>

using namespace std;

double f(double x) {
    double f;
    f = 3 * sin(sqrt(x)) + 0.35 * x - 3.8;
    return f;
}
int main() {

    double a = 2, b = 3, x, eps = 2.2985;
    int n;
    setlocale(LC_ALL, "Rus");
    cout << "a=" << a << endl;
    cout << "b=" << b << endl;
    cout << "eps=" << eps << endl;

    if (f(a) * f(b) < 0) {
        cout << "Условие сходимости выполнено" << endl;

        n = 0;
        while (1) {
            x = (a + b) / 2;
            n = n + 1;
            if (fabs(f(x) < eps)) break;
            if (f(a) * f(x) >= 0) {
                a = a; b = x;
            }
            else {
                a = x; b = b;
            }
        }
        cout << "Корень X=" << x << endl;
        cout << "Количество итерации n=" << n << endl;
    }
    else {
        cout << "Условие сходимости не выполнено" << endl;
    }
    system("Pause");
}