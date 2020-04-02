#pragma once

#include "iostream"
#include "fstream"
#include "cstring"
#include "direct.h"

using namespace std;

static class Universal
{
public:
	static bool FileExist(const string& _path);

	static string GetPath();

	static string GetCurTime();
};

