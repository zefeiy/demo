#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>


using namespace std;
int main() {
	
	ifstream infile("Text.txt");
	ofstream outfile("sort_test.txt");
	
	if (!infile || !outfile) {
		cout << "Open file error\n";
		return -1;
	}
	string words;
	vector<string> vec;
	while (infile >> words)
		vec.push_back(words);

	for (int i = 0; i < vec.size(); i++)
		cout << vec[i] << endl;
	cout << vec[0];

	sort(vec.begin(), vec.end());

	for (int i = 0; i < vec.size(); i++)
		outfile << vec[i] << endl;

	return 0;
	
}