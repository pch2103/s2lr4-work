#pragma once

#include "fio.h"

class common
{
	fio inName; //»м€ получател€
	char* inAddress; //јдрес получател€
	char* outAddress; //јдрес отправител€

public:
	common() : inName() //конструктор по-умолчанию
	{
		std::cout << "common::Kонструктор по-умолчанию" << std::endl;

		inAddress = new char[ONE_STRING_WITH];
		strcpy(inAddress, "");
		outAddress = new char[ONE_STRING_WITH];
		strcpy(outAddress, "");
	}

	//конструктор c 3 параметрами
	common(fio _fio, const char* _inAddress, const char* _outAddress) :
		inAddress(new char[strlen(_inAddress) + 1]),
		outAddress(new char[strlen(_outAddress) + 1]),
		inName(_fio)
	{
		std::cout << "common::Kонструктор c 3 параметрами" << std::endl;
		strcpy(inAddress, _inAddress);
		strcpy(outAddress, _outAddress);
	}

	//конструктор c 2 параметрами
	common(fio _fio, const char* _inAddress) :
		inAddress(new char[strlen(_inAddress) + 1]),
		outAddress(new char[ONE_STRING_WITH]),
		inName(_fio)
	{
		std::cout << "common::Kонструктор c 2 параметрами" << std::endl;
		strcpy(inAddress, _inAddress);
		strcpy(outAddress, "");
	}

	virtual ~common() {
		std::cout << "~common::¬иртуальный деструктор" << std::endl;
		delete[] inAddress;
		delete[] outAddress;
	}
	
	//сеттеры переменных базового класса
	void setFio(fio _f) { 
		inName = _f;
	}

	void setInAddress(char* _a) 
	{
		delete[] inAddress;
		inAddress = new char[strlen(_a) + 1];
		strcpy(inAddress, _a);
	}//setInAddress

	void setOutAddress(char* _a)
	{
		delete[] outAddress;
		outAddress = new char[strlen(_a) + 1];
		strcpy(outAddress, _a);
	}//setOutAddress

	//геттеры переменных базового класса
	fio getFio() const { return fio(inName); }
	char* getInAddress() const { return inAddress; }
	char* getOutAddress() const { return outAddress; }

	virtual void enterNewRecord() = 0; //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
	virtual int get_count() = 0; //получить количество записей в пам€ти
	virtual void add_count() = 0; //увеличить количество записей в пам€ти на 1 (при добавлении записи)
	virtual void showOneRecord(int) = 0; //показывает одну запись в пам€ти
    virtual void showAllRecords() = 0; //показать все записи в базе писем
	virtual void exitAndSave() = 0;  // запись данных из пам€ти на диск при выходе
};

