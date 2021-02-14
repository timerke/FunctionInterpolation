/*
Заголовочный файл с объявлением класса Lagrange для интерполяции сеточной
функции полиномами Лагранжа.
*/

#pragma once
#ifndef LAGRANGE_H
#define LAGRANGE_H

#include <vector>


/**
 * Класс для интерполяции сеточной функции полиномами Лагранжа.
 */
class Lagrange
{
public:
	// Конструктор по умолчанию
	Lagrange();
	// Конструктор копирования
	Lagrange(Lagrange&);
	// Конструктор инициализации
	Lagrange(const std::vector<double>&, const std::vector<double>&);
	// Деструктор
	~Lagrange();
	// Метод вычисляет значение функции в точке
	double calculate(double);

	// Перегрузка оператора присваивания
	Lagrange& operator = (const Lagrange&);

private:
	std::vector<double> x; // массив координат узлов
	std::vector<double> y; // массив значений сеточной функции в узлах

	// Метод инициализирует сеточную функцию, для которой будет применена
	// интерполяция полиномами Лагранжа
	void init(const std::vector<double>&, const std::vector<double>&);
};

#endif // !LAGRANGE_H