#include<iostream>
#include<vector>
#include<string>
using namespace std;

bool Pentagonal(int pos,vector<int>& ary) {
	//static vector<int> ary;
	if (pos >= 64 || pos <= 0)
		return false;
	for (int i = ary.size()+1; i <= pos; i++) {
		ary.push_back(i*(3*i-1)/2);
	}

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
	if(Pentagonal(10, ary))
		printve(ary,ty);
	return 0;
}