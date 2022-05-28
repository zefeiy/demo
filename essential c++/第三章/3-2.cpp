
#include<iterator>
#include<vector>
#include<string>
#include<iostream>
#include<list>
#include<functional>
#include<algorithm>
using namespace std;
/*
vector<int> filter(const vector<int>& vec,int val,const less<int>& lt) {
	vector<int> nvec;
	vector<int>::const_iterator iter = vec.begin();

	while ((iter = find_if(iter,vec.end(),bind2nd(lt,val)))!=vec.end()) {
		nvec.push_back(*iter);
		iter++;
	}
	return nvec;
}
*/

template<typename InputIterator,typename OutputIterator,typename ElemType,typename Comp>
OutputIterator filter_t(InputIterator first, InputIterator last, OutputIterator at,
	const ElemType& val, Comp pred) {
	while ((first = find_if(first, last, bind2nd(pred, val))) != last) {
		cout << "found value:" << *first << endl;
		*at++ = *first++;
	}
	return at;
}

template<typename InputIterator, typename OutputIterator, typename ElemType>
OutputIterator filter_t2(InputIterator first, InputIterator last, OutputIterator at,
	const ElemType& val, bool (*pred)(ElemType, ElemType)) {
	while (first!=last) {
		if (pred(*first, val)) {
			cout << "found value:" << *first << endl;
			*at++ = *first++;
		}
		first++;
	}
	return at;
}


bool pred(const int a, const int b) {
	return a > b;
}



int main() {
	int ary[8] = {12,8,43,0,6,21,3,7 };
	vector<int> vec(ary, ary +8);

	int ary2[8];
	vector<int> vec2(8);
	//函数指针做法
	//cout << "filtering_point integer array for values less than 8\n";
	//filter_t2(ary, ary + 8, ary2, 8, pred);

	//利用functional 库中的  templa function object 做法
	cout << "filtering integer array for values less than 8\n";
	filter_t(ary, ary + 8, ary2, 8, less<int>());


	cout << "filtering integer vector for values greater than 8\n";
	//filter_t(vec.begin(),vec.end(),vec2.begin(),8,greater<int>());
	filter_t(vec.begin(), vec.end(),back_inserter(vec2), 8, greater<int>());

	//在查找前先经过greater<int>()  对象排序(大->小)
	//binary_search(vec.begin(),vec.end(),10,greater<int>()); 

	//vec = filter(vec,8,less<int>());
	//sort(vec.begin(), vec.end(), greater<int>());
	for (vector<int>::iterator iter = vec.begin(); iter != vec.end(); iter++) {
		//cout << *iter << " ";
	}
	
	return 0;
}
