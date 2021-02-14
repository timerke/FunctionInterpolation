/*
 * Модуль содержит определения методов класса главного окна.
 */

#include <QApplication>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTextStream>
#include <QValidator>
#include <QVBoxLayout>
#include "functions.h"
#include "mainwindow.h"
#include "../lagrange/lagrange.h"
#include "../spline/spline.h"


/**
 * Конструктор.
 */
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	// Задаем иконку и название главного окна
	setWindowIcon(QIcon(ICON));
	setWindowTitle("Интерполяция");
	// Создаем меню
	create_menu();
	// Располагаем виджеты на главном окне
	create_main_wnd();
}

/**
 * Деструктор.
 */
MainWindow::~MainWindow() {}

/**
 * Метод конвертирует векторы.
 * @param x: вектор с существующими данными;
 * @param x_new: новый вектор, в который будут записаны данные.
 */
void MainWindow::convert_vector(std::vector<double>& x, QVector<double>& x_new)
{
	for (auto i : x)
		x_new.push_back(i);
}

/**
 * Метод располагает виджеты на главном окне.
 */
void MainWindow::create_main_wnd()
{
	// Область для ввода сеточной функции.
	// Кнопка для открытия файла с данными сеточной функции
	QVBoxLayout* vbox = new QVBoxLayout();
	QPushButton* btn = new QPushButton("Задать функцию из файла", this);
	connect(btn, &QPushButton::clicked, this,
		&MainWindow::set_grid_function_from_file);
	vbox->addWidget(btn);
	// Кнопка для задания сеточной функции вручную
	btn = new QPushButton("Задать функцию вручную", this);
	connect(btn, &QPushButton::clicked, this,
		&MainWindow::set_grid_function_manually);
	vbox->addWidget(btn);
	// Таблица сеточной функции
	tbl = new QTableWidget(0, 2, this);
	tbl->setHorizontalHeaderLabels({ "x", "y" });
	vbox->addWidget(tbl, 1);
	QGroupBox* grid_fnc_box = new QGroupBox("Сеточная функция");
	grid_fnc_box->setLayout(vbox);

	// Область для интерполяции функции
	QHBoxLayout* hbox_inter = new QHBoxLayout();
	// Список доступных типов интерполяции
	method = new QComboBox();
	QStringList types = { SPLINE, LAGRANGE };
	method->addItems(types);
	hbox_inter->addWidget(method);
	// Кнопка для запуска интерполяции
	btn = new QPushButton("Интерполировать", this);
	connect(btn, &QPushButton::clicked, this, &MainWindow::interpolate);
	hbox_inter->addWidget(btn);
	vbox = new QVBoxLayout();
	vbox->addLayout(hbox_inter);
	// Область для графика
	plot = new QCustomPlot();
	plot->xAxis->setLabel("x");
	plot->yAxis->setLabel("y");
	plot->xAxis->setRange(-1, 1);
	plot->yAxis->setRange(-1, 1);
	plot->replot();
	vbox->addWidget(plot, 1);
	QGroupBox* interpolation_box = new QGroupBox("Интерполяция");
	interpolation_box->setLayout(vbox);

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget(grid_fnc_box);
	hbox->addWidget(interpolation_box, 1);
	// Размещаем по центру окна
	QWidget* main_widget = new QWidget();
	main_widget->setLayout(hbox);
	setCentralWidget(main_widget);
}

/**
 * Метод создает меню на главном окне.
 */
void MainWindow::create_menu()
{
	QMenu* menu;
	// Меню с чтением данных
	menu = menuBar()->addMenu("Файл");
	// Пунтк меню для чтения файла с данными
	QAction* file = new QAction("Открыть файл", this);
	connect(file, &QAction::triggered, this,
		&MainWindow::set_grid_function_from_file);
	menu->addAction(file);
	// Пункт меню для выхода из приложения
	QAction* quit = new QAction("Выход", this);
	connect(quit, &QAction::triggered, qApp, &QApplication::quit);
	menu->addSeparator(); // устанавливаем разделитель
	menu->addAction(quit); // добавляем действие "Выход"

	// Меню с типами интерполяции
	menu = menuBar()->addMenu("Интерполяция");
	// Пункт меню с интерполяцией кубическими сплайнами
	menu_spline = new QAction(SPLINE, this);
	connect(menu_spline, &QAction::triggered, this, &MainWindow::interpolate);
	menu->addAction(menu_spline);
	// Пункт меню с интерполяцией полиномами Лагранжа
	menu_lagrange = new QAction(LAGRANGE, this);
	connect(menu_lagrange, &QAction::triggered, this,
		&MainWindow::interpolate);
	menu->addAction(menu_lagrange);

	// Меню с информацией о приложении
	menu = menuBar()->addMenu("Справка");
	QAction* info = new QAction("О приложении", this);
    connect(info, &QAction::triggered, this, &MainWindow::show_info);
	menu->addAction(info);
}

/**
 * Метод собирает значения сеточной функции из таблицы.
 * @return: true, если для полученной сеточной функции можно выполнить
 * интерполяцию, иначе false.
 */
bool MainWindow::get_grid_function()
{
	// Очищаем списки с координатами узлов и значениями сеточной функции
	x.clear();
	y.clear();
	// Пробегаем по строкам таблицы
	int row = 0;
	while (row < tbl->rowCount())
	{
		double values[2];
		try
		{
			for (int column = 0; column < 2; column++)
			{
				QLineEdit* cell = (QLineEdit*)tbl->cellWidget(row, column);
				if (cell->text() == "" || cell->text() == "-")
					throw - 1;
				values[column] = cell->text().toDouble();
			}
		}
		catch (...)
		{
			tbl->removeRow(row);
			continue;
		}
		x.push_back(values[0]);
		y.push_back(values[1]);
		row++;
	}
	return x.size() > 1;
}

/**
 * Слот для интерполяции сеточной функции.
 */
void MainWindow::interpolate()
{
	// Получаем значения сеточной функции
	if (!get_grid_function())
	{
		// Сеточная функция не подходит для интерполяции
		QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(ICON));
        msgBox.setWindowTitle("Ошибка");
		msgBox.setText("Интерполяция невозможна.");
		msgBox.exec();
		return;
	}
    // Сортируем узлы по возрастанию
    sort_quick(x, y, 0, x.size() - 1);
    // Выводим отсортированную сеточную функцию в таблицу
    show_grid_function(x.size(), true);
	// Определяем тип интерполяции
	void (*interpolation)(
		unsigned int, std::vector<double>&, std::vector<double>&,
		QVector<double>&, QVector<double>&) = nullptr;
	if (sender() != menu_lagrange && sender() != menu_spline)
	{
		// Была нажата кнопка 'Интерполировать'
		QString interpolation_type = method->currentText();
		if (interpolation_type == SPLINE)
			interpolation = interpolate_spline;
		else if (interpolation_type == LAGRANGE)
			interpolation = interpolate_lagrange;
	}
	else
	{
		// Был выбран один из пунктов меню с типом интерполяции
		if (sender() == menu_spline)
			interpolation = interpolate_spline;
		else if (sender() == menu_lagrange)
			interpolation = interpolate_lagrange;
	}
	// Интерполируем сеточную функцию и вычисляем значения в новых точках
	QVector<double> x_new;
	QVector<double> y_new;
	unsigned int N = 1000;
	interpolation(N, x, y, x_new, y_new);
	// Рисуем график интерполированной функции
	show_plot(x_new, y_new);
}

/**
 * Метод для интерполяции сеточной функции полиномами Лагранжа.
 * @param n: количество точек, в которых нужно посчитать значения
 * интерполированной функции;
 * @param x, y: массивы с координатами узлов и значениями сеточной функции;
 * @param x_new, y_new: массивы, куда будут записаны координаты и значения
 * интерполированной функции.
 */
void MainWindow::interpolate_lagrange(
	unsigned int n, std::vector<double>& x, std::vector<double>& y,
	QVector<double>& x_new, QVector<double>& y_new)
{
    Lagrange l(x, y);
    double dx = (x[x.size() - 1] - x[0]) / (n - 1);
    for (unsigned int i = 0; i < n; i++)
    {
        x_new.push_back(x[0] + dx * i);
        y_new.push_back(l.calculate(x_new[i]));
    }
}

/**
 * Метод для интерполяции сеточной функции сплайнами.
 * @param n: количество точек, в которых нужно посчитать значения
 * интерполированной функции;
 * @param x, y: массивы с координатами узлов и значениями сеточной функции;
 * @param x_new, y_new: массивы, куда будут записаны координаты и значения
 * интерполированной функции.
 */
void MainWindow::interpolate_spline(
	unsigned int n, std::vector<double>& x, std::vector<double>& y,
	QVector<double>& x_new, QVector<double>& y_new)
{
	Spline s(x, y);
	double dx = (x[x.size() - 1] - x[0]) / (n - 1);
	for (unsigned int i = 0; i < n; i++)
	{
		x_new.push_back(x[0] + dx * i);
		y_new.push_back(s.calculate(x_new[i]));
	}
}

/**
 * Слот для чтения данных о сеточной функции из файла.
 */
void MainWindow::set_grid_function_from_file()
{
	// Открываем диалоговое окно для выбора файла
	QString filename = QFileDialog::getOpenFileName(
		this, "Открыть файл", "/", "Files (*.txt *.dat)");
	if (filename == "")
		return;
	// Очищаем массивы с координатами узлов и значениями сеточной функции
	x.clear();
	y.clear();
	// Читаем данные из файла
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();
		QStringList xy = line.split(" ");
		x.push_back(xy[0].toDouble());
		y.push_back(xy[1].toDouble());
	}
	// Выводим сеточную функцию в таблицу
	show_grid_function(x.size(), true);
}

/**
 * Слот для задания данных о сеточной функции вручную.
 */
void MainWindow::set_grid_function_manually()
{
	// Получаем количество узлов сеточной функции
	bool ok;
	int n = QInputDialog::getInt(
		this, "Задать количество узлов",
		"Введите количество узлов сеточной функции", 2, 2, 1000, 1, &ok);
	if (!ok)
		return;
	// Выводим таблицу для сеточной функции
	show_grid_function(n);
}

/**
 * Метод выводит сеточную функцию в таблицу.
 * @param n: количество строк в таблице;
 * @param have_values: true, если значения сеточной функции уже заданы.
 */
void MainWindow::show_grid_function(int n, bool have_values)
{
	// Валидатор для ячеек таблицы
	QRegExpValidator* validator =
		new QRegExpValidator(QRegExp("^(\\-)?[0-9]+[\\.]?[0-9]*$"));
	// Задаем количество строк в таблице
	tbl->setRowCount(n);
	// Записываем значения построчно
	for (int row = 0; row < n; row++)
	{
		for (int column = 0; column < 2; column++)
		{
			QString item = "";
			if (have_values && column == 0)
				item = tr("%1").arg(x[row]);
			else if (have_values && column == 1)
				item = tr("%1").arg(y[row]);
			// Устанавливаем значение и валидатор для ячейки
			QLineEdit* cell = new QLineEdit();
			cell->setValidator(validator);
			cell->setText(item);
			cell->setFrame(false);
			tbl->setCellWidget(row, column, cell);
		}
	}
}

/**
 * Слот выводит информацию о приложении.
 */
void MainWindow::show_info()
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(ICON));
    msgBox.setWindowTitle("Информация");
    msgBox.setText("Приложение для интерполяции сеточных функций.");
    msgBox.exec();
}

/**
 * Метод рисует график интерполированной функции.
 * @param x_new, y_new: координаты x и значения интерполированной функции.
 */
void MainWindow::show_plot(QVector<double>& x_new, QVector<double>& y_new)
{
	plot->clearGraphs();
	// График интерполированной функции
	plot->addGraph();
	plot->graph(0)->setPen(QPen(Qt::blue));
	plot->graph(0)->setData(x_new, y_new);
	plot->graph(0)->rescaleAxes();
	// График сеточной функции
	QVector<double> x_grid, y_grid;
	convert_vector(x, x_grid);
	convert_vector(y, y_grid);
	plot->addGraph();
	plot->graph(1)->setPen(QColor(255, 0, 0, 255));
	plot->graph(1)->setLineStyle(QCPGraph::lsNone);
	plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 8));
	plot->graph(1)->setData(x_grid, y_grid);
	plot->replot();
}
