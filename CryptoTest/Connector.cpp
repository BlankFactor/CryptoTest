#include "Connector.h"
#include "Universal.h"

Connector::Connector(Rsa_Oaep* _rsa)
{
	rsa = _rsa;
	port = PORT;
	sock = new ip::tcp::socket(service);
	acceptor = new ip::tcp::acceptor(service,ip::tcp::endpoint(ip::tcp::v4(),PORT));
}

bool Connector::GetConnected()
{
	return connected;
}

void Connector::SetAESMode(bool _v)
{
	aesMode = _v;
}

bool Connector::GetAESMode()
{
	return aesMode;
}

char* Connector::GetMess()
{
	return message;
}

void Connector::SetSendable(bool _v)
{
	sendable = _v;
}

bool Connector::GetSendable()
{
	return sendable;
}

bool Connector::Connect(string _ipAddress)
{
	try {
		cout << "[Socket] : Start connectting " << _ipAddress << endl;
		cout << "[Socket] : Connecting " << endl;
		ternimal = new ip::tcp::endpoint(ip::address::from_string(_ipAddress), port);
		
		sock->connect(*ternimal);

		cout << "[Socket] : Connected successfully" << endl;

		// 初始化依次发送 主机名 加密方式
		sock->send(buffer(ip::host_name().c_str(), BUFFER));
		cout << "已发送本机主机名" << endl;

		char aes = aesMode ? 'A' : 'R';
		sock->send(buffer(&aes, 1));
		// 接受 对方主机名
		char temp[BUFFER] = "";
		sock->receive(buffer(temp, BUFFER));
		cout << "接收到主机名 : " << temp << endl;

		// 查询目标主机公钥是否存在
		string targetPubKeyPath = Universal::GetPath();
		targetPubKeyPath.append("\\");
		targetPubKeyPath.append(temp);
		targetPubKeyPath.append("_PublicKey.key");

		if (Universal::FileExist(targetPubKeyPath)) {
			cout << "目标主机公钥存在" << endl;
			rsa->LoadPublicKeyCoByHostName(temp);
		}
		else {
			cout << "目标主机公钥不存在 断开连接" << endl;
			sock->close();
			return false;
		}

		connected = true;
		SetSendable(true);

		return true;
	}
	catch (exception& e) {
		cerr << "[Socket Exception] : " << e.what() << endl;
		return false;
	}
}

bool Connector::Listen()
{
	try {
		cout << "[Socket] : Start listenning" << endl;
		acceptor->accept(*sock);
		cout << "[Socket] : Connected to " << sock->remote_endpoint().address() << endl;

		// 依次接受 对方主机名 加密方式
		char temp[BUFFER] = "";
		sock->receive(buffer(temp, BUFFER));
		cout << "接收到主机名 : " << temp << endl;

		char mode;
		sock->receive(buffer(&mode, 1));
		cout << "接收到加密模式 : " << (mode == 'A' ?  "AES" : "RSA") << endl;
		SetAESMode(mode == 'A' ? true : false);
		// 发送主机名
		sock->send(buffer(ip::host_name().c_str(), BUFFER));
		cout << "已发送本机主机名" << endl;

		// 查询目标主机公钥是否存在
		string targetPubKeyPath = Universal::GetPath();
		targetPubKeyPath.append("\\");
		targetPubKeyPath.append(temp);
		targetPubKeyPath.append("_PublicKey.key");

		if (Universal::FileExist(targetPubKeyPath)) {
			cout << "目标主机公钥存在" << endl;
			rsa->LoadPublicKeyCoByHostName(temp);
		}
		else {
			cout << "目标主机公钥不存在 断开连接" << endl;
			sock->close();
		}

		connected = true;
		SetSendable(false);
		return true;
	}
	catch (exception& e) {
		cout << "[Socket exception] : " << e.what() << endl;
		return false;
	}
}

void Connector::SendMess(const char* _mess)
{
	try{
		if (!connected) {
			cout << "[Socket] : Invliad ternimal" << endl;
			return;
		}

		//RSA模式下 首先发送签名
		if (!aesMode) {
			rsa->Sign();
			string sign = rsa->GetSignature();
			sock->send(buffer(sign.c_str(), BUFFER));
			sign.~basic_string();
		}
			

		recevied = false;
		sock->send(buffer(_mess, BUFFER));
		SetSendable(false);
	}
	catch (exception& e) {
		cout << "[Socket excpetion] : " << e.what() << endl;
		system("pause");
		exit(0);
	}
}
void Connector::SendMess(char* _mess)
{
	try {
		if (!connected) {
			cout << "[Socket] : Invliad ternimal" << endl;
			return;
		}

		//RSA模式下 首先发送签名
		if (!aesMode) {
			rsa->Sign();
			string sign = rsa->GetSignature();
			sock->send(buffer(sign.c_str(), BUFFER));
			sign.~basic_string();
		}

		recevied = false;
		sock->send(buffer(_mess, BUFFER));
		SetSendable(false);
	}
	catch (exception& e) {
		cout << "[Socket excpetion] : " << e.what() << endl;
		system("pause");
		exit(0);
	}
}

void Connector::RecvMess()
{
	memset(message, '\0', sizeof(message));

	try {
		if (!connected) {
			cout << "[Socket] : Invliad ternimal" << endl;
			return;
		}

		//RSA模式下 首先接收签名
		if (!aesMode) {
			char temp[BUFFER] = "";
			sock->receive(buffer(temp,BUFFER));
			rsa->SetSignature(temp);
			rsa->Verify();
		}

		sock->receive(buffer(message, BUFFER));
		SetSendable(true);
		recevied = true;
	}
	catch (exception& e) {
		cout << "[Socket excpetion] : " << e.what() << endl;
		system("pause");
		exit(0);
	}

}

bool Connector::SetPort(int _value)
{
	if (_value > 1024 && _value < 65535) {
		port = _value;
		cout << "[Socket] : Set port -> " << port << endl;
		return true;
	}

	return false;
}
