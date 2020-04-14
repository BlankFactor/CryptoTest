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

	// 公钥 保存/载入
	void SavePublicKey(const string& _filename, PublicKey& _key);
	void LoadPublicKey(const string& _filename, PublicKey& _key);
	// 根据主机名 载入连接对象的公钥
	void LoadPublicKeyCoByHostName(const string& _hostName);
	// 私钥 保存/载入
	void SavePrivateKey(const string& _filename, PrivateKey& _key);
	void LoadPrivateKey(const string& _filename, PrivateKey& _key);
	
	// 数字签名
	void Sign();
	// 验证签名
	bool Verify();

	void SetSignature(string _s);
	// 获得签名/校验结果
	string GetSignature();
	string GetRecovered();

	// RSA-OAEP 加密
	void Encrypte(string _plainText, string& _buffer);
	// RSA-OAEP 解密
	void Decrypte(string _cipherText, string& _buffer);

	// BASE64URL 编码
	void baseURL64Encode(string _in, string& _out);
	// BASE64URL 解码
	void baseURL64Decode(string _in, string& _out);

	// 加密后编码 用于网络传输
	void EncrypteAndEncode(string _cipherText, string& _base64);
	// 解码后解密 获得明文
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
	// 随机数
	AutoSeededRandomPool rng;

	// 准备工作
	RSA::PrivateKey rsaPriKey;
	rsaPriKey.GenerateRandomWithKeySize(rng, 1024);
	RSA::PublicKey rsaPubKey(rsaPriKey);

	string plainText = "abc";
	string cipherText = "";

	// 加密
	// OAEP
	RSAES_OAEP_SHA_Encryptor rsaEncryptor(rsaPubKey);

	// pipelining
	StringSource ss1(plainText, true, new PK_EncryptorFilter(rng, rsaEncryptor, new StringSink(cipherText)));

	// base64URL编码
	string str64URLEncode = "";
	StringSource ssEncode(cipherText, true, new Base64URLEncoder(new StringSink(str64URLEncode)));
	cout << str64URLEncode << endl;

	// 网络传输

	// base64URL解码
	string str64URLDecode = "";
	StringSource  ssDecode(str64URLEncode, true, new Base64URLDecoder(new StringSink(str64URLDecode)));

	// 解密
	RSAES_OAEP_SHA_Decryptor rsaDecryptor(rsaPriKey);
	string strOriginalText = "";
	StringSource ss2(str64URLDecode, true, new PK_DecryptorFilter(rng, rsaDecryptor, new StringSink(strOriginalText)));
	cout << strOriginalText << endl;
*/
