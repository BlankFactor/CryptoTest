#include "Aes_Ecb.h"

using namespace std;
using namespace CryptoPP;

Aes_Ecb::Aes_Ecb(string _key)
{
	cout << "[AES-ECB] : Initializing" << endl;
	SetKey(_key);
	cout << "[AES-ECB] : Initialized successfully" << endl;
}

Aes_Ecb::Aes_Ecb()
{
	cout << "[AES-ECB] : Initializing" << endl;
	strKey.append(16, 'a');
	cout << "[AES-ECB] : Setting key -> ( " << strKey << " ) " << endl;
	key.Assign((byte*)strKey.c_str(), strKey.size());

	encryptor.SetKey(key, key.size());
	decryptor.SetKey(key, key.size());
	cout << "[AES-ECB] : Initialized successfully" << endl;
}

string Aes_Ecb::GetKey()
{
	return strKey;
}

void Aes_Ecb::SetKey(string _key)
{
	strKey = _key;
	cout << "[AES-ECB] : Setting key -> ( " << strKey << " ) " << endl;

	if (strKey.size() < 16) {
		int count = 16 - strKey.size();
		strKey.append(count, 'a');
	}
	else {
		strKey = strKey.substr(0, 16);
	}

	key.Assign((byte*)strKey.c_str(), strKey.size());

	encryptor.SetKey(key, key.size());
	decryptor.SetKey(key, key.size());
}

void Aes_Ecb::DisplaySecByteBlock()
{
	cout << "[AES-ECB] : ***** SecByteBlock *****" << endl;
	for (auto it = key.begin(); it != key.end(); it++)
		cout <<hex<< (int)(*it) << ' ';
	cout << endl;
	cout << "*****" << endl;
}

void Aes_Ecb::DisplayStrKey()
{
	cout << "[AES-ECB] : ***** StrKey *****" << endl;
	for (int i = 0; i < strKey.size(); i++)
		cout << hex << (int)strKey[i] << ' ';
	cout << endl;
	cout << "*****" << endl;
}

void Aes_Ecb::Encrypte(string _plainText, string& _buffer)
{
	try
	{
		_buffer.clear();
		StringSource ss(_plainText, true, new StreamTransformationFilter(encryptor, new StringSink(_buffer), BlockPaddingSchemeDef::PKCS_PADDING));
	}
	catch (CryptoPP::InvalidCiphertext e)
	{
		cout << e.GetWhat() << endl;
	}
}

void Aes_Ecb::Decrypte(string _cipherText, string& _buffer)
{
	try
	{
		_buffer.clear();
		StringSource ss(_cipherText, true, new StreamTransformationFilter(decryptor, new StringSink(_buffer), BlockPaddingSchemeDef::PKCS_PADDING));
	}
	catch (CryptoPP::InvalidCiphertext e)
	{
		cout << e.GetWhat() << endl;
	}
}

void Aes_Ecb::EncodeBase64(string _in, string& _out)
{
	StringSource encode(_in, true, new Base64Encoder(new StringSink(_out)));
}

void Aes_Ecb::DecodeBase64(string _in, string& _out)
{
	StringSource decode(_in, true, new Base64Decoder(new StringSink(_out)));
}

void Aes_Ecb::EncrypteAndEncode(string _plainText, string& _base64)
{
	string temp = "";
	_base64.clear();

	Encrypte(_plainText, temp);
	EncodeBase64(temp, _base64);

	temp.~basic_string();
}

void Aes_Ecb::DecodeAndDecrypte(string _base64, string& _plainText)
{
	string temp = "";
	_plainText.clear();

	DecodeBase64(_base64, temp);
	Decrypte(temp, _plainText);

	temp.~basic_string();
}
