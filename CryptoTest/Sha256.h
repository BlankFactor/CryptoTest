#pragma once

#include "sha.h"
#include "filters.h"

using namespace std;
using namespace CryptoPP;

class Sha256
{
private:
	SHA256 sha;
public:
	void GetHash(string _plainText, unsigned char* _hash);
	bool Verify(string _plainText, unsigned char* _hash);
};

