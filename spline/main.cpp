/*
Алгоритм и программная реализация одномерной линейной интерполяции сеточной
функции кубическими сплайнами.
*/

#include <iostream>
#include "spline.h"


int main()
{
	const unsigned int N = 6;
	double x[N] = { 1, 2, 3, 4, 5, 6 };
	double y[N] = { 1.0002, 1.0341, 0.6, 0.40105, 0.1, 0.23975 };
	Spline s(N, x, y);
	for (unsigned int i = 0; i < N; i++)
		std::cout << "f(" << x[i] << ") = " << s.calculate(x[i]) << "\n";
}