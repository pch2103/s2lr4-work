#pragma once
#include <cstring>
#include <sstream>
#include <iomanip>
#include "common.h"
#include "fio.h"

#define LETTER_BASE_NAME "letterBase.txt" //файл базы писем

class letter :
	public common
{
	double letterPrice; //Стоимость письма
	static int count;
public:
	letter() : common(), letterPrice(0.0) //конструктор по-умолчанию
	{
		std::cout << "letter::Kонструктор по-умолчанию" << std::endl;
	}

	//конструктор c параметрами
	letter(fio _fio, const char* _inAddress, const char* _outAddress, double _letterPrice = 0.0) :
		common(_fio, _inAddress, _outAddress),
		letterPrice(_letterPrice)
	{
		std::cout << "letter::Kонструктор c параметрами" << std::endl;
	}

	//Kонструктор Копирования
	letter(letter& _L) : 
		common(_L.getFio(), _L.getInAddress(), _L.getOutAddress()),
		letterPrice(_L.letterPrice)
	{
		std::cout << "letter::Kонструктор Копирования" << std::endl;
	}

	letter& operator= (const letter &_L) // Перегрузка оператора "="
	{
		if (this == &_L) return *this; //самоприсваивание
		std::cout << "letter::Перегрузка оператора =" << std::endl;
		this->setFio(_L.getFio());
		this->setInAddress(_L.getInAddress());
		this->setOutAddress(_L.getOutAddress());
		letterPrice = _L.letterPrice;

		return *this;
	}

	~letter() {
		std::cout << "~letter::Деструктор" << std::endl;
	}

	int get_count() { return count; } //получить количество записей в памяти
	void add_count() { ++count; } //увеличить количество записей в памяти на 1 (при добавлении записи)
	void enterNewRecord(); //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
	letter* addNewRecord(letter*);//выделение памяти под новую структу и заполнение ее данными
	void showAllRecords(); //показать все записи в базе писем
	void showOneRecord(int); //показывает одну запись в памяти 
	void exitAndSave();  // запись данных из памяти на диск при выходе
	letter* loadRecords(); //загрузка данных из файла и вывод их на экран и загрузка в пямять для работы
}; //class letter

void showRecordHeader(const char*);
int letter::count = 0;


//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// РЕАЛИЗАЦИЯ МЕТОДОВ
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

//ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
void letter::enterNewRecord()
{
	char _inName[MAX_WIDTH], _inAddress[ONE_STRING_WITH], _outAddress[ONE_STRING_WITH];
	double _letterPrice;

	std::cout << "=============================================" << std::endl;
	std::cout << "Письма: Добавление новой записи в базу писем:" << std::endl;
	std::cout << "=============================================" << std::endl;

	std::cout << "ФИО отправителя: ";
	std::cin.getline(_inName, MAX_WIDTH);
	this->setFio(fio(_inName));

	std::cout << "Адрес получателя: ";
	std::cin.getline(_inAddress, ONE_STRING_WITH);
	this->setInAddress(_inAddress);

	std::cout << "Адрес oтправителя: ";
	std::cin.getline(_outAddress, ONE_STRING_WITH);
	this->setOutAddress(_outAddress);

	//защита от неверного ввода https://ravesli.com/urok-72-obrabotka-nekorrektnogo-vvoda-cherez-std-cin/#toc-2
	do {
		std::cout << "Стоимость письма: ";
		std::cin >> _letterPrice;

		if (std::cin.fail()) // если предыдущее извлечение было неудачным,
		{
			std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
			std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
		}
		else break;
	} while (true);

	this->letterPrice = _letterPrice;


} // enterNewRecord()

letter* letter::addNewRecord(letter* BS) //выделение памяти под новую структу и заполнение ее данными
{
	int amount = this->get_count();

	if (amount == 0)
	{
		BS = new letter(this->getFio(), this->getInAddress(), this->getOutAddress(), this->letterPrice);
	}
	else
	{
		letter *T = new letter[amount+1];

		for (int i = 0; i < amount; ++i)  // копируем во временный объект
		{
			T[i] = BS[i];
			BS[i].~letter(); // Удаляем ранние записи из памяти - вызываем деструктор явно для каждой записи
		}
		T[amount] =  letter(this->getFio(), this->getInAddress(), this->getOutAddress(), this->letterPrice); //Добавляем новую запись в память
		BS = T; //временный объект становится "присланным"
	}

	this->add_count(); // Увеличиваем счетчик (static-переменная)
	
	return BS;

} // addNewRecord()

void letter::showAllRecords() //показывает все записи в памяти
{
	showRecordHeader("БАЗА ПИСЕМ:");

	for (int i = 0; i < this->get_count(); ++i) {
		(this+i)->showOneRecord(i + 1);
	}
	std::cout << std::endl << "=========================================================================================================================" << std::endl << std::endl;
}// showAllRecords()

void showRecordHeader(const char* title) //вспомогательная функция - печать заголовка таблицы
{
	std::cout << std::endl << "=========================================================================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "=========================================================================================================================" << std::endl << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(4);
	std::cout << " №";
	std::cout.setf(std::ios::left);
	std::cout.width(MAX_WIDTH);
	std::cout << "ФИО получателя";
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

void letter::showOneRecord(int number = 1) //вспомогательная функция - печать одной записи
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
	// Вывод с определенной точностью
	//https://stackoverflow.com/questions/554063/how-do-i-print-a-double-value-with-full-precision-using-cout
	std::cout << std::fixed << std::setprecision(2) << this->letterPrice;
	std::cout << std::endl;
}

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

	FILE* FP = fopen(LETTER_BASE_NAME, "r");

	if (FP != NULL) {
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "Загрузка базы писем:" << std::endl;

		// Ниже все читается строками из файла, затем цена преобразовывается в double с помощю std::stod()

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

	FILE* FP = fopen(LETTER_BASE_NAME, "w");

	for (int i = 0; i < amount; ++i) {
		fprintf(FP, "%s\t%s\t%s\t%.2f\n", (this + i)->getFio().convertToString(), (this + i)->getInAddress(), (this + i)->getOutAddress() , (this + i)->letterPrice);
	}

	if (FP != NULL)
	{
		fclose(FP);
	}
} //exitAndSave()