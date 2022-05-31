#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;


void print_less_than(const vector<int>& vec, int comp, ostream & os=cout);

class Lessthan {
public:
	Lessthan(int val) :_val(val){}
	int comp_val() const { return _val; }
	void comp_val(int nval) { _val = nval; }
	
	bool operator()(int _value) const;
private:
	int _val;
};
inline bool Lessthan::operator()(int value) const {
	return value < _val;
}


int count_less_than(const vector<int>& vec,int comp) {
	Lessthan It(comp);

	int count = 0;
	for (int ix = 0; ix < vec.size(); ix++) {
		if (It(vec[ix]))
			++count;
	}
	return count;
}


void print_less_than(const vector<int>& v, int comp, ostream &os) {
	Lessthan it(comp);
	vector<int>::const_iterator iter = v.begin();
	vector<int>::const_iterator it_end = v.end();
	os << "elements less than" << it.comp_val() << endl;
	while ((iter = ::find_if(iter,it_end,it)) != it_end) {
		os << *iter++ << " ";
	}
}

int main() {
	int ia[8] = { 17,12,44,9,18,45,6,14 };
	vector<int> ary(ia, ia + 8);
	int comp_val = 20;
	cout << "Numbnr of elements less than"
		<< comp_val << "are"
		<< count_less_than(ary, comp_val) << endl;
	
	print_less_than(ary,comp_val);
}