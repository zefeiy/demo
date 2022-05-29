#include<iostream>
#include<iterator>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;
int main() {
	istream_iterator<int> is(cin);
	istream_iterator<int> eof;
	fstream out_file1("even_number.txt");
	fstream out_file2("sort_test.txt");
	if (!out_file1 || !out_file2) {
		cerr << "open file error\n";
		return -1;
	}
	vector<int> even_number;
	vector<int> odd_number;
	vector<int> input;

	copy(is, eof, back_inserter(input));

	vector<int>::iterator ite = input.begin();
	while (ite!=input.end()) {
		if (*ite % 2 == 0)
			even_number.push_back(*ite);
		else odd_number.push_back(*ite);
		ite++;
	}

	ostream_iterator<int> os_even(out_file1," "), os_odd(out_file2, " ");
	copy(even_number.begin(), even_number.end(), os_even);
	copy(odd_number.begin(), odd_number.end(), os_odd);

	return 0;
}