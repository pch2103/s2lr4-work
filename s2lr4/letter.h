#pragma once
#include <cstring>
#include <sstream>
#include <iomanip>
#include "common.h"
#include "fio.h"

#define LETTER_BASE_NAME "letterBase.txt" //���� ���� �����

class letter :
	public common
{
	double letterPrice; //��������� ������
	static int count;
public:
	letter() : common(), letterPrice(0.0) //����������� ��-���������
	{
		std::cout << "letter::K���������� ��-���������" << std::endl;
	}

	//����������� c �����������
	letter(fio _fio, const char* _inAddress, const char* _outAddress, double _letterPrice = 0.0) :
		common(_fio, _inAddress, _outAddress),
		letterPrice(_letterPrice)
	{
		std::cout << "letter::K���������� c �����������" << std::endl;
	}

	//K���������� �����������
	letter(letter& _L) : 
		common(_L.getFio(), _L.getInAddress(), _L.getOutAddress()),
		letterPrice(_L.letterPrice)
	{
		std::cout << "letter::K���������� �����������" << std::endl;
	}

	letter& operator= (const letter &_L) // ���������� ��������� "="
	{
		if (this == &_L) return *this; //����������������
		std::cout << "letter::���������� ��������� =" << std::endl;
		this->setFio(_L.getFio());
		this->setInAddress(_L.getInAddress());
		this->setOutAddress(_L.getOutAddress());
		letterPrice = _L.letterPrice;

		return *this;
	}

	~letter() {
		std::cout << "~letter::����������" << std::endl;
	}

	int get_count() { return count; } //�������� ���������� ������� � ������
	void add_count() { ++count; } //��������� ���������� ������� � ������ �� 1 (��� ���������� ������)
	void enterNewRecord(); //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
	letter* addNewRecord(letter*);//��������� ������ ��� ����� ������� � ���������� �� �������
	void showAllRecords(); //�������� ��� ������ � ���� �����
	void showOneRecord(int); //���������� ���� ������ � ������ 
	void exitAndSave();  // ������ ������ �� ������ �� ���� ��� ������
	letter* loadRecords(); //�������� ������ �� ����� � ����� �� �� ����� � �������� � ������ ��� ������
}; //class letter

void showRecordHeader(const char*);
int letter::count = 0;


//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// ���������� �������
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

//���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
void letter::enterNewRecord()
{
	char _inName[MAX_WIDTH], _inAddress[ONE_STRING_WITH], _outAddress[ONE_STRING_WITH];
	double _letterPrice;

	std::cout << "=============================================" << std::endl;
	std::cout << "������: ���������� ����� ������ � ���� �����:" << std::endl;
	std::cout << "=============================================" << std::endl;

	std::cout << "��� �����������: ";
	std::cin.getline(_inName, MAX_WIDTH);
	this->setFio(fio(_inName));

	std::cout << "����� ����������: ";
	std::cin.getline(_inAddress, ONE_STRING_WITH);
	this->setInAddress(_inAddress);

	std::cout << "����� o����������: ";
	std::cin.getline(_outAddress, ONE_STRING_WITH);
	this->setOutAddress(_outAddress);

	//������ �� ��������� ����� https://ravesli.com/urok-72-obrabotka-nekorrektnogo-vvoda-cherez-std-cin/#toc-2
	do {
		std::cout << "��������� ������: ";
		std::cin >> _letterPrice;

		if (std::cin.fail()) // ���� ���������� ���������� ���� ���������,
		{
			std::cin.clear(); // �� ���������� cin � '�������' ����� ������
			std::cin.ignore(32767, '\n'); // � ������� �������� ����������� ����� �� �������� ������
		}
		else break;
	} while (true);

	this->letterPrice = _letterPrice;


} // enterNewRecord()

letter* letter::addNewRecord(letter* BS) //��������� ������ ��� ����� ������� � ���������� �� �������
{
	int amount = this->get_count();

	if (amount == 0)
	{
		BS = new letter(this->getFio(), this->getInAddress(), this->getOutAddress(), this->letterPrice);
	}
	else
	{
		letter *T = new letter[amount+1];

		for (int i = 0; i < amount; ++i)  // �������� �� ��������� ������
		{
			T[i] = BS[i];
			BS[i].~letter(); // ������� ������ ������ �� ������ - �������� ���������� ���� ��� ������ ������
		}
		T[amount] =  letter(this->getFio(), this->getInAddress(), this->getOutAddress(), this->letterPrice); //��������� ����� ������ � ������
		BS = T; //��������� ������ ���������� "����������"
	}

	this->add_count(); // ����������� ������� (static-����������)
	
	return BS;

} // addNewRecord()

void letter::showAllRecords() //���������� ��� ������ � ������
{
	showRecordHeader("���� �����:");

	for (int i = 0; i < this->get_count(); ++i) {
		(this+i)->showOneRecord(i + 1);
	}
	std::cout << std::endl << "=========================================================================================================================" << std::endl << std::endl;
}// showAllRecords()

void showRecordHeader(const char* title) //��������������� ������� - ������ ��������� �������
{
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "=========================================================================================================================" << std::endl << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(4);
	std::cout << " �";
	std::cout.setf(std::ios::left);
	std::cout.width(MAX_WIDTH);
	std::cout << "��� ����������";
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

void letter::showOneRecord(int number = 1) //��������������� ������� - ������ ����� ������
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
	std::cout.width(ONE_STRING_WITH);
	std::cout << this->getOutAddress();
	std::cout.setf(std::ios::left);
	// ����� � ������������ ���������
	//https://stackoverflow.com/questions/554063/how-do-i-print-a-double-value-with-full-precision-using-cout
	std::cout << std::fixed << std::setprecision(2) << this->letterPrice;
	std::cout << std::endl;
}

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

	FILE* FP = fopen(LETTER_BASE_NAME, "r");

	if (FP != NULL) {
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "�������� ���� �����:" << std::endl;

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

	FILE* FP = fopen(LETTER_BASE_NAME, "w");

	for (int i = 0; i < amount; ++i) {
		fprintf(FP, "%s\t%s\t%s\t%.2f\n", (this + i)->getFio().convertToString(), (this + i)->getInAddress(), (this + i)->getOutAddress() , (this + i)->letterPrice);
	}

	if (FP != NULL)
	{
		fclose(FP);
	}
} //exitAndSave()