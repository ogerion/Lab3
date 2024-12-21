#pragma once
#include "./UnorderedMap.h"
#include "cassert"
void testUnorderedMap()
{
	UnorderedMap<int, int> map(5);
	map.insert(1, 2);
	map.insert(2, 3);
	map.insert(4, 5);
	map.insert(5, 6);

	int r;
	map.find(4, r);

	assert(r == 5);
	assert(!map.empty());
	assert(map.find(1));
	map.erase(2);
	assert(map.size() == 3);
	assert(r == 5);
}