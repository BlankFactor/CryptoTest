#pragma once
#include "iostream"
using namespace std;

class ICryptoFilter
{
public:
	virtual void EncrypteAndEncode(string _cipherText, string& _base64) = 0;
	virtual void DecodeAndDecrypte(string _base64, string& _plainText) = 0;
};

