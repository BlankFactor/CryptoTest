#pragma once

#include "pssr.h"
#include "hex.h"
#include "sha.h"
#include "MD5.h"
#include "Sha256.h"
#include "filters.h"

using namespace std;
using namespace CryptoPP;

class MD5_Cus
{
	MD5 md5;
public:
	void GetHash(string _plainText, string& _hash);
	void GetHash16(string _plainText, string& _hash);
};

