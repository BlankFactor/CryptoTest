#pragma once

#include "asio.hpp"
#include "iostream"
#include "cstring"
#include "Rsa_Oaep.h"

using namespace std;
using namespace asio;

#define PORT 5000
#define BUFFER 1024

class Connector
{
private:
	io_service service;
	Rsa_Oaep* rsa;

	bool connected = false;
	bool sendable = false;
	bool recevied = false;

	bool aesMode = true;

	string hostName;

	char message[BUFFER];

	ip::tcp::socket* sock;
	ip::tcp::endpoint* ternimal;
	ip::tcp::acceptor* acceptor;
public:
	Connector(Rsa_Oaep* _rsa);

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
};

