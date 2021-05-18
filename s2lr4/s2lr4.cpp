#include <iostream>
#include <Windows.h>

#include "fio.h"
#include "letter.h"
#include "delivery.h"

#pragma warning (disable : 4996) //Без этого ругается на fopen и strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define ONE_STRING_WITH 20
#define MAX_WIDTH 60


//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// ПРОТОТИПЫ МЕТОДОВ
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

char menu(); //Меню функций базы


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char choice;

	letter* tmpLetter = new letter(); //временная переменная - указатель на письмо
	letter* baseL = nullptr; //база писем в памяти и доставок
	delivery* tmpDelivery = new delivery(); //временная переменная - указатель на письмо
	delivery* baseD = nullptr; //временная переменная - указатель на доставку

	baseL = baseL->loadRecords(); //чтение данных из файла писем в память
	baseD = baseD->loadRecords(); //чтение данных из файла доставок в память

	do {
		choice = menu();

		switch (choice)
		{
		case '1': //1 - Добавить новое письмо в базу (производный от common class letter)
			tmpLetter->enterNewRecord();
			baseL = tmpLetter->addNewRecord(baseL);
			break;
		case '2': //1 - Добавить новое письмо в базу (производный от common class delivery)
			tmpDelivery->enterNewRecord();
			baseD = tmpDelivery->addNewRecord(baseD);
			break;
		case '3': //2 - Показать все записи 
			if (baseL != nullptr) baseL->showAllRecords();
			if (baseD != nullptr) baseD->showAllRecords();
			break;
	//	case '5': //3 -  Найти все письма определенного получателя
	//		myLetter->inNameSearch();
	//		break;
	//	case '5': //4 -  Сортировка писем по увелечению стоимости
	//		myLetter = priceSort(myLetter);
	//		showAllRecords(myLetter);
	//		break;
	//	case '6': //5 -  Сортировка писем по алфавиту - адресу отправителя (перегрузка оператора !)
	//		!(*myLetter);
	//		showAllRecords(myLetter);
	//		break;

		default:
			break;
		}
	} while (choice != '0');

	if (baseL != nullptr) baseL->exitAndSave(); // запись базы писем из памяти на диск при выходе
	if (baseD != nullptr) baseD->exitAndSave(); // запись базы доставок из памяти на диск при выходе
}

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// РЕАЛИЗАЦИЯ МЕТОДОВ
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

char menu() //Меню функций базы
{
	char select;
	std::cout << "==============================================================" << std::endl;
	std::cout << "Меню:" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << "1 - Письма: Добавить новую запись в базу писем" << std::endl;
	std::cout << "2 - Доставка: Добавить новую запись в базу доставок" << std::endl;
	std::cout << "3 - Pаспечатка всех данных (писем и доставок) в табличном виде" << std::endl;
	std::cout << "4 - Найти все письма определенного получателя" << std::endl;
	std::cout << "5 - Сортировать писем по увелечению стоимости" << std::endl;
	std::cout << "6 - Сортировка в алфавитном порядке (по адресу отправителя)" << std::endl;
	std::cout << "0 - Выход из программы и запись данных на диск." << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;

	do
	{
		std::cout << "Введите номер пункта меню: ";
		std::cin >> select;
		std::cin.ignore(32767, '\n'); // https://ravesli.com/urok-72-obrabotka-nekorrektnogo-vvoda-cherez-std-cin/

	} while (select < '0' || select > '6');

	return select;
} // menu()


