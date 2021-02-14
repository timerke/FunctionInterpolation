/*
 * Заголовочный файл с прототипами полезных функций.
 */

#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>


// Функция разбиения массива для быстрой сортировки
int partition(std::vector<double>&, std::vector<double>&, int, int);

// Функция сортирует два вектора по возрастанию элементов одного из
// векторов
void sort_quick(std::vector<double>&, std::vector<double>&, int, int);

// Функция меняет местами два элемента массива
void swap(std::vector<double>&, int, int);

#endif // FUNCTIONS_H
