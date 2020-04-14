#include "MD5_Cus.h"

void MD5_Cus::GetHash(string _plainText, string& _hash)
{
	_hash.clear();
	StringSource(_plainText, true, new HashFilter(md5, new HexEncoder(new StringSink(_hash))));
}

void MD5_Cus::GetHash16(string _plainText, string& _hash)
{
	_hash.clear();
	StringSource(_plainText, true, new HashFilter(md5, new HexEncoder(new StringSink(_hash))));

	_hash = _hash.substr(0, 16);
}


