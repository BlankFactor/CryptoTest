#include "asio.hpp"
#include "Rsa_Oaep.h"
#include "files.h"
#include "Universal.h"
#include "pssr.h"

#define SIGN "FACTOR"

using namespace CryptoPP;
using namespace std;
using namespace asio;

Rsa_Oaep::Rsa_Oaep()
{
	cout << "[RSA] : Initializing" << endl;
	priKey = new RSA::PrivateKey();

	string path = Universal::GetPath();
	path.append("\\");
	path.append(ip::host_name());

	string pathPub(path);
	pathPub.append("_PublicKey.key");
	string pathPri(path);
	pathPri.append("_PrivateKey.key");

	// 私钥存在
	if (Universal::FileExist(pathPri)) {
		LoadPrivateKey(pathPri, *priKey);

		// 生成公钥
		pubKey = new RSA::PublicKey(*priKey);
		// 保存公钥
		SavePublicKey(pathPub,*pubKey);
	}
	else {
		priKey->GenerateRandomWithKeySize(rng, 3072);
		pubKey = new RSA::PublicKey(*priKey);

		SavePrivateKey(pathPri, *priKey);
		SavePublicKey(pathPub, *pubKey);
	}

	path.~basic_string();
	pathPri.~basic_string();
	pathPub.~basic_string();
	
	// 初始化连接对象公钥
	pubKey_Co = new RSA::PublicKey();

	encryptor = new RSAES_OAEP_SHA_Encryptor(*pubKey_Co);
	decryptor = new RSAES_OAEP_SHA_Decryptor(*priKey);

	// 初始化签名器,校验器
	signer = new RSASS<PSS, SHA256>::Signer(*priKey);
	verifier = new RSASS<PSS, SHA256>::Verifier(*pubKey_Co);

	cout << "[RSA] : Initialized successfully" << endl;
}

void Rsa_Oaep::SavePublicKey(const string& _filename, PublicKey& _key)
{
	cout << "[RSA] : Generating public key" << endl;

	ByteQueue queue;
	_key.Save(queue);

	FileSink file(_filename.c_str());
	queue.CopyTo(file);
	file.MessageEnd();
}

void Rsa_Oaep::LoadPublicKey(const string& _filename, PublicKey& _key)
{
	ByteQueue queue;

	FileSource file(_filename.c_str(), true);
	file.TransferTo(queue);
	queue.MessageEnd();

	_key.Load(queue);
}

void Rsa_Oaep::LoadPublicKeyCoByHostName(const string& _hostName)
{
	connected = true;

	string path = Universal::GetPath();
	path.append("\\");
	path.append(_hostName);
	path.append("_PublicKey.key");

	RSA::PublicKey pk;

	LoadPublicKey(path, pk);
	
	pubKey_Co = &pk;
	encryptor = new RSAES_OAEP_SHA_Encryptor(*pubKey_Co);
	verifier = new RSASS<PSS, SHA256>::Verifier(*pubKey_Co);

	path.~basic_string();
}

void Rsa_Oaep::SavePrivateKey(const string& _filename, PrivateKey& _key)
{
	cout << "[RSA] : Generating private key" << endl;
	ByteQueue queue;
	_key.Save(queue);

	FileSink file(_filename.c_str());
	queue.CopyTo(file);
	file.MessageEnd();
}

void Rsa_Oaep::LoadPrivateKey(const string& _filename, PrivateKey& _key)
{
	cout << "[RSA] : Loadding private key" << endl;

	ByteQueue queue;

	FileSource file(_filename.c_str(), true);
	file.TransferTo(queue);
	queue.MessageEnd();

	_key.Load(queue);
}

void Rsa_Oaep::Sign()
{
	signature.clear();
	StringSource ss(SIGN, true, new SignerFilter(rng, *signer, new StringSink(signature)));

	baseURL64Encode(signature, signature);
}	

bool Rsa_Oaep::Verify()
{
	recovered.clear();
	baseURL64Decode(signature, signature);

	StringSource ss(SIGN + signature, true,
		new SignatureVerificationFilter(
			*verifier,
			new StringSink(recovered),
			SignatureVerificationFilter::THROW_EXCEPTION |
			SignatureVerificationFilter::PUT_MESSAGE
		)
	);

	if (recovered == SIGN) {
		cout << "[RSA] : Verifies digitial signurate successfully" << endl;

		return true;
	}

	cout << "[RSA] : ***** Digitial signurate not valid *****" << endl;
	return false;
}

void Rsa_Oaep::SetSignature(string _s)
{
	signature = _s;
}

string Rsa_Oaep::GetSignature()
{
	return signature;
}

string Rsa_Oaep::GetRecovered()
{
	return recovered;
}

void Rsa_Oaep::Encrypte(string _plainText, string& _buffer)
{
	StringSource ss(_plainText, true, new PK_EncryptorFilter(rng, *encryptor, new StringSink(_buffer)));
}

void Rsa_Oaep::Decrypte(string _cipherText, string& _buffer)
{
	StringSource ss(_cipherText, true, new PK_DecryptorFilter(rng, *decryptor, new StringSink(_buffer)));
}

void Rsa_Oaep::baseURL64Encode(string _in, string& _out)
{
	_out.clear();
	StringSource ssEncode(_in, true, new Base64URLEncoder(new StringSink(_out)));
}

void Rsa_Oaep::baseURL64Decode(string _in, string& _out)
{
	_out.clear();
	StringSource  ssDecode(_in, true, new Base64URLDecoder(new StringSink(_out)));
}

void Rsa_Oaep::EncrypteAndEncode(string _cipherText, string& _out)
{
	try {
		string temp = "";
		_out.clear();

		Encrypte(_cipherText, temp);
		baseURL64Encode(temp, _out);

		temp.~basic_string();
	}
	catch (std::exception& e) {
		cerr << "[RSA exception] : " << e.what() << endl;
	}
}

void Rsa_Oaep::DecodeAndDecrypte(string _in, string& _plainText)
{
	try {
	string temp = "";
	_plainText.clear();

	baseURL64Decode(_in, temp);
	Decrypte(temp, _plainText);

	temp.~basic_string();
	}
	catch (std::exception& e) {
		cerr << "[RSA exception] : " << e.what() << endl;
	}
}
