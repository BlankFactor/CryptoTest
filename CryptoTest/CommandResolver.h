#pragma once
#include "queue"
#include "iostream"
#include "cstring"
#include "Aes_Ecb.h"
#include "Rsa_Oaep.h"
#include "Connector.h"
#include "BObjectIdentity.h"
#include "ObjectManager.h"

using namespace std;

#define connect StringToHash("\connect");

class CommandResolver:public BObjectIdentity
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
	CommandResolver();

	bool Resolve(string _command);
};

