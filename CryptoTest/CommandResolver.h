#pragma once
#include "queue"
#include "iostream"
#include "cstring"
#include "Aes_Ecb.h"
#include "Rsa_Oaep.h"
#include "Connector.h"

using namespace std;

#define connect StringToHash("\connect");

class CommandResolver
{
private:
	Connector* con;
	Aes_Ecb* aes;
	Rsa_Oaep* rsa;

	map<string, int> map;

	vector<string> Split(string _str, string pattern);
	unsigned int StringToHash(string _str);
	unsigned int StringToHash(char* _str);
	unsigned int StringToHash(const char* _str);

	void displayCommands();

	bool Connect(vector<string> _com);
	bool Listen();
	bool Decrypto();
	bool SetKey(vector<string> _com);

public:
	CommandResolver(Connector* _con, Aes_Ecb* _aes, Rsa_Oaep* _rsa);

	bool Resolve(string _command);
};

