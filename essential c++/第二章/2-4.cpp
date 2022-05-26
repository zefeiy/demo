#include<iostream>
#include<vector>
using namespace std;


const vector<int>* Pentagonal(int pos) {
	static vector<int> ary;
	if (pos < 0)
		return NULL;
	for (int i = ary.size() + 1; i <= pos; i++) {
		ary.push_back(i*(3 * i - 1) / 2);
	}
	return &ary;
}

int find_element(int pos, vector<int> const *ary) {
	if (ary == NULL || pos > ary->size())
		return -1;
	return (*ary)[pos];
}


int main() {
	const vector<int>* ary = Pentagonal(10);
	cout << find_element(5, ary);


}