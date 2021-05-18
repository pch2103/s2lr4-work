#pragma once

#include "fio.h"

class common
{
	fio inName; //��� ����������
	char* inAddress; //����� ����������
	char* outAddress; //����� �����������

public:
	common() : inName() //����������� ��-���������
	{
		std::cout << "common::K���������� ��-���������" << std::endl;

		inAddress = new char[ONE_STRING_WITH];
		strcpy(inAddress, "");
		outAddress = new char[ONE_STRING_WITH];
		strcpy(outAddress, "");
	}

	//����������� c 3 �����������
	common(fio _fio, const char* _inAddress, const char* _outAddress) :
		inAddress(new char[strlen(_inAddress) + 1]),
		outAddress(new char[strlen(_outAddress) + 1]),
		inName(_fio)
	{
		std::cout << "common::K���������� c 3 �����������" << std::endl;
		strcpy(inAddress, _inAddress);
		strcpy(outAddress, _outAddress);
	}

	//����������� c 2 �����������
	common(fio _fio, const char* _inAddress) :
		inAddress(new char[strlen(_inAddress) + 1]),
		outAddress(new char[ONE_STRING_WITH]),
		inName(_fio)
	{
		std::cout << "common::K���������� c 2 �����������" << std::endl;
		strcpy(inAddress, _inAddress);
		strcpy(outAddress, "");
	}

	virtual ~common() {
		std::cout << "~common::����������� ����������" << std::endl;
		delete[] inAddress;
		delete[] outAddress;
	}
	
	//������� ���������� �������� ������
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

	//������� ���������� �������� ������
	fio getFio() const { return fio(inName); }
	char* getInAddress() const { return inAddress; }
	char* getOutAddress() const { return outAddress; }

	virtual void enterNewRecord() = 0; //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
	virtual int get_count() = 0; //�������� ���������� ������� � ������
	virtual void add_count() = 0; //��������� ���������� ������� � ������ �� 1 (��� ���������� ������)
	virtual void showOneRecord(int) = 0; //���������� ���� ������ � ������
    virtual void showAllRecords() = 0; //�������� ��� ������ � ���� �����
	virtual void exitAndSave() = 0;  // ������ ������ �� ������ �� ���� ��� ������
};

