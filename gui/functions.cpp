/*
 * Модуль содержит определение полезных функций.
 */

#include "functions.h"


/**
 * Функция разбиения массива для быстрой сортировки.
 * @param a: массив, который должен быть отсортирован по возрастанию;
 * @param b: массив, который будет отсортирован за компанию;
 * @param left, right: индексы, элементы между которыми нужно отсортировать.
 */
int partition(std::vector<double>& a, std::vector<double>& b, int left,
	int right)
{
	// Выбираем средний элемент в качестве опорного
	double pivot = a[(left + right) / 2];
	int i = left - 1;
	int j = right + 1;
	while (true)
	{
		i++;
		while (a[i] < pivot)
			i++;
		j--;
		while (a[j] > pivot)
			j--;
		if (i >= j)
			return j;
		// Если элемент с индексом i (слева от опорного) больше, чем элемент с
		// индексом j (справа от опорного), меняем их местами
		swap(a, i, j);
		swap(b, i, j);
	}
}

/**
 * Функция сортирует два вектора по возрастанию элементов одного из векторов.
 * @param a: массив, который должен быть отсортирован по возрастанию;
 * @param b: массив, который будет отсортирован за компанию;
 * @param left, right: индексы, элементы между которыми нужно отсортировать.
 */
void sort_quick(std::vector<double>& a, std::vector<double>& b, int left,
	int right)
{
	if (left < right)
	{
		int split_index = partition(a, b, left, right);
		sort_quick(a, b, left, split_index);
		sort_quick(a, b, split_index + 1, right);
	}
}

/**
 * Функция меняет местами два элемента массива.
 * @param a: массив;
 * @param i, j: индексы элементов, которые нужно поменять местами.
 */
void swap(std::vector<double>& a, int i, int j)
{
	double tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}
