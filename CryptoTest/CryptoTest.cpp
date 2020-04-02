// CryptoTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "asio.hpp"

#include <iostream>
#include "stdio.h"
#include "aes.h"
#include "modes.h"
#include "filters.h"
#include "base64.h"
#include "Aes.h"
#include "cstring"
#include "Aes_Ecb.h"
#include "Converter.h"
#include "rsa.h"
#include "osrng.h"
#include "json.hpp"
#include "Rsa_Oaep.h"
#include "validate.h"
#include "direct.h"
#include "fstream"
#include "Universal.h"
#include "Connector.h"
#include "queue"
#include "time.h"
#include "CommandResolver.h"
#include "pssr.h"

using namespace std;
using namespace CryptoPP;
using nlohmann::json;
using namespace asio;

#define BUFFER 1024
#define IPADDR "127.0.0.1"

int main()
{
	Aes_Ecb aes;
	Rsa_Oaep rsa;
	Connector con(&rsa);
	CommandResolver res(&con,&aes,&rsa);
	bool sendable;

	cout << "[Resolver] : Type '\\\?' for listing all commands" << endl;

	string c;
	getline(cin, c);

	while (1) {
		if(res.Resolve(c))
			break;
		else {
			getline(cin, c);
		}
	}

	while (1) {
		if (con.GetSendable()) {
			cout << "[Resolver] : Type '\\confirm' to continue or type '\\\?' for listing all commands" << endl;
			while (1) {
				getline(cin, c);
				if (c == "\\confirm") {
					break;
				}
				else {
					res.Resolve(c);
				}
			}
			cout << "[Socket] : Permit to send messages" << endl;

			getline(cin, c);

			if (con.GetAESMode()) {
				aes.EncrypteAndEncode(c, c);
			}
			else {
				rsa.EncrypteAndEncode(c, c);
			}

			con.SendMess(c.c_str());

		}
		else {
			cout << "[Socket] : Waitting for messages" << endl;
			con.RecvMess();
			cout << "[Socket]-" <<Universal::GetCurTime()<<" : "<< con.GetMess() << endl;
		}
	}
	return 0;
}

/*
	AutoSeededRandomPool rng;

	InvertibleRSAFunction parameters;
	parameters.GenerateRandomWithKeySize(rng, 3072);

	RSA::PrivateKey privateKey(parameters);
	RSA::PublicKey publicKey(parameters);

	// Message
	string message = "Yoda said, Do or Do Not. There is no try.";
	string signature, recovered;

	////////////////////////////////////////////////
	// Sign and Encode
	RSASS<PSS, SHA256>::Signer signer(privateKey);

	StringSource ss1(message, true,
		new SignerFilter(rng, signer,
			new StringSink(signature)
		) // SignerFilter
	); // StringSource

	cout << signature << endl;

	////////////////////////////////////////////////
	// Verify and Recover
	RSASS<PSS, SHA256>::Verifier verifier(publicKey);

	StringSource ss2(message + signature, true,
		new SignatureVerificationFilter(
			verifier,
			new StringSink(recovered),
			SignatureVerificationFilter::THROW_EXCEPTION |
			SignatureVerificationFilter::PUT_MESSAGE
		) // SignatureVerificationFilter
	); // StringSource

	cout << recovered << endl;
*/

/*
	try {
		// 建立ioservice
		io_service service;

		// 建立套接字并连接
		ip::tcp::socket sockClient(service);
		// 连接终端
		ip::tcp::endpoint toEnd(ip::address::from_string(IPADDR), PORT);
		sockClient.connect(toEnd);

		// 连接成功发送
		cout << "Success" << endl;
		string send = "老二次元了";
		sockClient.send(buffer(send, BUFFER));

		// 接受信息
		char recv[BUFFER] = "";
		sockClient.receive(buffer(recv, BUFFER));
		cout << recv << endl;
	}
	catch (exception& e) {
		cerr << "Exception : " << e.what() << endl;
	}
*/

/*
	try {
// 建立对象
io_service service;

// 建立接收器监听
ip::tcp::acceptor acceptClient(service, ip::tcp::endpoint(ip::tcp::v4(), PORT));

// 连接套接字
ip::tcp::socket sockConnected(service);

// accept阻塞进程
acceptClient.accept(sockConnected);

// 接受成功后连接套接字 
// 接收
char recvBuff[BUFFER] = "";
sockConnected.receive(buffer(recvBuff, BUFFER));
cout << recvBuff << endl;

//发送
string send = "Test";
sockConnected.send(buffer(send, BUFFER));

system("pause")
	}
	catch (exception& e) {
		cerr << "Exception : " << e.what() << endl;
	}
*/