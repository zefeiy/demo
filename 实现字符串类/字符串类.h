 #pragma once
#include<iostream>

class mystring {
public:

	friend std::ostream& operator<<(std::ostream &cout, mystring &str);   //�������������

	friend std::istream& operator>>(std::istream& cin, mystring &str);		//�������������

	char& operator[](int);   //����[]�����

	mystring& operator=(const char*);  

	mystring& operator=(const mystring&);

	mystring operator+(const mystring&);

	mystring operator+(const char*);


	bool operator==(const mystring&);

	bool operator==(const char*);

	mystring(char *str);

	mystring(const mystring &str);

	~mystring();

private:

	char *pstring;
	
	int m_size; //�ַ������Ȳ����� \0;

};
