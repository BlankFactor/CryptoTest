#include "Converter.h"

void Converter::StringToHex(string _s)
{
	for (int i = 0; i < _s.size(); i++)
		cout << hex << (int)_s[i] << ' ';
	cout << endl;
}

void Converter::StringToHex(unsigned char* _s,int _size)
{
	for (int i = 0; i < _size; i++)
		cout << hex << (int)_s[i] << ' ';
	cout << endl;
}
