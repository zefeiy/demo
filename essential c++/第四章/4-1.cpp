#include<iostream>
#include<algorithm>
#include<iterator>
#include"4-1.h"

using namespace std;

bool Stack::push(const string& elem) {
	if (full()) return false;
	this->_stack.push_back(elem);
	return true;
}

bool Stack::pop() {
	if (empty()) return false;
	this->_stack.pop_back();
	return true;
}

string Stack::peek() const {
	if (empty()) return NULL;
	return this->_stack.back();
}

bool Stack::find(const string& elem) const {
	vector<string>:: const_iterator it = _stack.begin();
	return ::find(it,this->_stack.end(),elem)!=this->_stack.end();
	return true;
}

int Stack::count(const string& elem) const {
	return ::count(_stack.begin(), _stack.end(), elem);

}

int main() {
	Stack st;
	string str;
	while (cin>>str && !st.full()) {
		st.push(str);
	}
	cout << '\n' << "Read in" << st.size() << "strings!\n";
	cin.clear();
	cout << "what word to search for?:";
	cin >> str;
	bool found = st.find(str);
	int count = found ? st.count(str) : 0;
	cout << str << (found ? "  is" : "   isn\'t") << "   in the stack\n";
	if (found)
		cout << "It occurs  " << count << "   times\n";

	return 0;
}