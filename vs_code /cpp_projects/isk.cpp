#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    double a = 0.0;
    double b = 0.0;
    std::cin >> a >> b;
    std::cout << std::fixed << std::setprecision(2) << a / b << '\n';
}