#include "CHashMap.hpp"

// C version of hash/equal functions
unsigned long CStringHash(const char* str)
{
	unsigned long h = 0;
	unsigned long g;
	for (int i = 0; str[i] != 0; i++)
	{
		h = (h << 4) + str[i];
		g = h & 0xf0000000;
		if (g) h ^= (g >> 28) , h ^= g;
	}
	return h;
}

int CStringEqual(const char* str1, const char* str2)
{
	if ((str1 == nullptr) || (str2 == nullptr))
		return(false);
	
	return  !strcmp(str1, str2) ;
}

CHashMap CStringHashMap(5931, 0.8, 2.3, (HMHashFunc)CStringHash, (HMEqualFunc)CStringEqual);

const char* CUniqueStr(const char* str)
{
	if(str == nullptr)
		return nullptr;

	char* strFound = (char*)CStringHashMap.find(str);
	if (strFound == nullptr)
	{
		strFound = (char*) std::malloc(strlen(str) + 1);
		std::strcpy(strFound, str);
		CStringHashMap.add(strFound, strFound);
	}
	return strFound;
}


