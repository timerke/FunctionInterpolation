/*
В модуле применяется интерполяция сеточной функции полиномами Лагранжа.
*/

#include <iostream>
#include "lagrange.h"


int main()
{
	const unsigned int N = 6;
	std::vector<double> x = { 1, 2, 3, 4, 5, 6 };
	std::vector<double> y = { 1.0002, 1.0341, 0.6, 0.40105, 0.1, 0.23975 };
	Lagrange l(x, y);
	for (unsigned int i = 0; i < N; i++)
		std::cout << "f(" << x[i] << ") = " << l.calculate(x[i]) << "\n";
}