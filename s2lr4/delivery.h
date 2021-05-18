#pragma once
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "common.h"
#include "fio.h"
#include "date.h"

#define DELIVERY_BASE_NAME "deliveryBase.txt" //���� ���� ��������

class delivery :
	public common
{
	date deliveryDate;
	static int count;
public:
	delivery() : common(), deliveryDate() //����������� ��-���������
	{
		std::cout << "delivery::K���������� ��-���������" << std::endl;
	}

	//����������� c �����������
	delivery(fio _fio, const char* _inAddress, date _deliveryDate) :
		common(_fio, _inAddress),
		deliveryDate(_deliveryDate)
	{
		std::cout << "delivery::K���������� c �����������" << std::endl;
	}

	//K���������� �����������
	delivery(delivery& _L) :
		common(_L.getFio(), _L.getInAddress()),
		deliveryDate(_L.deliveryDate)
	{
		std::cout << "delivery::K���������� �����������" << std::endl;
	}

	// ���������� ��������� "="
	delivery& operator= (const delivery &_D) 
	{
		if (this == &_D) return *this; //����������������
		std::cout << "delivery::���������� ��������� =" << std::endl;
		this->setFio(_D.getFio());
		this->setInAddress(_D.getInAddress());
		deliveryDate = _D.deliveryDate;

		return *this;
	}

	~delivery() {
		std::cout << "~delivery::����������" << std::endl;
	}

	int get_count() { return count; } //�������� ���������� ������� � ������
	void add_count() { ++count; } //��������� ���������� ������� � ������ �� 1 (��� ���������� ������)

	void enterNewRecord(); //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
	delivery* addNewRecord(delivery*);//��������� ������ ��� ����� ������� � ���������� �� �������
	void showOneRecord(int); //���������� ���� ������ � ������
	void showAllRecords(); //�������� ��� ������ � ���� �����
	void exitAndSave();  // ������ ������ �� ������ �� ���� ��� ������
	delivery* loadRecords(); //�������� ������ �� ����� � ����� �� �� ����� � �������� � ������ ��� ������
}; //class delivery

void showDeliveryHeader(const char*);
int delivery::count = 0;

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// ���������� �������
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

//���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
void delivery::enterNewRecord()
{
	char _inName[MAX_WIDTH], _inAddress[ONE_STRING_WITH];
	int dd, mm, yyyy;
	date _deliveryDate;

	std::cout << "==================================================" << std::endl;
	std::cout << "��������: ���������� ����� ������ � ���� ��������:" << std::endl;
	std::cout << "==================================================" << std::endl;

	std::cout << "��� �����������: ";
	std::cin.getline(_inName, MAX_WIDTH);
	this->setFio(fio(_inName));

	std::cout << "����� ����������: ";
	std::cin.getline(_inAddress, ONE_STRING_WITH);
	this->setInAddress(_inAddress);

	std::cout << "���� �������� ������: ";

	//������ �� ��������� ����� https://ravesli.com/urok-72-obrabotka-nekorrektnogo-vvoda-cherez-std-cin/#toc-2
	do {
		std::cout << "���� (��): ";
		std::cin >> dd;

		if (std::cin.fail()) // ���� ���������� ���������� ���� ���������,
		{
			std::cin.clear(); // �� ���������� cin � '�������' ����� ������
			std::cin.ignore(32767, '\n'); // � ������� �������� ����������� ����� �� �������� ������
		}
		else if (dd < 1 || dd > 31)  
		{
			std::cout << "������������ ���� (����)" << std::endl;
		}
		else break;
	} while (true);

	do {
		std::cout << "����� (��): ";
		std::cin >> mm;

		if (std::cin.fail()) // ���� ���������� ���������� ���� ���������,
		{
			std::cin.clear(); // �� ���������� cin � '�������' ����� ������
			std::cin.ignore(32767, '\n'); // � ������� �������� ����������� ����� �� �������� ������
		}
		else if (mm < 1 || mm > 12)
		{
			std::cout << "������������ ���� (�����)" << std::endl;
		}
		else break;
	} while (true);

	do {
		std::cout << "��� (����): ";
		std::cin >> yyyy;
		if (std::cin.fail()) // ���� ���������� ���������� ���� ���������,
		{
			std::cin.clear(); // �� ���������� cin � '�������' ����� ������
			std::cin.ignore(32767, '\n'); // � ������� �������� ����������� ����� �� �������� ������
		}
		else if (yyyy < 2000 || yyyy > CURRENT_YEAR)
		{
			std::cout << "������������ ���� (���)" << std::endl;
		}
		else break;
	} while (true);

	this->deliveryDate.set(dd, mm, yyyy);

} // enterNewRecord()

delivery* delivery::addNewRecord(delivery* BS) //��������� ������ ��� ����� ������� � ���������� �� �������
{
	int amount = this->get_count();

	if (amount == 0)
	{
		BS = new delivery(this->getFio(), this->getInAddress(), this->deliveryDate);
	}
	else
	{
		delivery *T = new delivery[amount + 1];

		for (int i = 0; i < amount; ++i)  // �������� �� ��������� ������
		{
			T[i] = BS[i];
			BS[i].~delivery(); // ������� ������ ������ �� ������ - �������� ���������� ���� ��� ������ ������
		}
		T[amount] = delivery(this->getFio(), this->getInAddress(), this->deliveryDate); //��������� ����� ������ � ������
		BS = T; //��������� ������ ���������� "����������"
	}

	this->add_count(); // ����������� ������� (static-����������)

	return BS;

} // addNewRecord()


void delivery::showAllRecords() //���������� ��� ������ � ������
{
	showDeliveryHeader("���� ��������:");

	for (int i = 0; i < this->get_count(); ++i) {
		(this + i)->showOneRecord(i + 1);
	}
	std::cout << std::endl << "=========================================================================================================================" << std::endl << std::endl;
}// showAllRecords()

void showDeliveryHeader(const char* title) //��������������� ������� - ������ ��������� �������
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
	std::cout.width(18);
	std::cout << "���� ��������";
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
}

void delivery::showOneRecord(int number = 1) //��������������� ������� - ������ ����� ������
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

delivery* delivery::loadRecords() //�������� ������ �� ����� � ����� �� �� ����� � �������� � ������ ��� ������
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
		std::cout << "�������� ���� ��������:" << std::endl;

		// ���� ��� �������� �������� �� �����

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

void delivery::exitAndSave() // ������ ���� �� ������ � ���� ��� ������
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
