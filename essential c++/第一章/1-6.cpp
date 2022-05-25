#include<iostream>
#include<vector>
using namespace std;


int main() {
	int num;
	vector<int> vec;
	while (cin>>num) {
		vec.push_back(num);
	}
	int sum = 0;
	for (int i = 0; i < vec.size(); i++)
		sum += vec[i];
	cout << "sum = " << sum << endl;

	return 0;
}
