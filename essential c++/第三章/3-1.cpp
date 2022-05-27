#include<vector>
#include<string>
#include<iostream>
#include<list>
using namespace std;

template<typename T,typename elemType>
T* find( T const* first, T const* last,const elemType& value) {  //我不希望值被修改,只读操作

	for (; first != last; first++) {   //当对象不支持 == 运算符时。如何解决
		if (*first == value)
			return first;
	}
	return last;
}


int main() {
	int array[8] = { 1,1,2,3,5,8,13,21 };
	vector<int> ivec(array, array + 8);
	list<int> ilist(array, array + 8);

	int* p = find(array, array + 8, 5);
	if (p != array + 8)
		cout << "success find it\n";
	else cout << "no find\n";

	vector<int>::iterator iter;
	iter = find(ivec.begin(), ivec.end(), 5);
	if(iter!=ivec.end())
		cout << "success find it\n";
	else cout << "no find\n";
	

	list<int>::const_iterator it_list; //常量迭代器，不可做写入操作
	it_list = find(ilist.begin(), ilist.end(), 5);
	if (it_list != ilist.end())
		cout << "success find it\n";
	else cout << "no find\n";

	return 0;
}
