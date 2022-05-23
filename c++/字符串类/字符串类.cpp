#include"字符串类.h"
#include<string.h>
//重载左移运算符 
std::ostream& operator<<(std::ostream &cout,mystring &str) {
	std::cout << str.pstring;
	return cout;
}

std::istream& operator>>(std::istream& cin,mystring &str) {
	if (str.pstring) {
		delete[] str.pstring;
		str.pstring = NULL;
	}
	
	//char *temp = NULL;
	char temp[1024]; 
	std::cin >> temp;
	str.pstring = new char[strlen(temp) + 1];
	strcpy(str.pstring, temp);
	str.m_size = strlen(temp);
	
	return cin;
}

char& mystring::operator[](int index) {  //这里没有做越界的判断
	return this->pstring[index];
}



mystring& mystring::operator=(const char* str) {
	if (this->pstring) {
		delete[] this->pstring;
		this->pstring = NULL;
	}

	this->pstring = new char[strlen(str) + 1];
	strcpy(this->pstring, str);
	this->m_size = strlen(str);

	return *this;
}

mystring& mystring::operator=(const mystring& temp) {
	if (this->pstring) {
		delete[] this->pstring;
		this->pstring = NULL;
	}
	this->pstring = new char[strlen(temp.pstring) + 1];
	strcpy(this->pstring, temp.pstring);
	this->m_size = temp.m_size;

	return *this;
}

mystring mystring::operator+(const mystring& temp) {
	int newsize = this->m_size + temp.m_size;
	char *t = new char[newsize+1];
	memset(t, 0, newsize + 1);
	strcat(t, this->pstring);
	strcat(t, temp.pstring);

	mystring newstring = t;
	delete[] t;
	t = NULL;
	return newstring;
}


mystring mystring::operator+(const char* str) {
	int newsize = this->m_size + strlen(str);
	char* t = new char[newsize + 1];
	memset(t, 0, newsize + 1);
	strcat(t, this->pstring);
	strcat(t, str);

	mystring newstring = t;

	delete[] t;
	t = NULL;
	return newstring;

}



bool mystring::operator==(const mystring& temp) {
	if (strcmp(this->pstring, temp.pstring)==0) {
		return true;
	}

	return false;
}


bool mystring::operator==(const char* str) {
	if (strcmp(this->pstring,str)==0) {
		return true;
	}

	return false;
}


mystring::mystring(char *str) {
	//std::cout << "构造函数调用" << std::endl;
	this->pstring = new char[strlen(str)+1];
	strcpy(this->pstring, str); //结尾一样拷贝
	this->m_size = strlen(str);
	 
}


mystring::mystring(const mystring& str) {
	//std::cout << "拷贝构造函数调用" << std::endl;
	this->pstring = new char[strlen(str.pstring)+1];
	strcpy(this->pstring, str.pstring);
	this->m_size = str.m_size;

}

mystring::~mystring() {
	//std::cout << "析构函数调用" << std::endl;
	if (this->pstring != NULL) {
		delete[] this->pstring;
		this->pstring = NULL;
	}
}
