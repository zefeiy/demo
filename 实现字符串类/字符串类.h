 #pragma once
#include<iostream>

class mystring {
public:

	friend std::ostream& operator<<(std::ostream &cout, mystring &str);   //重载左移运算符

	friend std::istream& operator>>(std::istream& cin, mystring &str);		//重载右移运算符

	char& operator[](int);   //重载[]运算符

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
	
	int m_size; //字符串长度不包括 \0;

};
