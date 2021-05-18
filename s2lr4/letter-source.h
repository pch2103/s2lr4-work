#pragma once
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <Windows.h>

#include "fio.h"

#pragma warning (disable : 4996) //��� ����� �������� �� fopen � strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define ONE_STRING_WITH 20
#define MAX_WIDTH 60
#define BASE_NAME "letterBasePart.txt" //��������� ����� ����� ���������� �� �������� �������� � ����� �������


class letter {
	fio inName; //��� ����������
	char* inAddress; //����� ����������
	char* outAddress; //����� �����������
	double letterPrice; //��������� ������

	static int count;
public:
	letter() : inName(), letterPrice(0.0) //����������� ��-���������
	{
		std::cout << "letter::K���������� ��-���������" << std::endl;

		inAddress = new char[ONE_STRING_WITH];
		strcpy(inAddress, "");
		outAddress = new char[ONE_STRING_WITH];
		strcpy(outAddress, "");
	}

	letter(fio _fio) : inName(_fio), letterPrice(0.0) //����������� ��-���������
	{
		std::cout << "letter::K���������� ��-���������" << std::endl;

		inAddress = new char[ONE_STRING_WITH];
		strcpy(inAddress, "");
		outAddress = new char[ONE_STRING_WITH];
		strcpy(outAddress, "");
	}

	//����������� c �����������
	letter(fio _fio, const char* _inAddress, const char* _outAddress, double _letterPrice = 0.0) :
		inName(_fio),
		inAddress(new char[strlen(_inAddress) + 1]),
		outAddress(new char[strlen(_outAddress) + 1]),
		letterPrice(_letterPrice)

	{
		std::cout << "letter::K���������� c �����������" << std::endl;
		strcpy(inAddress, _inAddress);
		strcpy(outAddress, _outAddress);
	}

	//K���������� �����������
	letter(const letter& _L) :
		inName(_L.inName),
		inAddress(new char[strlen(_L.inAddress) + 1]),
		outAddress(new char[strlen(_L.outAddress) + 1]),
		letterPrice(_L.letterPrice)
	{
		std::cout << "letter::K���������� �����������" << std::endl;
		strcpy(inAddress, _L.inAddress);
		strcpy(outAddress, _L.outAddress);
	}

	letter& operator= (const letter &_L) // ���������� ��������� "="
	{
		if (this == &_L) return *this; //����������������

		std::cout << "letter::���������� ��������� =" << std::endl;
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

	//���������� �������� ��������� ! - ���������� �� ��������, �� ������ ����������� (�� - ����� ������)
	letter& operator! ()
	{
		int i, j, amount = this->get_count();

		letter T;
		std::cout << "letter::���������� ��������� !" << std::endl;
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

	~letter() //����������
	{
		std::cout << "~letter::����������" << std::endl;
		delete[] inAddress;
		delete[] outAddress;
	}

	//�� - ������������� ������� ===============================================
	void friend showOneRecord(letter, int); //��������������� ������� - ������ ����� ������
	friend letter* priceSort(letter*); //4 -  ����������� ����� �� ���������� ��������� �� - ���������� � ������� ������

	//�� - ������ ������ ===============================================
	letter* loadRecords();  // ������ ������ � ������ �� ����� ��� �����
	void exitAndSave();  // ������ ������ �� ������ �� ���� ��� ������
	void enterNewRecord(); //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
	letter* addNewRecord(letter*); //��������� ������ ��� ����� ������� � ���������� �� �������
	void inNameSearch(); //����� ��� ������ ������������� ����������

	int get_count() { return count; } //�������� ���������� ������� � ������
	void add_count() { ++count; } //��������� ���������� ������� � ������ �� 1 (��� ���������� ������)
}; //class letter

int letter::count = 0;

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// ���������� �������
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

//���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
void letter::enterNewRecord()
{
	char _inName[MAX_WIDTH], _inAddress[ONE_STRING_WITH], _outAddress[ONE_STRING_WITH];
	double _letterPrice;

	std::cout << "==================================" << std::endl;
	std::cout << "���������� ������ ������ � ����:" << std::endl;
	std::cout << "==================================" << std::endl;

	std::cout << "��� �����������: ";
	std::cin.getline(_inName, MAX_WIDTH);
	this->inName = fio::fio(_inName);

	std::cout << "����� ����������: ";
	std::cin.getline(_inAddress, ONE_STRING_WITH);
	this->inAddress = new char[strlen(_inAddress) + 1];
	strcpy(this->inAddress, _inAddress);

	std::cout << "����� o����������: ";
	std::cin.getline(_outAddress, ONE_STRING_WITH);
	this->outAddress = new char[strlen(_outAddress) + 1];
	strcpy(this->outAddress, _outAddress);

	std::cout << "��������� ������: ";
	std::cin >> _letterPrice;
	std::cin.get();
	this->letterPrice = _letterPrice;

} // enterNewRecord()

letter* letter::addNewRecord(letter* LP) //��������� ������ ��� ����� ������� � ���������� �� �������
{
	int amount = this->get_count();

	if (amount == 0)
	{
		LP = new letter(this->inName, this->inAddress, this->outAddress, this->letterPrice);
	}
	else
	{
		letter *T = new letter[amount + 1];

		for (int i = 0; i < amount; ++i)  // �������� �� ��������� ������
		{
			T[i] = LP[i];
			LP[i].~letter(); // ������� ������ ������ �� ������ (delete[] LP; - �� ��������) �������� ���������� ���� ��� ������ ������
		}

		T[amount] = letter(this->inName, this->inAddress, this->outAddress, this->letterPrice); //��������� ����� ������ � ������
		//delete[] P; - �� �������� ��. ���������� ����������� - �������� ���������� ����
		LP = T; //��������� ������ ���������� "����������"
	}

	this->add_count(); // ����������� ������� (static-����������)

	return LP;
} // addNewRecord()


void showRecordHeader(const char* title) //��������������� ������� - ������ ��������� �������
{
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(4);
	std::cout << " �";
	std::cout.setf(std::ios::left);
	std::cout.width(MAX_WIDTH);
	std::cout << "��� �����������";
	std::cout.setf(std::ios::left);
	std::cout.width(ONE_STRING_WITH);
	std::cout << "����� ����������";
	std::cout.setf(std::ios::left);
	std::cout.width(ONE_STRING_WITH);
	std::cout << "����� �����������";
	std::cout.setf(std::ios::left);
	std::cout.width(18);
	std::cout << "��������� ������";
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
}

void showOneRecord(letter Record, int number = 1) //��������������� ������� - ������ ����� ������
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
	// ����� � ������������ ���������
	//https://stackoverflow.com/questions/554063/how-do-i-print-a-double-value-with-full-precision-using-cout
	std::cout << std::fixed << std::setprecision(2) << Record.letterPrice;
	std::cout << std::endl;
}


//����� ��� ������ ������������� ����������
void letter::inNameSearch()
{
	int count = 0;
	int amount = this->get_count();
	char buffer[MAX_WIDTH];
	char* _fio;

	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
	std::cout << "������ ��� ���������� ������: ";
	std::cin.getline(buffer, MAX_WIDTH);
	_fio = new char[strlen(buffer) + 1];
	strcpy(_fio, buffer);
	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;

	showRecordHeader("����� ������ �� ��� �����������:");

	for (int i = 0; i < amount; ++i) {
		// ������������� �������������� ��������� == ��� ������ �� ������� ���������� (������� ������)
		if (_fio == this[i].inName)
		{
			++count;
			showOneRecord(this[i], count);
		}
	} //for
	if (count == 0) std::cout << "������� ��� ��������� ��� ���" << std::endl;
	std::cout << "=========================================================================================================================" << std::endl;

	delete[] _fio;

}//inNameSearch()

letter* priceSort(letter* L) //4 -  ����������� ����� �� ���������� ���������
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

letter* letter::loadRecords() //�������� ������ �� ����� � ����� �� �� ����� � �������� � ������ ��� ������
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
		std::cout << "�������� ������� ����:" << std::endl;

		// ���� ��� �������� �������� �� �����, ����� ���� ����������������� � double � ������ std::stod()

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

void letter::exitAndSave() // ������ ���� �� ������ � ���� ��� ������
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
