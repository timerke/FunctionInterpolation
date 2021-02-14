/*
 * Заголовочный файл с классом для главного окна приложения.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QComboBox>
#include <QMainWindow>
#include <QTableWidget>
#include <QVector>
#include "qcustomplot.h"
#include <vector>


 // Постоянные
const QString LAGRANGE = "Полиномы Лагранжа";
const QString SPLINE = "Кубические сплайны";

/**
 * Класс для главного окна приложения.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	// Конструктор
	MainWindow(QWidget* parent = nullptr);
	// Деструктор
	~MainWindow();

private:
	const QString ICON = "icon.png"; // путь к иконке
	// Пункты меню интерполяции полиномами Лагранжа и сплайнами
	QAction* menu_lagrange;
	QAction* menu_spline;
	QComboBox* method; // выпадающий список с методами интерполяции
	QCustomPlot* plot; // область для графика функции
	QTableWidget* tbl; // таблица для сеточной функции
	std::vector<double> x; // массив координат узлов
	std::vector<double> y; // массив значений функции в узлах

	// Метод конвертирует векторы
	void convert_vector(std::vector<double>&, QVector<double>&);
	// Метод располагает виджеты на главном окне
	void create_main_wnd();
	// Метод создает меню
	void create_menu();
	// Метод собирает значения сеточной функции из таблицы
	bool get_grid_function();
	// Метод для интерполяции полиномами Лагранжа
	static void interpolate_lagrange(
		unsigned int, std::vector<double>&, std::vector<double>&,
		QVector<double>&, QVector<double>&);
	// Метод для интерполяции кубическими сплайнами
	static void interpolate_spline(
		unsigned int, std::vector<double>&, std::vector<double>&,
		QVector<double>&, QVector<double>&);
	// Метод выводит сеточную функцию в таблицу
	void show_grid_function(int, bool have_values = false);
	// Метод рисует график интерполированной функции
	void show_plot(QVector<double>&, QVector<double>&);

public slots:
	// Слот для интерполяции сеточной функции
	void interpolate();
	// Слот для чтения данных о сеточной функции из файла
	void set_grid_function_from_file();
	// Слот для задания данных о сеточной функции вручную
	void set_grid_function_manually();
    // Слот выводит информацию о приложении
    void show_info();
};

#endif // MAINWINDOW_H
