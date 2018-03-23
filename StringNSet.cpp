#include <cstdlib>
#include "NSet.h"

// Identical to CStringHash()
template<> struct std::hash<const char*>
{
	size_t operator()(const char* str) const noexcept
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
	};
};

// Identical to CStringEqual()
template<> struct std::equal_to<const char*>
{
	bool operator()(const char* lhs, const char* rhs) const noexcept
	{
		return lhs != nullptr && rhs != nullptr && strcmp(lhs, rhs) == 0;
	};
};



NSet<const char*> StringNSet{5931};

const char* NSUniqueStr(const char* str, bool new_item)
{
	if(str == nullptr)
		return nullptr;

	if (new_item)
	{
		char* strCopy = (char*) std::malloc(strlen(str) + 1);
		strcpy(strCopy, str);
		StringNSet.addItem(const_cast<const char*>(strCopy));
		return strCopy;
	}
	
	const NSet<const char*>::Node* node = StringNSet.findItemNode(str);
	if (node == nullptr)
	{
		char* strCopy = (char*) std::malloc(strlen(str) + 1);
		strcpy(strCopy, str);
		StringNSet.addItem(const_cast<const char*>(strCopy));
		return strCopy;
	}
	return node->item_;
}

