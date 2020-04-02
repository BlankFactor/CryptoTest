#include "Universal.h"
#include "direct.h"
#include "ctime"

bool Universal::FileExist(const string& _path)
{
	ifstream f(_path.c_str());
	return f.good();
}

string Universal::GetPath()
{
	char path[256];
	_getcwd(path, sizeof(path));
	string pathStr(path);
	return pathStr;
}

string Universal::GetCurTime()
{
	time_t t = time(NULL);
	tm ti;
	localtime_s(&ti, &t);
	char ch[64] = { 0 };
	strftime(ch, sizeof(ch) - 1, "%H:%M:%S", &ti);
	return ch;
}
