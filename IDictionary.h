#pragma once
#include "UnorderedMap.h"

template <class KType, class VType>
class IDictionary
{
private:
	int count;
	UnorderedMap<KType, VType> data;
public:

	IDictionary() : data(), count(0) {}

	int GetCount()
	{
		return count;
	}

	VType Get(KType key)
	{
		if (data.find(key))
		{
			VType el;
			data.find(key, el);
			return el;
		}
		throw "There're no elements with this key!";
	}

	int GetCapacity()
	{
		return data.capacity();
	}

	bool ContainsKey(TKey key)
	{
		return data.find(key);
	}

	void Add(TKey key, VType element)
	{
		data.insert(key, element);
	}

	void erase(TKey key)
	{
		data.erase(key);
	}
};