#pragma once


// C HashMap

#include <cstdlib>
#include <cstring>

typedef unsigned long (*HMHashFunc)(const void *);
typedef int (*HMEqualFunc)(const void *, const void *);

class ListNode {
public:
	ListNode* next;
	unsigned long hashVal;
	const void* key;
	void* val;
};

class CHashMap
{
public:
	CHashMap(int theSize, double theResizeFactor, double theGrowthFactor, HMHashFunc theHashFn, HMEqualFunc theEqualFn)
		: size(theSize)
		, resizeFactor(theResizeFactor)
		, growthFactor(theGrowthFactor)
		, nElems(0)
		, table((ListNode**)std::malloc(sizeof(ListNode) * size))
		, hashFn(theHashFn)
		, equalFn(theEqualFn)
	{
		std::memset(table, 0, size*sizeof(ListNode*));
	}

	void resize(int newSize)
	{
		int i;
		int oldSize = size;

		ListNode** oldTable = table;
		ListNode** newTable = table = (ListNode**)std::malloc(sizeof(ListNode*)*newSize);
		std::memset(newTable, 0, newSize*sizeof(ListNode*));
		size = newSize;

		for (i = 0; i < oldSize; i++)
		{
			ListNode* node = oldTable[i];
			
			while (node)
			{
				ListNode* oldNext = node->next;
				unsigned long slot = node->hashVal % newSize;
				node->next = newTable[slot];
				newTable[slot] = node;
				node = oldNext;
			}
		}

		std::free(oldTable);
	}

	void* find(const void* key)
	{
		unsigned long hashVal = hashFn((void *)key);
		ListNode* node = table[hashVal % size];

		while (node)
		{
			if (hashVal == node->hashVal && equalFn((void *)node->key, (void *)key))
				return node->val;
			node = node->next;
		}

		return nullptr;
	}

	void* add(const void* key, const void* val)
	{
		unsigned long hashVal = hashFn((void *)key);
		unsigned long slot = hashVal % size;
		ListNode* newNode;

		if ((++nElems) > (resizeFactor * size))
		{
			resize(((int)(growthFactor * size + 0.5)) | 0x1);
			slot = hashVal % size;
		}
		
		newNode = (ListNode*)std::malloc(sizeof(ListNode));
		newNode->hashVal = hashVal;
		newNode->key = key;
		newNode->val = (void *)val;
		newNode->next = table[slot];
		table[slot] = newNode;
		
		return nullptr;
	}

	
	unsigned long size;		/* size of hash table array */
	unsigned long nElems;	/* number of elements */
	double resizeFactor;	/* when to resize */
	double growthFactor;	/* how much to grow during resize*/
	HMHashFunc hashFn;		/* hash on key */
	HMEqualFunc equalFn;	/* comparison(key, key) */
	ListNode** table;		/* head of chain lists */
};
