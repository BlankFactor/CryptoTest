#pragma once
#include "iostream"
#include "map"
#include "vector"

using namespace std;

static class ObjectManager
{
public:
	static map<string, int> objectPool;

	template <typename T>
	static T* FindObject();

	template <typename T>
	static void InsertOjbect(T& _o);
};

template<typename T>
inline static T* ObjectManager::FindObject()
{
	int addr;

	for (auto it = objectPool.begin(); it != objectPool.end(); it++) {
		if (typeid(T).name() == it->first) {
			addr = it->second;
			return (T*)addr;
			break;
		}
	}

	cerr << "[Object Manager] : returned a nullptr" << endl;
	return nullptr;
}

template<typename T>
inline static void ObjectManager::InsertOjbect(T& _o)
{
	objectPool[typeid(T).name()] = (int)&_o;
}