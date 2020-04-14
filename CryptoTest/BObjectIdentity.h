#pragma once
#include "ObjectManager.h"
#include "iostream"

class BObjectIdentity
{
public:
	template <typename T>
	BObjectIdentity(T& t);
};

template<typename T>
inline BObjectIdentity::BObjectIdentity(T& t)
{
	ObjectManager::InsertOjbect(t);
}
