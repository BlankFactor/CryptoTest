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
#include "hex.h"
#include "sha.h"
#include "MD5.h"
#include "Sha256.h"
#include "MD5_Cus.h"
#include "ObjectManager.h"

using namespace std;
using namespace CryptoPP;
using nlohmann::json;
using namespace asio;

#define BUFFER 1024
#define IPADDR "127.0.0.1"
char CHARSET[62];
/*
string source = "HAED";
Sha256 sha;

void crack(string* _str, int _curIndex = 0) {
	for (int i = 0; i < 62; i++) {
		(*_str)[_curIndex] = CHARSET[i];
		
		if (_curIndex != _str->size() - 1) {
			crack(_str, _curIndex + 1);
		}
		else {
			unsigned char _hash[32];

			sha.GetHash(*_str, _hash);
			if (sha.Verify(source, _hash)) {
				cout << "Success : " << *_str << endl;
				exit(0);
			}
			else {
				cout << "Failed : " << *_str << endl;
			}
		}
	}
}
*/

int main()
{

	Aes_Ecb aes;
	Rsa_Oaep rsa;
	Connector con;
	CommandResolver res;

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

			con.filter->EncrypteAndEncode(c, c);

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
int j = 0;
for (int i = 'a'; i < 'a' + 26; i++,j++) {
	CHARSET[j] = i;
}

for (int i = 'A'; i < 'A' + 26; i++, j++) {
	CHARSET[j] = i;
}

for (int i = '0'; i < '0' + 10; i++, j++) {
	CHARSET[j] = i;
}

unsigned char hash[32] = "";
unsigned char hashAfer[32] = "";
sha.GetHash(source, hash);
string* aa = new string(source.size(), 'a');

string path = Universal::GetPath();
path.append("\\Hash.txt");

fstream file;
file.open(path, ios::out);

string ss = "";
string dd = "";
MD5_Cus md5;

cout << "Type '\quit' to exit" << endl;

while (1) {
	ss.clear();
	dd.clear();
	getline(cin, ss);

	if (ss == "\quit");

	md5.GetHash(ss, dd);
	cout << dd << endl;

	file << ss << "	Hash:  " << dd << endl;
	cout << dd.size() << endl;
}

return 0;
*/


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