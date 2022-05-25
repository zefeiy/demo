#include<iostream>
using namespace std;

bool fiebo_elem(int pos, unsigned int& elem);

int main() {
	int pos;
	cout << "input pos:\n";
	cin >> pos;
	while (pos !=-1) {
		
		unsigned int elem = 0;
		if (fiebo_elem(pos, elem)) {
			cout << "index" << pos << "result is:" << elem << endl;
			cout << "input pos:\n";
			cin >> pos;
		}
		else {
			cout << "pos is error\n";
			cout << "input pos:\n";
			cin >> pos;
		}
	}
}
bool fiebo_elem(int pos,unsigned int& elem) {
	
	int n1 = 1, n2 = 1;
	elem = 1;
	if (pos > 1024 || pos <= 0) {
		return false;
	}
	for (int i = 3; i <= pos; i++) {
		elem = n1 + n2;
		n1 = n2;
		n2 = elem;
	}
	return true;

}