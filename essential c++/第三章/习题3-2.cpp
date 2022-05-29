#include<iostream>
#include<iterator>
#include<string>
#include<algorithm>
#include<vector>
#include<fstream>
using namespace std;


int main() {
	ifstream in_file("Text.txt");

	istream_iterator<string> is(in_file);
	istream_iterator<string> eof;
	vector<string> vec;
	copy(is,eof,back_inserter(vec));
	
	sort(vec.begin(), vec.end(), [](const string& a, const string& b) {
		return a.size() < b.size();
	});

	for (vector<string>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		cout << *it << " " << endl;
	}

	return 0;
}