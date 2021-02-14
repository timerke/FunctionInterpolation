/*
Заголовочный файл содержит объявление класса Spline для осуществления
интерполяции одномерной сеточной функции кубическими сплайнами.
*/

#pragma once
#ifndef SPLINE_H
#define SPLINE_H

#include <vector>


/**
 * Класс для интерполяции сеточной функции кубическими сплайнами.
 */
class Spline
{
public:
	// Конструктор по умолчанию
	Spline();
	// Конструктор копирования
	Spline(Spline&);
	// Конструктор инициализации
	Spline(unsigned int, double*, double*);
	// Конструктор инициализации
	Spline(std::vector<double>&, std::vector<double>&);
	// Деструктор
	~Spline();
	// Метод вычисляет значение функции в точке
	double calculate(double);

	// Перегрузка оператора присваивания
	Spline& operator = (const Spline&);

private:
	unsigned int n = 0; // количество узлов сеточной функции
	double* x = nullptr; // массив координат узлов
	double* y = nullptr; // массив значений сеточной функции в узлах

	// Коэффициенты интерполяции кубическими сплайнами
	double* a = nullptr;
	double* b = nullptr;
	double* c = nullptr;
	double* d = nullptr;

	// Метод удаляет динамические массивы
	void delete_arrays(unsigned int, ...);
	// Метод находит индекс наименьшего из двух узлов, между которыми попадает
	// координата точки
	unsigned int find_index(double);
	// Метод инициализирует сеточную функцию, для которой будет применена
	// интерполяция сплайнами
	void init(unsigned int, double*, double*);
	// Метод инициализирует сеточную функцию, для которой будет применена
	// интерполяция сплайнами
	void init(std::vector<double>&, std::vector<double>&);
	// Метод вычисляет коэффициенты для интерполяции сплайнами
	void init_spline();
	// Метод инициализирует коэффициенты для интерполяции сплайнами
	void init_spline(double*, double*, double*, double*);
	// Метод вычисляет в обратном ходе коэффициенты c кубических сплайнов
	void run_reverse(double*, double*);
	// Метод вычисляет в прямом ходе коэффициенты eta, xi
	void run_straight(double**, double**);
};

#endif // !SPLINE_H