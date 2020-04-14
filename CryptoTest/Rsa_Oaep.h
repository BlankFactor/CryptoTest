#pragma once
#include "asio.hpp"
#include "rsa.h"
#include "osrng.h"
#include "Converter.h"
#include "base64.h"
#include "fstream"
#include "files.h"
#include "pssr.h"
#include "ICryptoFilter.h"
#include "BObjectIdentity.h"

using namespace std;
using namespace CryptoPP;

class Rsa_Oaep :public ICryptoFilter,public BObjectIdentity
{
private:
	AutoSeededRandomPool rng;
	RSA::PrivateKey* priKey;
	RSA::PublicKey* pubKey;
	RSA::PublicKey* pubKey_Co;

	string signature, recovered;
	RSASS<PSS, SHA256>::Signer* signer;
	RSASS<PSS, SHA256>::Verifier* verifier;

	bool connected = false;

	RSAES_OAEP_SHA_Encryptor* encryptor;
	RSAES_OAEP_SHA_Decryptor* decryptor;
public:
	Rsa_Oaep();

	// ��Կ ����/����
	void SavePublicKey(const string& _filename, PublicKey& _key);
	void LoadPublicKey(const string& _filename, PublicKey& _key);
	// ���������� �������Ӷ���Ĺ�Կ
	void LoadPublicKeyCoByHostName(const string& _hostName);
	// ˽Կ ����/����
	void SavePrivateKey(const string& _filename, PrivateKey& _key);
	void LoadPrivateKey(const string& _filename, PrivateKey& _key);
	
	// ����ǩ��
	void Sign();
	// ��֤ǩ��
	bool Verify();

	void SetSignature(string _s);
	// ���ǩ��/У����
	string GetSignature();
	string GetRecovered();

	// RSA-OAEP ����
	void Encrypte(string _plainText, string& _buffer);
	// RSA-OAEP ����
	void Decrypte(string _cipherText, string& _buffer);

	// BASE64URL ����
	void baseURL64Encode(string _in, string& _out);
	// BASE64URL ����
	void baseURL64Decode(string _in, string& _out);

	// ���ܺ���� �������紫��
	void EncrypteAndEncode(string _cipherText, string& _base64);
	// �������� �������
	void DecodeAndDecrypte(string _base64, string& _plainText);
};

/*
	Rsa_Oaep rsa;

	string mingwen = "Good";
	string miwen = "";

	rsa.EncrypteAndEncode(mingwen, miwen);
	rsa.DecodeAndDecrypte(miwen, miwen);

	cout << miwen << endl;
*/


/*
	// �����
	AutoSeededRandomPool rng;

	// ׼������
	RSA::PrivateKey rsaPriKey;
	rsaPriKey.GenerateRandomWithKeySize(rng, 1024);
	RSA::PublicKey rsaPubKey(rsaPriKey);

	string plainText = "abc";
	string cipherText = "";

	// ����
	// OAEP
	RSAES_OAEP_SHA_Encryptor rsaEncryptor(rsaPubKey);

	// pipelining
	StringSource ss1(plainText, true, new PK_EncryptorFilter(rng, rsaEncryptor, new StringSink(cipherText)));

	// base64URL����
	string str64URLEncode = "";
	StringSource ssEncode(cipherText, true, new Base64URLEncoder(new StringSink(str64URLEncode)));
	cout << str64URLEncode << endl;

	// ���紫��

	// base64URL����
	string str64URLDecode = "";
	StringSource  ssDecode(str64URLEncode, true, new Base64URLDecoder(new StringSink(str64URLDecode)));

	// ����
	RSAES_OAEP_SHA_Decryptor rsaDecryptor(rsaPriKey);
	string strOriginalText = "";
	StringSource ss2(str64URLDecode, true, new PK_DecryptorFilter(rng, rsaDecryptor, new StringSink(strOriginalText)));
	cout << strOriginalText << endl;
*/
