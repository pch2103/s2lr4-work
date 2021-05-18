#pragma once
#include <iostream>

#define CURRENT_YEAR 2021

class date
{
	int day;
	int mounth;
	int year;
public:
	date(): day(1), mounth(1), year(CURRENT_YEAR) //конструктор по-умолчанию
	{
		std::cout << "date::Kонструктор по-умолчанию" << std::endl;
	}

	date(const int _d, const int _m, const int _y) : day(_d), mounth(_m), year(_y) //конструктор c параметрами
	{
		std::cout << "date::Kонструктор c параметрами" << std::endl;
	}

	date(const char*_date) //конструктор c параметрами в виде одной строки дата в формате dd-mm-yyyy
	{
		char buff[3][ONE_STRING_WITH] = { "", "", "" };
		sscanf(_date, "%99[^-]-%99[^-]-%s", buff[0], buff[1], buff[2]);

		day = std::stoi(buff[0]);
		mounth = std::stoi(buff[1]);
		year = std::stoi(buff[2]);
	}

	date(const date& D) : day(D.day), mounth(D.mounth), year(D.year)
		//Kонструктор Копирования
	{
		std::cout << "fio::Kонструктор Копирования" << std::endl;
	}

	date& operator= (const date &D) // Перегрузка оператора "="
	{
		std::cout << "date::Оператор =" << std::endl;

		if (this == &D) return *this; //самоприсваивание

		day = D.day;
		mounth = D.mounth;
		year = D.year;

		return *this;
	}

	~date()
	{
		std::cout << "~date::Деструктор" << std::endl;
	}

	char* convertToString() {
		char* _date = new char[12];

		sprintf(_date, "%d-%d-%d", day, mounth, year);

		return _date;
	}


	//Простой сеттер
	void set(int dd, int mm, int yyyy) 
	{
		day = dd;
		mounth = mm;
		year = yyyy;
	}

	//Перегрузка оператора == (ДФ - дружественная функция)
	friend bool operator== (const date &left, const date &right)
	{
		int ly = left.year;
		int ry = right.year;

		if (left.day != right.day) return false;
		if (left.mounth != right.mounth) return false;

		if (ly >= 2000) ly -= 2000; //исключаем ошибку сравнения годов, например 2021 == 21
		if (ry >= 2000) ry -= 2000;

		if (ly != ry) return false;

		return true;

	}
};

