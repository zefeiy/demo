#pragma once
#include<string>
#include<vector>
#define max_size 5
using namespace std;
class Stack {
public:
	Stack() {};

	bool push(const string&);
	bool pop();
	string peek() const;
	bool empty() const {return _stack.empty();}
	bool full() const {return _stack.size() == max_size;}
	int size() const { return _stack.size(); }

	bool find(const string&) const;
	int count(const string&) const;
		
private:
	vector<string> _stack;
};
