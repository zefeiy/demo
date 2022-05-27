#include<iostream>
#include<string>
#include<vector>
using namespace std;

template<typename T>
T max(T& a,T& b) {
	return a > b ? a : b;
}

template<typename T>
T max(T* a, T& b) {
	T m = a[0];
	for (int i = 0; i < b; i++) {
		if (a[i] > m) m = a[i];
	}
	return m;
}

template<typename T>
T max(vector<T>& ary) {
	T m = ary[0];
	for (int i = 0; i < ary.size(); i++) {
		if (ary[i] > m) m = ary[i];
	}
	return m;
}


int main() {
	string a = "";
	string b = "sadas";
	int ary[10] = { 2,3,4 };
	int len=10;
	vector<int> bary(ary, ary + 3);
	//cout << max(ary, len);
	cout << max(bary);
	return 0;
}