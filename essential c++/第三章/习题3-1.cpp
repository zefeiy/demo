#include<iostream>
#include<iterator>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include<map>
#include<set>

using namespace std;
int main() {
	/*
	ifstream in_file("Text.txt");
	ofstream out_file("sort_test.txt");

	istream_iterator<string> is(in_file);
	istream_iterator<string> eof;

	vector<string> text;
	copy(is, eof, back_inserter(text));
	sort(text.begin(), text.end());

	ostream_iterator<string> os(out_file, " ");
	copy(text.begin(), text.end(), os);
	*/

	set<string> set({"a","an","or","the","and","but"});
	map<string, int> mp;
	ifstream in_file("Text.txt");
	istream_iterator<string> is(in_file);
	istream_iterator<string> eof;
	
	while (is != eof) {
		if (!set.count(*is)) {
			map<string, int>::iterator it;
			it = mp.find(*is);
			if (it == mp.end())
				mp[*is]++;
			else {
				it->second++;
			}
		}
		is++;
	}

	for (auto it = mp.begin(); it != mp.end(); ++it) {
		//fprintf(stdout, it->first + "");
		cout << it->first << ":" << it->second << endl;
	}

	return 0;
}