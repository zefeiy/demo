#include<iostream>
#include<string>
#include<vector>
#include<map>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

class UserProfile {
public:
	enum uLevel { A, B, C, D };
	UserProfile(string login, uLevel = A);
	UserProfile();

	bool operator==(const UserProfile&);
	bool operator!=(const UserProfile&);

	string login() const { return this->_login; }
	string user_name()const { return this->_user_name; }
	int login_count() const { return this->_times_logged; }
	int guess_count() const { return this->_guesses; }
	int guess_correct() const { return this->_correct_guesses; }
	double guess_average() const;   //正确率
	string level() const;

	void reset_login(const string& val) { _login = val; }
	void user_name(const string& val){_user_name = val;}
	void reset_level(const string&);
	void reset_level(uLevel newlevel) { _user_level = newlevel; }
	void reset_login_count(int val) { _times_logged = val; }
	void reset_guess_count(int val) { _guesses = val; }
	void reset_guess_correct(int val) { _correct_guesses = val; }
	void bump_login_count(int cnt = 1) { _times_logged += cnt; }
	void bump_guess_count(int cnt = 1) { _guesses += cnt; }
	void bump_guess_correct(int cnt = 1) { _correct_guesses += cnt; }

private:
	string _login;   //登录名
	string _user_name;   //用户名   
	int _times_logged;     //登录次数
	int _guesses; //总次数
	int _correct_guesses;  //正确的次数
	uLevel _user_level;   // 等级

	static map<string, uLevel> _level_map;
	static void init_level_map();
	//static string guess_login();
};

inline double UserProfile::guess_average() const {
	return _guesses ? double(_correct_guesses) / double(_guesses) * 100 : 0.0;
}
inline UserProfile::UserProfile(string login, uLevel level)
:_login(login),_user_level(level),_times_logged(1),_guesses(0),_correct_guesses(0){}

inline UserProfile::UserProfile()
	:_login("guest"), _user_level(A), _times_logged(1), _guesses(0), _correct_guesses(0) {
	static int id = 0;
	char buffer[16];
	_itoa(id++, buffer, 10);
	_login += buffer;
}
inline bool UserProfile::operator==(const UserProfile& it) {
	if (this->_login == it._login && this->_user_name == it._user_name)
		return true;
	return false;
}
inline bool UserProfile::operator!=(const UserProfile& it) { return !(*this == it);}

inline string UserProfile::level() const {
	static string _level_table[] = {
		"A","B","C","D"
	};
	return _level_table[_user_level];
}
 
ostream& operator<<(ostream& os,const UserProfile& rhs) {
	os << rhs.login() << ' '
		<< rhs.level() << ' '
		<< rhs.login_count() << ' '
		<< rhs.guess_count() << ' '
		<< rhs.guess_correct() << ' '
		<< rhs.guess_average() << endl;
	return os;
}

map<string, UserProfile::uLevel> UserProfile::_level_map;
void UserProfile::init_level_map() {
	_level_map["A"] = A;
	_level_map["B"] = B;
	_level_map["C"] = C;
	_level_map["D"] = D;
}

inline void UserProfile::reset_level(const string& level) {
	map<string, uLevel>::iterator it;
	if (_level_map.empty())
		init_level_map();
	_user_level = ((it = _level_map.find(level)) != _level_map.end()) ? it->second : A;
}

istream& operator>>(istream& is, UserProfile &rhs) {
	string login, level;
	int lcount, gcount, gcorrect;
	is >> lcount >> gcount >> gcorrect;
	rhs.reset_login(login);
	rhs.reset_level(level);
	rhs.reset_login_count(lcount);
	rhs.reset_guess_count(gcount);
	rhs.reset_guess_correct(gcorrect);
	return is;
}

int main() {
	UserProfile anon;
	cout << anon;

	/*
	UserProfile anon_too;
	cout << anon_too;

	UserProfile anna("C", UserProfile::A);
	cout << anna;

	anna.bump_guess_count(27);
	anna.bump_guess_correct(25);
	anna.bump_login_count();
	cout << anna;

	cin >> anon;
	cout << anon;
	*/
}
