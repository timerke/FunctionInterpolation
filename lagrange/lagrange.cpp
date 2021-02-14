/*
Модуль содержит определение методов класса Lagrange.
*/

#include "lagrange.h"


/**
 * Конструктор по умолчанию.
 */
Lagrange::Lagrange() {}

/**
 * Конструктор копирования.
 * @param l: копируемый объект.
 */
Lagrange::Lagrange(Lagrange& l)
{
	// Для интерполяции полиномами Лагранжа необходимо как минимум 2 узла
	if (l.x.size() < 2)
		return;
	// Инициализируем сеточную функцию
	init(l.x, l.y);
}

/**
 * Конструктор инициализации.
 * @param x: массив координат узлов сеточной функции;
 * @param y: массив значений сеточной функции.
 */
Lagrange::Lagrange(const std::vector<double>& x, const std::vector<double>& y)
{
	// Для интерполяции кубическими сплайнами необходимо как минимум 2 узла
	if (x.size() < 2)
		return;
	// Инициализируем сеточную функцию
	init(x, y);
}

/**
 * Деструктор.
 */
Lagrange::~Lagrange() {}

/**
 * Метод вычисляет значение функции в точке.
 * @param x: координата точки.
 * @return: значение интерполированной функции.
 */
double Lagrange::calculate(double x)
{
	double y = 0;
	for (unsigned int i = 0; i < this->x.size(); i++)
	{
		double l_i = 1;
		for (unsigned int j = 0; j < this->x.size(); j++)
		{
			if (i == j)
				continue;
			l_i *= (x - this->x[j]) / (this->x[i] - this->x[j]);
		}
		y += l_i * this->y[i];
	}
	return y;
}

/**
 * Метод инициализирует сеточную функцию, для которой будет применена
 * интерполяция полиномами Лагранжа.
 * @param x: массив координат узлов сеточной функции;
 * @param y: массив значений сеточной функции.
 */
void Lagrange::init(const std::vector<double>& x, const std::vector<double>& y)
{
	// Очищаем массивы
	this->x.clear();
	this->y.clear();
	// Записываются координаты узлов и значения сеточной функции в узлах
	this->x = x;
	this->y = y;
}

/**
 * Перегрузка оператора присваивания.
 */
Lagrange& Lagrange::operator = (const Lagrange& l)
{
	// Проверка на самоприсваивание
	if (this == &l)
		return *this;

	// Инициализируем сеточную функцию
	init(l.x, l.y);
	return *this;
}