#include<iostream>
#include<string>
#include<vector>
using namespace std;

int max(int a, int b);
double max(double a, double b);
string& max(const string& a, const string& b);
int max(const vector<int>& ary);
double max(const vector<double>& ary)



int max(int a,int b) {
	return a > b ? a : b;
}
 
double max(double a, double b) {
	cout << 123;
	return a > b ? a : b;
}

string& max(string& a, string& b) {
	return a.size() > b.size() ? a : b;
}

int max(const vector<int>& ary) {
	int m = ary[0];
	for (int i = 1; i < ary.size(); i++) {
		if (ary[i] > m)
			m = ary[i];
	}
	return m;
}

double max(const vector<double>& ary) {
	int m = ary[0];
	for (int i = 1; i < ary.size(); i++) {
		if (ary[i] > m)
			m = ary[i];
	}
	return m;
}




int main() {
	int a = 10, b = 20;
	double da = 10, db = 20;
	//cout << max(a, b);
	//cout << max(da, db);


}




