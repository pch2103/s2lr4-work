#pragma once
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <Windows.h>

#include "fio.h"

#pragma warning (disable : 4996) //Без этого ругается на fopen и strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define ONE_STRING_WITH 20
#define MAX_WIDTH 60
#define BASE_NAME "letterBasePart.txt" //Структура файла будет отличаться от базового варианта с одним классом


class letter {
	fio inName; //Имя получателя
	char* inAddress; //Адрес получателя
	char* outAddress; //Адрес отправителя
	double letterPrice; //Стоимость письма

	static int count;
public:
	letter() : inName(), letterPrice(0.0) //конструктор по-умолчанию
	{
		std::cout << "letter::Kонструктор по-умолчанию" << std::endl;

		inAddress = new char[ONE_STRING_WITH];
		strcpy(inAddress, "");
		outAddress = new char[ONE_STRING_WITH];
		strcpy(outAddress, "");
	}

	letter(fio _fio) : inName(_fio), letterPrice(0.0) //конструктор по-умолчанию
	{
		std::cout << "letter::Kонструктор по-умолчанию" << std::endl;

		inAddress = new char[ONE_STRING_WITH];
		strcpy(inAddress, "");
		outAddress = new char[ONE_STRING_WITH];
		strcpy(outAddress, "");
	}

	//конструктор c параметрами
	letter(fio _fio, const char* _inAddress, const char* _outAddress, double _letterPrice = 0.0) :
		inName(_fio),
		inAddress(new char[strlen(_inAddress) + 1]),
		outAddress(new char[strlen(_outAddress) + 1]),
		letterPrice(_letterPrice)

	{
		std::cout << "letter::Kонструктор c параметрами" << std::endl;
		strcpy(inAddress, _inAddress);
		strcpy(outAddress, _outAddress);
	}

	//Kонструктор Копирования
	letter(const letter& _L) :
		inName(_L.inName),
		inAddress(new char[strlen(_L.inAddress) + 1]),
		outAddress(new char[strlen(_L.outAddress) + 1]),
		letterPrice(_L.letterPrice)
	{
		std::cout << "letter::Kонструктор Копирования" << std::endl;
		strcpy(inAddress, _L.inAddress);
		strcpy(outAddress, _L.outAddress);
	}

	letter& operator= (const letter &_L) // Перегрузка оператора "="
	{
		if (this == &_L) return *this; //самоприсваивание

		std::cout << "letter::Перегрузка оператора =" << std::endl;
		delete[] inAddress;
		delete[] outAddress;
		inName = _L.inName;
		inAddress = new char[strlen(_L.inAddress) + 1];
		outAddress = new char[strlen(_L.outAddress) + 1];
		strcpy(inAddress, _L.inAddress);
		strcpy(outAddress, _L.outAddress);
		letterPrice = _L.letterPrice;

		return *this;
	}

	//Перегрузка унарного оператора ! - сортировка по алфавиту, по адресу отправителя (МК - метод класса)
	letter& operator! ()
	{
		int i, j, amount = this->get_count();

		letter T;
		std::cout << "letter::Перегрузка оператора !" << std::endl;
		for (i = 0; i < amount; ++i) {
			for (j = i + 1; j < amount; ++j) {
				if (strcmp(this[i].outAddress, this[j].outAddress) > 0) {
					T = this[i];
					this[i] = this[j];
					this[j] = T;
				}
			} //for j
		} //for i

		return *this;
	}

	~letter() //Деструктор
	{
		std::cout << "~letter::Деструктор" << std::endl;
		delete[] inAddress;
		delete[] outAddress;
	}

	//ДФ - дружественные функции ===============================================
	void friend showOneRecord(letter, int); //вспомогательная функция - печать одной записи
	friend letter* priceSort(letter*); //4 -  Сортировать писем по увелечению стоимости ДФ - требование в условие задачи

	//МК - методы класса ===============================================
	letter* loadRecords();  // чтение данных в память из файла при входе
	void exitAndSave();  // запись данных из памяти на диск при выходе
	void enterNewRecord(); //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
	letter* addNewRecord(letter*); //выделение памяти под новую структу и заполнение ее данными
	void inNameSearch(); //Найти все письма определенного получателя

	int get_count() { return count; } //получить количество записей в памяти
	void add_count() { ++count; } //увеличить количество записей в памяти на 1 (при добавлении записи)
}; //class letter

int letter::count = 0;

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// РЕАЛИЗАЦИЯ МЕТОДОВ
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

//ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
void letter::enterNewRecord()
{
	char _inName[MAX_WIDTH], _inAddress[ONE_STRING_WITH], _outAddress[ONE_STRING_WITH];
	double _letterPrice;

	std::cout << "==================================" << std::endl;
	std::cout << "Добавление нового письма в базу:" << std::endl;
	std::cout << "==================================" << std::endl;

	std::cout << "ФИО отправителя: ";
	std::cin.getline(_inName, MAX_WIDTH);
	this->inName = fio::fio(_inName);

	std::cout << "Адрес получателя: ";
	std::cin.getline(_inAddress, ONE_STRING_WITH);
	this->inAddress = new char[strlen(_inAddress) + 1];
	strcpy(this->inAddress, _inAddress);

	std::cout << "Адрес oтправителя: ";
	std::cin.getline(_outAddress, ONE_STRING_WITH);
	this->outAddress = new char[strlen(_outAddress) + 1];
	strcpy(this->outAddress, _outAddress);

	std::cout << "Стоимость письма: ";
	std::cin >> _letterPrice;
	std::cin.get();
	this->letterPrice = _letterPrice;

} // enterNewRecord()

letter* letter::addNewRecord(letter* LP) //выделение памяти под новую структу и заполнение ее данными
{
	int amount = this->get_count();

	if (amount == 0)
	{
		LP = new letter(this->inName, this->inAddress, this->outAddress, this->letterPrice);
	}
	else
	{
		letter *T = new letter[amount + 1];

		for (int i = 0; i < amount; ++i)  // копируем во временный объект
		{
			T[i] = LP[i];
			LP[i].~letter(); // Удаляем ранние записи из памяти (delete[] LP; - не работает) вызываем деструктор явно для каждой записи
		}

		T[amount] = letter(this->inName, this->inAddress, this->outAddress, this->letterPrice); //Добавляем новую запись в память
		//delete[] P; - не работает см. предыдущий комментарий - удаление перенесено туда
		LP = T; //временный объект становится "присланным"
	}

	this->add_count(); // Увеличиваем счетчик (static-переменная)

	return LP;
} // addNewRecord()


void showRecordHeader(const char* title) //вспомогательная функция - печать заголовка таблицы
{
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(4);
	std::cout << " №";
	std::cout.setf(std::ios::left);
	std::cout.width(MAX_WIDTH);
	std::cout << "ФИО отправителя";
	std::cout.setf(std::ios::left);
	std::cout.width(ONE_STRING_WITH);
	std::cout << "Адрес получателя";
	std::cout.setf(std::ios::left);
	std::cout.width(ONE_STRING_WITH);
	std::cout << "Адрес отправителя";
	std::cout.setf(std::ios::left);
	std::cout.width(18);
	std::cout << "Стоимость письма";
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
}

void showOneRecord(letter Record, int number = 1) //вспомогательная функция - печать одной записи
{
	std::cout.setf(std::ios::left);
	std::cout << " ";
	std::cout.width(4);
	std::cout << number;
	std::cout.setf(std::ios::left);
	std::cout.width(MAX_WIDTH);
	std::cout << Record.inName.getFio();
	std::cout.setf(std::ios::left);
	std::cout.width(ONE_STRING_WITH);
	std::cout << Record.inAddress;
	std::cout.setf(std::ios::left);
	std::cout.width(ONE_STRING_WITH);
	std::cout << Record.outAddress;
	std::cout.setf(std::ios::left);
	// Вывод с определенной точностью
	//https://stackoverflow.com/questions/554063/how-do-i-print-a-double-value-with-full-precision-using-cout
	std::cout << std::fixed << std::setprecision(2) << Record.letterPrice;
	std::cout << std::endl;
}


//Найти все письма определенного получателя
void letter::inNameSearch()
{
	int count = 0;
	int amount = this->get_count();
	char buffer[MAX_WIDTH];
	char* _fio;

	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
	std::cout << "Ведите ФИО получателя письма: ";
	std::cin.getline(buffer, MAX_WIDTH);
	_fio = new char[strlen(buffer) + 1];
	strcpy(_fio, buffer);
	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;

	showRecordHeader("Поиск письма по ФИО отправителя:");

	for (int i = 0; i < amount; ++i) {
		// использование перегруженного оператора == для поиска по фамилии получателя (условие задачи)
		if (_fio == this[i].inName)
		{
			++count;
			showOneRecord(this[i], count);
		}
	} //for
	if (count == 0) std::cout << "Записей для введенных ФИО нет" << std::endl;
	std::cout << "=========================================================================================================================" << std::endl;

	delete[] _fio;

}//inNameSearch()

letter* priceSort(letter* L) //4 -  Сортировать писем по увелечению стоимости
{
	int i, j, amount = L->get_count();

	letter T;

	for (i = 0; i < amount; ++i) {
		for (j = i + 1; j < amount; ++j) {
			if (L[i].letterPrice > L[j].letterPrice) {
				T = L[i];
				L[i] = L[j];
				L[j] = T;
			}
		} //for j
	} //for i
	return L;
} //sortRecordsByAlpha()

letter* letter::loadRecords() //загрузка данных из файла и вывод их на экран и загрузка в пямять для работы
{
	double _letterPrice = 0;
	char* _fio = new char[MAX_WIDTH];
	char*_inAddress = new char[ONE_STRING_WITH];
	char*_outAddress = new char[ONE_STRING_WITH];
	char* _letterPriceString = new char[ONE_STRING_WITH];

	fio _F;

	letter T;
	letter* PT = nullptr;

	FILE* FP = fopen(BASE_NAME, "r");

	if (FP != NULL) {
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "Загрузка текущей базы:" << std::endl;

		// Ниже все читается строками из файла, затим цена преобразовывается в double с помощю std::stod()

		while (fscanf(FP, "%99[^\t]\t%99[^\t]\t%99[^\t]\t%99[^\n]\n", _fio, _inAddress, _outAddress, _letterPriceString) != EOF)
		{
			_letterPrice = std::stod(_letterPriceString);
			std::cout << _fio << " " << _inAddress << " " << _outAddress << " " << _letterPrice << std::endl;
			_F = fio(_fio);
			T = letter(_F, _inAddress, _outAddress, _letterPrice);
			PT = T.addNewRecord(PT);
		}
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++++" << std::endl;
		fclose(FP);

		delete[] _fio;
		delete[] _inAddress;
		delete[] _outAddress;

		return PT;
	}

	return nullptr;
} //loadRecords()

void letter::exitAndSave() // запись базы из памяти в файл при выходе
{
	int amount = this->get_count();

	FILE* FP = fopen(BASE_NAME, "w");

	for (int i = 0; i < amount; ++i) {
		fprintf(FP, "%s\t%s\t%s\t%.2f\n", (this + i)->inName.getFio(), (this + i)->inAddress, (this + i)->outAddress, (this + i)->letterPrice);
	}

	if (FP != NULL)
	{
		fclose(FP);
	}
} //exitAndSave()
