/*
Модуль содержит определение методов класса Spline.
*/

#include <iostream>
#include <stdarg.h>
#include "spline.h"


/**
 * Конструктор по умолчанию.
 */
Spline::Spline() {}

/**
 * Конструктор копирования.
 * @param s: копируемый объект.
 */
Spline::Spline(Spline& s)
{
	// Для интерполяции кубическими сплайнами необходимо как минимум 2 узла
	if (s.n < 2)
		return;
	// Инициализируем сеточную функцию
	init(s.n, s.x, s.y);
	// Инициализируем кубические сплайны
	init_spline(s.a, s.b, s.c, s.d);
}

/**
 * Конструктор инициализации.
 * @param n: количество узлов, в которых определена сеточная функция;
 * @param x: массив координат узлов сеточной функции;
 * @param y: массив значений сеточной функции.
 */
Spline::Spline(unsigned int n, double* x, double* y)
{
	// Для интерполяции кубическими сплайнами необходимо как минимум 2 узла
	if (n < 2)
		return;
	// Инициализируем сеточную функцию
	init(n, x, y);
	// Вычисляем коэффициенты кубических сплайнов
	init_spline();
}

/**
 * Конструктор инициализации.
 * @param x: массив координат узлов сеточной функции;
 * @param y: массив значений сеточной функции.
 */
Spline::Spline(std::vector<double>& x, std::vector<double>& y)
{
	// Для интерполяции кубическими сплайнами необходимо как минимум 2 узла
	if (x.size() < 2)
		return;
	// Инициализируем сеточную функцию
	init(x, y);
	// Вычисляем коэффициенты кубических сплайнов
	init_spline();
}

/**
 * Деструктор.
 */
Spline::~Spline()
{
	// Очищаем память, выделенную на динамические массивы со значениями
	// координат узлов, сеточной функции, коэффициентами кубических сплайнов
	delete_arrays(6, &x, &y, &a, &b, &c, &d);
}

/**
 * Метод вычисляет значение функции в точке.
 * @param x: координата точки.
 * @return: значение интерполированной функции.
 */
double Spline::calculate(double x)
{
	if (n < 2)
		return 0;
	// Определяем индекс наименьшего из двух узлов, между которыми попадает
	// координата x
	unsigned int i = find_index(x);
	// Вычисляем значение интерполяции
	double dx = x - this->x[i];
	return a[i + 1] + dx * (b[i + 1] + dx * (c[i + 1] + dx * d[i + 1]));
}

/**
 * Метод удаляет динамические массивы.
 * @param n: количество удаляемых динамических массивов.
 */
void Spline::delete_arrays(unsigned int n, ...)
{
	va_list factor; // указатель на необязательный параметр
	va_start(factor, n); // устанавливаем указатель
	for (unsigned int i = 0; i < n; i++)
	{
		double** array = va_arg(factor, double**);
		if (*array != nullptr)
			delete[] * array;
	}
	va_end(factor);
}

/**
 * Метод находит индекс наименьшего из двух узлов, между которыми попадает
 * координата точки.
 * @param x: координата точки.
 * @return: индекс наименьшего из двух соседних узлов, между которыми попадает
 * точка.
 */
unsigned int Spline::find_index(double x)
{
	if (x <= this->x[0])
		return 0;
	if (x >= this->x[n - 1])
		return n - 2;
	for (unsigned int i = 0; i < n - 1; i++)
	{
		if (this->x[i] <= x && x <= this->x[i + 1])
			return i;
	}
	return 0;
}

/**
 * Метод инициализирует сеточную функцию, для которой будет применена
 * интерполяция сплайнами.
 * @param n: количество узлов, в которых определена сеточная функция;
 * @param x: массив координат узлов сеточной функции;
 * @param y: массив значений сеточной функции.
 */
void Spline::init(unsigned int n, double* x, double* y)
{
	// Удаляем память, выделенную на динамические массивы
	delete_arrays(2, &this->x, &this->y);
	// Записываются координаты узлов и значения сеточной функции в узлах
	this->n = n;
	this->x = new double[this->n];
	this->y = new double[this->n];
	for (unsigned int i = 0; i < n; i++)
	{
		this->x[i] = x[i];
		this->y[i] = y[i];
	}
}

/**
 * Метод инициализирует сеточную функцию, для которой будет применена
 * интерполяция сплайнами.
 * @param x: массив координат узлов сеточной функции;
 * @param y: массив значений сеточной функции.
 */
void Spline::init(std::vector<double>& x, std::vector<double>& y)
{
	// Удаляем память, выделенную на динамические массивы
	delete_arrays(2, &this->x, &this->y);
	// Записываются координаты узлов и значения сеточной функции в узлах
	this->n = x.size();
	this->x = new double[this->n];
	this->y = new double[this->n];
	for (unsigned int i = 0; i < n; i++)
	{
		this->x[i] = x[i];
		this->y[i] = y[i];
	}
}

/**
 * Метод вычисляет коэффициенты для интерполяции сплайнами.
 */
void Spline::init_spline()
{
	// Прямой ход для вычисления коэффициентов eta, xi
	double* eta = new double[n + 1];
	double* xi = new double[n + 1];
	run_straight(&eta, &xi);
	// Выделяем память на массив с коэффициентами кубических сплайнов
	a = new double[n];
	b = new double[n];
	c = new double[n];
	d = new double[n];
	// Обратный ход для вычисления коэффициентов кубических сплайнов
	run_reverse(eta, xi);
	// Удаляем выделенную для eta и xi память
	delete_arrays(2, &eta, &xi);
}

// Метод инициализирует коэффициенты для интерполяции сплайнами
void Spline::init_spline(double* a, double* b, double* c, double* d)
{
	// Удаляем память, выделенную на динамические массивы
	delete_arrays(4, &this->a, &this->b, &this->c, &this->d);
	// Записываются коэффициенты кубических сплайнов
	this->a = new double[n];
	this->b = new double[n];
	this->c = new double[n];
	this->d = new double[n];
	for (unsigned int i = 0; i < n; i++)
	{
		this->a[i] = a[i];
		this->b[i] = b[i];
		this->c[i] = c[i];
		this->d[i] = d[i];
	}
}

/**
 * Метод вычисляет в обратном ходе коэффициенты кубических сплайнов.
 * @param eta, xi: массивы для коэффициентов eta, xi.
 */
void Spline::run_reverse(double* eta, double* xi)
{
	double h = x[n - 1] - x[n - 2];
	a[n - 1] = y[n - 2];
	c[n - 1] = eta[n];
	b[n - 1] = (y[n - 1] - y[n - 2]) / h - 2 * h * c[n - 1] / 3;
	d[n - 1] = -c[n - 1] / 3 / h;
	for (unsigned int i = n - 2; i > 0; i--)
	{
		h = x[i] - x[i - 1];
		a[i] = y[i - 1];
		c[i] = xi[i + 1] * c[i + 1] + eta[i + 1];
		b[i] = (y[i] - y[i - 1]) / h - h * (c[i + 1] + 2 * c[i]) / 3;
		d[i] = (c[i + 1] - c[i]) / 3 / h;
	}
}

/**
 * Метод вычисляет в прямом ходе коэффициенты eta, xi.
 * @param eta, xi: массивы для коэффициентов eta, xi.
 */
void Spline::run_straight(double** eta, double** xi)
{
	(*eta)[2] = 0;
	(*xi)[2] = 0;
	for (unsigned int i = 2; i < n; i++)
	{
		double f = 3 * ((y[i] - y[i - 1]) / (x[i] - x[i - 1]) -
			(y[i - 1] - y[i - 2]) / (x[i - 1] - x[i - 2]));
		double d = (x[i - 1] - x[i - 2]) * (*xi)[i] + 2 * (x[i] - x[i - 2]);
		(*eta)[i + 1] = (f - (x[i - 1] - x[i - 2]) * (*eta)[i]) / d;
		(*xi)[i + 1] = (x[i - 1] - x[i]) / d;
	}
}

/**
 * Перегрузка оператора присваивания.
 */
Spline& Spline::operator = (const Spline& s)
{
	// Проверка на самоприсваивание
	if (this == &s)
		return *this;

	// Удаляем память, выделенную на динамические массивы
	delete_arrays(6, &this->x, &this->y, &this->a, &this->b, &this->c, &this->d);
	// Инициализируем сеточную функцию
	init(s.n, s.x, s.y);
	// Инициализируем кубические сплайны
	init_spline(s.a, s.b, s.c, s.d);
	return *this;
}