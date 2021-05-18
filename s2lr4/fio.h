#pragma once

#pragma warning (disable : 4996) //Без этого ругается на fopen и strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define ONE_STRING_WITH 20
#define MAX_WIDTH 60

class fio {
	char* familia;
	char* imya;
	char* otchestvo;

public:
	fio() //конструктор по-умолчанию
	{
		//std::cout << "fio::Kонструктор по-умолчанию" << std::endl;
		familia = new char[ONE_STRING_WITH];
		strcpy(familia, "");
		imya = new char[ONE_STRING_WITH];
		strcpy(imya, "");
		otchestvo = new char[ONE_STRING_WITH];
		strcpy(otchestvo, "");
	}

	fio(const char* _fio) //конструктор c параметрами в виде одной строки
	{
		//std::cout << "fio::Kонструктор c параметрами в виде одной строки" << std::endl;

		char buff[3][ONE_STRING_WITH] = { "", "", "" };
		sscanf(_fio, "%s %s %s", buff[0], buff[1], buff[2]);

		familia = new char[strlen(buff[0]) + 1];
		imya = new char[strlen(buff[1]) + 1];
		otchestvo = new char[strlen(buff[2]) + 1];

		strcpy(familia, buff[0]);
		strcpy(imya, buff[1]);
		strcpy(otchestvo, buff[2]);
	}

	fio(const fio& F)
		: familia(new char[strlen(F.familia) + 1]), imya(new char[strlen(F.imya) + 1]), otchestvo(new char[strlen(F.otchestvo) + 1])
		//Kонструктор Копирования
 	{
		//std::cout << "fio::Kонструктор Копирования" << std::endl;
		strcpy(familia, F.familia);
		strcpy(imya, F.imya);
		strcpy(otchestvo, F.otchestvo);
	}


	fio& operator= (const fio &F) // Перегрузка оператора "="
	{
		//std::cout << "fio::Оператор =" << std::endl;

		if (this == &F) return *this; //самоприсваивание

		delete[] familia;
		delete[] imya;
		delete[] otchestvo;

		familia = new char[strlen(F.familia) + 1];
		imya = new char[strlen(F.imya) + 1];
		otchestvo = new char[strlen(F.otchestvo) + 1];

		strcpy(familia, F.familia);
		strcpy(imya, F.imya);
		strcpy(otchestvo, F.otchestvo);

		return *this;
	}

	~fio()
	{
		//std::cout << "~fio::Деструктор" << std::endl;
		delete[] familia;
		delete[] imya;
		delete[] otchestvo;
	}

	char* convertToString() {
		char* _fio = new char[MAX_WIDTH];

		sprintf(_fio, "%s %s %s", familia, imya, otchestvo);

		return _fio;
	}

	//Перегрузка оператора << (ДФ - дружественная функция)
	friend std::ostream& operator<<(std::ostream& out, const fio& F)
	{
		out << F.familia << " " << F.imya << " " << F.otchestvo;
		return out;
	}

	//Перегрузка оператора == (ДФ - дружественная функция)
	friend bool operator== (const fio &L1, const fio &L2)
	{
		if (strcmp(L1.familia, L2.familia) != 0) return false;
		if (strcmp(L1.imya, L2.imya) != 0) return false;
		if (strcmp(L1.otchestvo, L2.otchestvo) != 0) return false;

		return true;

	}
}; //class fio