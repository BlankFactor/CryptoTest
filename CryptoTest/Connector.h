#pragma once

#include "asio.hpp"
#include "iostream"
#include "cstring"
#include "Rsa_Oaep.h"
#include "ICryptoFilter.h"
#include "BObjectIdentity.h"

using namespace std;
using namespace asio;

#define PORT 5000
#define BUFFER 1024

class Connector:public BObjectIdentity
{
private:
	io_service service;
	Rsa_Oaep* rsa;

	bool connected = false;
	bool sendable = false;
	bool recevied = false;

	bool aesMode = true;

	string hostName;
	int port;

	char message[BUFFER];

	ip::tcp::socket* sock;
	ip::tcp::endpoint* ternimal;
	ip::tcp::acceptor* acceptor;
public:
	ICryptoFilter* filter;

	Connector();

	bool GetConnected();
	void SetAESMode(bool _v);
	bool GetAESMode();

	char* GetMess();

	void SetSendable(bool _v);
	bool GetSendable();

	bool Connect(string _ipAddress);
	bool Listen();

	void SendMess(const char* _mess);
	void SendMess(char* _mess);
	void RecvMess();

	bool SetPort(int _value);
};

