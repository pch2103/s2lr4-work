#pragma once
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "common.h"
#include "fio.h"
#include "date.h"

#define DELIVERY_BASE_NAME "deliveryBase.txt" //файл базы доставок

class delivery :
	public common
{
	date deliveryDate;
	static int count;
public:
	delivery() : common(), deliveryDate() //конструктор по-умолчанию
	{
		std::cout << "delivery::Kонструктор по-умолчанию" << std::endl;
	}

	//конструктор c параметрами
	delivery(fio _fio, const char* _inAddress, date _deliveryDate) :
		common(_fio, _inAddress),
		deliveryDate(_deliveryDate)
	{
		std::cout << "delivery::Kонструктор c параметрами" << std::endl;
	}

	//Kонструктор Копирования
	delivery(delivery& _L) :
		common(_L.getFio(), _L.getInAddress()),
		deliveryDate(_L.deliveryDate)
	{
		std::cout << "delivery::Kонструктор Копирования" << std::endl;
	}

	// Перегрузка оператора "="
	delivery& operator= (const delivery &_D) 
	{
		if (this == &_D) return *this; //самоприсваивание
		std::cout << "delivery::Перегрузка оператора =" << std::endl;
		this->setFio(_D.getFio());
		this->setInAddress(_D.getInAddress());
		deliveryDate = _D.deliveryDate;

		return *this;
	}

	~delivery() {
		std::cout << "~delivery::Деструктор" << std::endl;
	}

	int get_count() { return count; } //получить количество записей в памяти
	void add_count() { ++count; } //увеличить количество записей в памяти на 1 (при добавлении записи)

	void enterNewRecord(); //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
	delivery* addNewRecord(delivery*);//выделение памяти под новую структу и заполнение ее данными
	void showOneRecord(int); //показывает одну запись в памяти
	void showAllRecords(); //показать все записи в базе писем
	void exitAndSave();  // запись данных из памяти на диск при выходе
	delivery* loadRecords(); //загрузка данных из файла и вывод их на экран и загрузка в пямять для работы
}; //class delivery

void showDeliveryHeader(const char*);
int delivery::count = 0;

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// РЕАЛИЗАЦИЯ МЕТОДОВ
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

//ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
void delivery::enterNewRecord()
{
	char _inName[MAX_WIDTH], _inAddress[ONE_STRING_WITH];
	int dd, mm, yyyy;
	date _deliveryDate;

	std::cout << "==================================================" << std::endl;
	std::cout << "Доставка: Добавление новой записи в базу доставок:" << std::endl;
	std::cout << "==================================================" << std::endl;

	std::cout << "ФИО отправителя: ";
	std::cin.getline(_inName, MAX_WIDTH);
	this->setFio(fio(_inName));

	std::cout << "Адрес получателя: ";
	std::cin.getline(_inAddress, ONE_STRING_WITH);
	this->setInAddress(_inAddress);

	std::cout << "Дата доставки письма: ";

	//защита от неверного ввода https://ravesli.com/urok-72-obrabotka-nekorrektnogo-vvoda-cherez-std-cin/#toc-2
	do {
		std::cout << "День (дд): ";
		std::cin >> dd;

		if (std::cin.fail()) // если предыдущее извлечение было неудачным,
		{
			std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
			std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
		}
		else if (dd < 1 || dd > 31)  
		{
			std::cout << "Некорректная дата (день)" << std::endl;
		}
		else break;
	} while (true);

	do {
		std::cout << "Месяц (мм): ";
		std::cin >> mm;

		if (std::cin.fail()) // если предыдущее извлечение было неудачным,
		{
			std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
			std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
		}
		else if (mm < 1 || mm > 12)
		{
			std::cout << "Некорректная дата (месяц)" << std::endl;
		}
		else break;
	} while (true);

	do {
		std::cout << "Год (ГГГГ): ";
		std::cin >> yyyy;
		if (std::cin.fail()) // если предыдущее извлечение было неудачным,
		{
			std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
			std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
		}
		else if (yyyy < 2000 || yyyy > CURRENT_YEAR)
		{
			std::cout << "Некорректная дата (год)" << std::endl;
		}
		else break;
	} while (true);

	this->deliveryDate.set(dd, mm, yyyy);

} // enterNewRecord()

delivery* delivery::addNewRecord(delivery* BS) //выделение памяти под новую структу и заполнение ее данными
{
	int amount = this->get_count();

	if (amount == 0)
	{
		BS = new delivery(this->getFio(), this->getInAddress(), this->deliveryDate);
	}
	else
	{
		delivery *T = new delivery[amount + 1];

		for (int i = 0; i < amount; ++i)  // копируем во временный объект
		{
			T[i] = BS[i];
			BS[i].~delivery(); // Удаляем ранние записи из памяти - вызываем деструктор явно для каждой записи
		}
		T[amount] = delivery(this->getFio(), this->getInAddress(), this->deliveryDate); //Добавляем новую запись в память
		BS = T; //временный объект становится "присланным"
	}

	this->add_count(); // Увеличиваем счетчик (static-переменная)

	return BS;

} // addNewRecord()


void delivery::showAllRecords() //показывает все записи в памяти
{
	showDeliveryHeader("БАЗА ДОСТАВОК:");

	for (int i = 0; i < this->get_count(); ++i) {
		(this + i)->showOneRecord(i + 1);
	}
	std::cout << std::endl << "=========================================================================================================================" << std::endl << std::endl;
}// showAllRecords()

void showDeliveryHeader(const char* title) //вспомогательная функция - печать заголовка таблицы
{
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "=========================================================================================================================" << std::endl << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(4);
	std::cout << " №";
	std::cout.setf(std::ios::left);
	std::cout.width(MAX_WIDTH);
	std::cout << "ФИО получателя";
	std::cout.setf(std::ios::left);
	std::cout.width(ONE_STRING_WITH);
	std::cout << "Адрес получателя";
	std::cout.setf(std::ios::left);
	std::cout.width(18);
	std::cout << "Дата доставки";
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
}

void delivery::showOneRecord(int number = 1) //вспомогательная функция - печать одной записи
{
	std::cout.setf(std::ios::left);
	std::cout << " ";
	std::cout.width(4);
	std::cout << number;
	std::cout.width(MAX_WIDTH);
	std::cout << std::left << this->getFio().convertToString();
	std::cout.setf(std::ios::left);
	std::cout.width(ONE_STRING_WITH);
	std::cout << this->getInAddress();
	std::cout.setf(std::ios::left);
	std::cout << this->deliveryDate.convertToString();
	std::cout << std::endl;
}

delivery* delivery::loadRecords() //загрузка данных из файла и вывод их на экран и загрузка в пямять для работы
{
	double _letterPrice = 0;
	char* _fio = new char[MAX_WIDTH];
	char*_inAddress = new char[ONE_STRING_WITH];
	char* _deliveryDate = new char[12];

	fio _F;
	date _D;

	delivery T;
	delivery* PT = nullptr;

	FILE* FP = fopen(DELIVERY_BASE_NAME, "r");

	if (FP != NULL) {
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "Загрузка базы доставок:" << std::endl;

		// Ниже все читается строками из файла

		while (fscanf(FP, "%99[^\t]\t%99[^\t]\t%99[^\n]\n", _fio, _inAddress, _deliveryDate) != EOF)
		{
			std::cout << _fio << " " << _inAddress << " " << _deliveryDate << std::endl;

			_F = fio(_fio);
			_D = date(_deliveryDate);

			T = delivery(_F, _inAddress, _D);
			PT = T.addNewRecord(PT);
		}
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++++" << std::endl;
		fclose(FP);

		delete[] _fio;
		delete[] _inAddress;
		delete[] _deliveryDate;

		return PT;
	}

	return nullptr;
} //loadRecords()

void delivery::exitAndSave() // запись базы из памяти в файл при выходе
{
	int amount = this->get_count();

	FILE* FP = fopen(DELIVERY_BASE_NAME, "w");

	for (int i = 0; i < amount; ++i) {
		fprintf(FP, "%s\t%s\t%s\n", (this + i)->getFio().convertToString(), (this + i)->getInAddress(), (this + i)->deliveryDate.convertToString());
	}

	if (FP != NULL)
	{
		fclose(FP);
	}
} //exitAndSave()
