#include <iostream>
#include <Windows.h>

#include "fio.h"
#include "letter.h"
#include "delivery.h"

#pragma warning (disable : 4996) //��� ����� �������� �� fopen � strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define ONE_STRING_WITH 20
#define MAX_WIDTH 60


//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// ��������� �������
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

char menu(); //���� ������� ����


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char choice;

	letter* tmpLetter = new letter(); //��������� ���������� - ��������� �� ������
	letter* baseL = nullptr; //���� ����� � ������ � ��������
	delivery* tmpDelivery = new delivery(); //��������� ���������� - ��������� �� ������
	delivery* baseD = nullptr; //��������� ���������� - ��������� �� ��������

	baseL = baseL->loadRecords(); //������ ������ �� ����� ����� � ������
	baseD = baseD->loadRecords(); //������ ������ �� ����� �������� � ������

	do {
		choice = menu();

		switch (choice)
		{
		case '1': //1 - �������� ����� ������ � ���� (����������� �� common class letter)
			tmpLetter->enterNewRecord();
			baseL = tmpLetter->addNewRecord(baseL);
			break;
		case '2': //1 - �������� ����� ������ � ���� (����������� �� common class delivery)
			tmpDelivery->enterNewRecord();
			baseD = tmpDelivery->addNewRecord(baseD);
			break;
		case '3': //2 - �������� ��� ������ 
			if (baseL != nullptr) baseL->showAllRecords();
			if (baseD != nullptr) baseD->showAllRecords();
			break;
	//	case '5': //3 -  ����� ��� ������ ������������� ����������
	//		myLetter->inNameSearch();
	//		break;
	//	case '5': //4 -  ���������� ����� �� ���������� ���������
	//		myLetter = priceSort(myLetter);
	//		showAllRecords(myLetter);
	//		break;
	//	case '6': //5 -  ���������� ����� �� �������� - ������ ����������� (���������� ��������� !)
	//		!(*myLetter);
	//		showAllRecords(myLetter);
	//		break;

		default:
			break;
		}
	} while (choice != '0');

	if (baseL != nullptr) baseL->exitAndSave(); // ������ ���� ����� �� ������ �� ���� ��� ������
	if (baseD != nullptr) baseD->exitAndSave(); // ������ ���� �������� �� ������ �� ���� ��� ������
}

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// ���������� �������
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

char menu() //���� ������� ����
{
	char select;
	std::cout << "==============================================================" << std::endl;
	std::cout << "����:" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << "1 - ������: �������� ����� ������ � ���� �����" << std::endl;
	std::cout << "2 - ��������: �������� ����� ������ � ���� ��������" << std::endl;
	std::cout << "3 - P��������� ���� ������ (����� � ��������) � ��������� ����" << std::endl;
	std::cout << "4 - ����� ��� ������ ������������� ����������" << std::endl;
	std::cout << "5 - ����������� ����� �� ���������� ���������" << std::endl;
	std::cout << "6 - ���������� � ���������� ������� (�� ������ �����������)" << std::endl;
	std::cout << "0 - ����� �� ��������� � ������ ������ �� ����." << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;

	do
	{
		std::cout << "������� ����� ������ ����: ";
		std::cin >> select;
		std::cin.ignore(32767, '\n'); // https://ravesli.com/urok-72-obrabotka-nekorrektnogo-vvoda-cherez-std-cin/

	} while (select < '0' || select > '6');

	return select;
} // menu()


