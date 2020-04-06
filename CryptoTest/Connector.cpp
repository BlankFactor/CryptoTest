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

		// ��ʼ�����η��� ������ ���ܷ�ʽ
		sock->send(buffer(ip::host_name().c_str(), BUFFER));
		cout << "�ѷ��ͱ���������" << endl;

		char aes = aesMode ? 'A' : 'R';
		sock->send(buffer(&aes, 1));
		// ���� �Է�������
		char temp[BUFFER] = "";
		sock->receive(buffer(temp, BUFFER));
		cout << "���յ������� : " << temp << endl;

		// ��ѯĿ��������Կ�Ƿ����
		string targetPubKeyPath = Universal::GetPath();
		targetPubKeyPath.append("\\");
		targetPubKeyPath.append(temp);
		targetPubKeyPath.append("_PublicKey.key");

		if (Universal::FileExist(targetPubKeyPath)) {
			cout << "Ŀ��������Կ����" << endl;
			rsa->LoadPublicKeyCoByHostName(temp);
		}
		else {
			cout << "Ŀ��������Կ������ �Ͽ�����" << endl;
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

		// ���ν��� �Է������� ���ܷ�ʽ
		char temp[BUFFER] = "";
		sock->receive(buffer(temp, BUFFER));
		cout << "���յ������� : " << temp << endl;

		char mode;
		sock->receive(buffer(&mode, 1));
		cout << "���յ�����ģʽ : " << (mode == 'A' ?  "AES" : "RSA") << endl;
		SetAESMode(mode == 'A' ? true : false);
		// ����������
		sock->send(buffer(ip::host_name().c_str(), BUFFER));
		cout << "�ѷ��ͱ���������" << endl;

		// ��ѯĿ��������Կ�Ƿ����
		string targetPubKeyPath = Universal::GetPath();
		targetPubKeyPath.append("\\");
		targetPubKeyPath.append(temp);
		targetPubKeyPath.append("_PublicKey.key");

		if (Universal::FileExist(targetPubKeyPath)) {
			cout << "Ŀ��������Կ����" << endl;
			rsa->LoadPublicKeyCoByHostName(temp);
		}
		else {
			cout << "Ŀ��������Կ������ �Ͽ�����" << endl;
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

		//RSAģʽ�� ���ȷ���ǩ��
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

		//RSAģʽ�� ���ȷ���ǩ��
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

		//RSAģʽ�� ���Ƚ���ǩ��
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
