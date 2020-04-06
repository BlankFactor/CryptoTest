#pragma once

#include "iostream"

using namespace std;

static class Converter
{
public:
	static void StringToHex(string _s);
	static void StringToHex(unsigned char* _s,int _size);
};

