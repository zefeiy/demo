#include<iostream>
#include<vector>
#include<string>
using namespace std;

void Pentagonal(int pos,vector<int>& ary) {
	//static vector<int> ary;
	for (int i = ary.size()+1; i <= pos; i++) {
		ary.push_back(i*(3*i-1)/2);
	}
}

inline bool is_ok(int pos,vector<int>& ary) {
	if (pos >= 64 || pos <= 0 || pos<ary.size())
		return false;
	Pentagonal(pos,ary);
	return true;
}


void printve(const vector<int>& ary,const string& str) {
	cout << "print everyone element\n";
	for (int i = 0; i < ary.size(); i++) {
		cout << ary[i] << " ";
	}
	cout << endl;

}

int main() {
	vector<int> ary;
	const string ty("element type");
	if (is_ok(5, ary))
		printve(ary, ty);
		//Pentagonal(5,ary);
	return 0;
}