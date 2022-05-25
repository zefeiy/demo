#include<iostream>
#include<string>
using namespace std;

/*
int main() {
	char name[10];
	cout << "输入姓名" << endl;
	cin >> name;
	if (strlen(name) <= 2)
		cout << "名字无效"<<strlen(name) << endl;
	else {
		cout << "ok" << endl;
		cout << strlen(name) << endl;
	}
}
*/

int main() {
	string s;
	cout << "输入姓名" << endl;
	cin >> s;
	if(s.length()<=2)
		cout << "名字无效" << s.size()<<s.length()<< endl;
	else {
		cout << "ok" << endl;
		cout << s.size() << endl;
	}
}