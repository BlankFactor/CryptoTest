#pragma once

#include <iostream>
#include "stdio.h"
#include "aes.h"
#include "modes.h"
#include "filters.h"
#include "base64.h"
#include "Aes.h"
#include <cstring>

using namespace CryptoPP;
using namespace std;

class Aes_Ecb
{
private:
	string strKey;
	SecByteBlock key;

	ECB_Mode<AES>::Encryption encryptor;
	ECB_Mode<AES>::Decryption decryptor;
public:
	Aes_Ecb(string _key);
	Aes_Ecb();

	string GetKey();
	void SetKey(string _key);

	// ʮ��������ʽ�����Կ
	void DisplaySecByteBlock();
	void DisplayStrKey();

	// Aes����
	void Encrypte(string _plainText, string& _buffer);
	// Aes����
	void Decrypte(string _cipherText, string& _buffer);

	// Base64 ����
	void EncodeBase64(string _in, string& _out);
	// Base64 ����
	void DecodeBase64(string _in, string& _out);

	// ������ Aes���� �� Base64����
	void EncrypteAndEncode(string _plainText, string& _base64);
	// �Զ������� Base64���� �� Aes���� �������
	void DecodeAndDecrypte(string _base64, string& _plainText);
};

