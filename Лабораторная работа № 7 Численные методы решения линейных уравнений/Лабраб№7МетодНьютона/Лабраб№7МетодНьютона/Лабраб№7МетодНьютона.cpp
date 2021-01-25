#include <conio.h>
#include <cmath>
#include <iostream>

using namespace std;

double f(double x) {
    double f;
    f = 3 * sin(sqrt(x)) + 0.35 * x - 3.8;
    return f;
}

double f1(double x) {
    double f1;
    f1 = 0.35 + 3 * cos(sqrt(x)) / 2 * sqrt(x);
    return f1;

}

double f2(double x) {
    double f2;
    f2 = -3 * (sin(sqrt(x)) / x + cos(sqrt(x)) / pow(x, 3 / 2)) / 4;

    return f2;
}

int main() {
    setlocale(LC_ALL, "rus");
    int n;
    double a, b, x, eps;
    a = 2; b = 3; eps = 2.2985;
    cout << "a=" << a << endl;
    cout << "b=" << b << endl;
    cout << "eps=" << eps << endl;
    if (f(a) * f2(a) > 0) {
        cout << "Условие сходимости выполнено для x=a=" << a << endl;
        x = a;
    }
    else {
        if (f(b) * f2(b) > 0) {
            cout << "Условие сходимости выполнено для x=b=" << b << endl;
            x = b;
        }
        else {
            cout << "Условие сходимости не выполнено" << endl;
            x = -10E10;
        }
    }
    if (x > -10E10)
    {
        n = 0;
        while (1) {
            x = x - f(x) / f1(x);
            n += 1;
            if (fabs(f(x)) <= eps)break;
        }
        cout << "корень x=" << x << "\n";
        cout << "Количесво итерации n=" << n << "\n";

    }

    _getch();
    return 0;
}
