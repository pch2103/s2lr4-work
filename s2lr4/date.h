#pragma once
#include <iostream>

#define CURRENT_YEAR 2021

class date
{
	int day;
	int mounth;
	int year;
public:
	date(): day(1), mounth(1), year(CURRENT_YEAR) //����������� ��-���������
	{
		std::cout << "date::K���������� ��-���������" << std::endl;
	}

	date(const int _d, const int _m, const int _y) : day(_d), mounth(_m), year(_y) //����������� c �����������
	{
		std::cout << "date::K���������� c �����������" << std::endl;
	}

	date(const char*_date) //����������� c ����������� � ���� ����� ������ ���� � ������� dd-mm-yyyy
	{
		char buff[3][ONE_STRING_WITH] = { "", "", "" };
		sscanf(_date, "%99[^-]-%99[^-]-%s", buff[0], buff[1], buff[2]);

		day = std::stoi(buff[0]);
		mounth = std::stoi(buff[1]);
		year = std::stoi(buff[2]);
	}

	date(const date& D) : day(D.day), mounth(D.mounth), year(D.year)
		//K���������� �����������
	{
		std::cout << "fio::K���������� �����������" << std::endl;
	}

	date& operator= (const date &D) // ���������� ��������� "="
	{
		std::cout << "date::�������� =" << std::endl;

		if (this == &D) return *this; //����������������

		day = D.day;
		mounth = D.mounth;
		year = D.year;

		return *this;
	}

	~date()
	{
		std::cout << "~date::����������" << std::endl;
	}

	char* convertToString() {
		char* _date = new char[12];

		sprintf(_date, "%d-%d-%d", day, mounth, year);

		return _date;
	}


	//������� ������
	void set(int dd, int mm, int yyyy) 
	{
		day = dd;
		mounth = mm;
		year = yyyy;
	}

	//���������� ��������� == (�� - ������������� �������)
	friend bool operator== (const date &left, const date &right)
	{
		int ly = left.year;
		int ry = right.year;

		if (left.day != right.day) return false;
		if (left.mounth != right.mounth) return false;

		if (ly >= 2000) ly -= 2000; //��������� ������ ��������� �����, �������� 2021 == 21
		if (ry >= 2000) ry -= 2000;

		if (ly != ry) return false;

		return true;

	}
};

