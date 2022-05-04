#include<iostream>
#include<string>
#include<vector>
using namespace std;


template<typename T>
class Myarray {
public:

	Myarray(int capacity);

	Myarray(Myarray&);

	Myarray& operator=(const Myarray&);

	T& operator[](int index);

	void push_back(T x);

	int getcapacity();

	int getsize();

	~Myarray() {
		if (this->paddress) {
			delete[] this->paddress;
			this->paddress = NULL;
		}
	}

private:
	int m_size;

	T* paddress;

	int m_capacity;
};



template<typename T>
Myarray<T>::Myarray(int capacity) {
	this->m_size = 0;
	this->m_capacity = capacity;
	this->paddress = new T[capacity];
}

template<typename T>
Myarray<T>::Myarray(Myarray& ary) {

	this->m_size = ary.m_size;
	this->m_capacity = ary.m_capacity;
	this->paddress = new T[ary.m_capacity];
	for (int i = 0; i < ary.m_size; i++) {
		this->paddress[i] = ary.paddress[i];
	}
}

template<typename T>
Myarray<T>& Myarray<T>::operator=(const Myarray &ary) {
	if (this->paddress) {
		delete[] this->paddress;
		this->paddress = NULL;
	}
	this->m_size = ary.m_size;
	this->m_capacity = ary.m_capacity;
	this->paddress = new T[ary.m_capacity];
	for (int i = 0; i < ary.m_size; i++) {
		this->paddress[i] = ary.paddress[i];
	}

}

template<typename T>
T& Myarray<T>::operator[](int index) {
	return this->paddress[index];
}

template<typename T>
void Myarray<T>::push_back(T x) {
	this->paddress[this->m_size] = x;
	this->m_size++;
}

template<typename T>
int Myarray<T>::getcapacity() {
	return this->m_capacity;
}

template<typename T>
int Myarray<T>::getsize() {
	return this->m_size;
}

int main() {
	 
	Myarray<int> a(10);
	a.push_back(20);
	a.push_back(20.40);
	a[1] = 50;
	std::cout << a[1];
	return 0;
} 
