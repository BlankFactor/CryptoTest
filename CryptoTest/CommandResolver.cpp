#include "CommandResolver.h"

#define HELP 100
#define CONNECT 1
#define LISTEN 2
#define ADDRESS 11
#define MODE 12
#define PORT 13
#define SEND 3
#define DECRYPTO 4
#define SETKEY 5
#define CLS 6


vector<string> CommandResolver::Split(string _str, string pattern)
{
	vector<string> resVec;

	if ("" == _str)
	{
		return resVec;
	}

	string strs = _str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != string::npos)
	{
		string x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}

unsigned int CommandResolver::StringToHash(string _str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;
	for (int i = 0; i < _str.size(); i++) {
		hash = hash * seed + (_str[0]);
	}
	return (hash & 0x7FFFFFFF);
}
unsigned int CommandResolver::StringToHash(char* _str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;
	while(*_str){
		hash = hash * seed + (*_str++);
	}
	return (hash & 0x7FFFFFFF);
}
unsigned int CommandResolver::StringToHash(const char* _str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;
	while (*_str) {
		hash = hash * seed + (*_str++);
	}
	return (hash & 0x7FFFFFFF);
}

void CommandResolver::displayCommands()
{
	cout << "ÎÒËï¸çËµÁËËã" << endl;
}

bool CommandResolver::Connect(vector<string> _com)
{
	if (con->GetConnected()) {
		cout << "[Resolver ]Unable to execute this command when connecting a host" << endl;
		return false;
	}

	auto it = _com.begin();
	_com.erase(it);

	string ip;

	while (!_com.empty()) {
		switch (map[_com[0]]) {
			case ADDRESS: {
				if (_com.size() == 1) {
					cout << "[Resolver] : Inviald command" << endl;
					return false;
				}

				ip = _com[1];

				it = _com.begin();
				_com.erase(it, it + 2);
				break;
			}
			case MODE: {
				if (_com.size() == 1) {
					cout << "[Resolver] : Inviald command" << endl;
					return false;
				}

				string m = _com[1];
				if (m == "aes") {
					con->SetAESMode(true);
				}
				else if(m == "rsa"){
					con->SetAESMode(false);
				}
				else {
					cout << "[Resolver] : Inviald command" << endl;
					return false;
				}

				it = _com.begin();
				_com.erase(it, it + 2);

				break;
			}
			case PORT: {
				if (_com.size() == 1) {
					cout << "[Resolver] : Inviald command" << endl;
					return false;
				}

				string m = _com[1];
				int newP = atoi(m.c_str());
				if (!con->SetPort(newP)) 
				{
					cout << "[Resolver] : Inviald command" << endl;
					return false;
				}

				it = _com.begin();
				_com.erase(it, it + 2);

				break;
			}

			default: {
				cout << "[Resolver] : Inviald command" << endl;
				return false;
			}
		}
	}

	if (!con->Connect(ip)) {
		cout << "[Resolver] : Inviald command" << endl;
		return false;
	}

	return true;
}

bool CommandResolver::Listen()
{
	if (con->GetConnected()) {
		cout << "[Resolver] : Unable to execute this command when connecting a host" << endl;
		return false;
	}

	return con->Listen();
}

bool CommandResolver::Decrypto()
{
	if (con->GetConnected()) {
		string temp = "";

		if (con->GetAESMode()) {
			aes->DecodeAndDecrypte(con->GetMess(), temp);
		}
		else {
			rsa->DecodeAndDecrypte(con->GetMess(),temp);
		}
		cout << "[PlainText] : " << temp << endl;

		return true;
	}
	else {
		cout << "[Resolver] : Unable to execute this command without connecting a host" << endl;
		return false;
	}
}

bool CommandResolver::SetKey(vector<string> _com)
{
	if (!con->GetAESMode()) {
		cout << "[Resolver] : Unable to execute this command in RSA mode" << endl;
		return false;
	}

	auto it = _com.begin();
	_com.erase(it);

	if (_com.size() == 0) {
		cout << "[Resolver] : Inviald command" << endl;
		return false;
	}

	string newkey = _com[0];
	aes->SetKey(newkey);

	return true;
}

CommandResolver::CommandResolver(Connector* _con, Aes_Ecb* _aes, Rsa_Oaep* _rsa)
{
	con = _con;
	aes = _aes;
	rsa = _rsa;

	map["\\connect"] = CONNECT;
	map["\\listen"] = LISTEN;
	map["\\send"] = SEND;
	map["\\decrypto"] = DECRYPTO;
	map["\\setkey"] = SETKEY;
	map["\\cls"] = CLS;
	map["\\\?"] = HELP;
	map["-h"] = ADDRESS;
	map["-m"] = MODE;
	map["-p"] = PORT;
}

bool CommandResolver::Resolve(string _command)
{
	vector<string> result = Split(_command," ");

	switch (map[result[0]])
	{
	case CONNECT: {
		return Connect(result);
		break;
	}
	case LISTEN: {
		return Listen();
		break;
	}
	case SEND: {

		break;
	}
	case SETKEY: {
		return SetKey(result);
		break;
	}
	case DECRYPTO: {
		return Decrypto();
		break;
	}
	case HELP: {
		displayCommands();
		return false;
		break;
	}
	case CLS: {
		system("cls");
		return false;
		break;
	}
	default: {
		cout << "[Resolver] : Inviald command" << endl;
		return false;
	}
	}
}
