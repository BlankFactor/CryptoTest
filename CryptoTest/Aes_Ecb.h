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

	// 十六进制形式输出密钥
	void DisplaySecByteBlock();
	void DisplayStrKey();

	// Aes加密
	void Encrypte(string _plainText, string& _buffer);
	// Aes解密
	void Decrypte(string _cipherText, string& _buffer);

	// Base64 编码
	void EncodeBase64(string _in, string& _out);
	// Base64 解码
	void DecodeBase64(string _in, string& _out);

	// 对明文 Aes加密 且 Base64编码
	void EncrypteAndEncode(string _plainText, string& _base64);
	// 对二进制流 Base64解码 且 Aes解密 输出明文
	void DecodeAndDecrypte(string _base64, string& _plainText);
};

