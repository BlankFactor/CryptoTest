#include "Sha256.h"

void Sha256::GetHash(string _plainText, unsigned char* _hash)
{
	sha.CalculateDigest(_hash, (byte*)_plainText.c_str(), _plainText.size());
}

bool Sha256::Verify(string _plainText, unsigned char* _hash)
{
	return sha.VerifyDigest(_hash, (byte*)_plainText.c_str(), _plainText.size());
}
