#include "nonius/nonius_single.h++"

extern const char* CUniqueStr(const char* str);
extern const char* USUniqueStr(const char* str, bool new_item);
extern const char* NSUniqueStr(const char* str, bool new_item);
extern const char* NSLPUniqueStr(const char* str, bool new_item);

// Generate a random string of a given length
std::string RandomAlphaNumericString(int length)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> alphaNumericChar(0, 61);

	std::string result;

	for (int i = 0; i < length; i++)
	{
		int c = alphaNumericChar(gen);
		result.push_back(c <= 9 ? c + '0' : (c <= 35 ? c - 10 + 'A' : c - 36 + 'a'));
	}

	return result;
}



NONIUS_BENCHMARK("CHashMap: Insert", [](nonius::chronometer meter)
{
	std::deque<std::string> randomStringTable;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> length(4, 15);

	for (int i = 0; i < 5000; i++)
		randomStringTable.push_back(RandomAlphaNumericString(length(gen)));

	meter.measure([&] {
		for (auto& str : randomStringTable)
			CUniqueStr(str.c_str());
	});

	return;
}
);

NONIUS_BENCHMARK("CHashMap: Find", [](nonius::chronometer meter)
{
	std::deque<std::string> randomStringTable;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> length(4, 15);

	for (int i = 0; i < 5000; i++)
		randomStringTable.push_back(RandomAlphaNumericString(length(gen)));

	// preload the table
	for (auto& str : randomStringTable)
		CUniqueStr(str.c_str());

	// now time the finds
	meter.measure([&] {
		for (auto& str : randomStringTable)
			CUniqueStr(str.c_str());
	});

	return;
}
);



NONIUS_BENCHMARK("NSet: Insert", [](nonius::chronometer meter)
{
	std::deque<std::string> randomStringTable;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> length(4, 15);

	for (int i = 0; i < 5000; i++)
		randomStringTable.push_back(RandomAlphaNumericString(length(gen)));

	meter.measure([&] {
		for (auto& str : randomStringTable)
			NSUniqueStr(str.c_str(), true);
	});

	return;
}
);

NONIUS_BENCHMARK("NSet: Find", [](nonius::chronometer meter)
{
	std::deque<std::string> randomStringTable;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> length(4, 15);

	for (int i = 0; i < 5000; i++)
		randomStringTable.push_back(RandomAlphaNumericString(length(gen)));

	// preload the table
	for (auto& str : randomStringTable)
		NSUniqueStr(str.c_str(), true);

	// now time the finds
	meter.measure([&] {
		for (auto& str : randomStringTable)
			NSUniqueStr(str.c_str(), false);
	});

	return;
}
);



NONIUS_BENCHMARK("NSet w/ Linear Probe: Insert", [](nonius::chronometer meter)
{
	std::deque<std::string> randomStringTable;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> length(4, 15);

	for (int i = 0; i < 5000; i++)
		randomStringTable.push_back(RandomAlphaNumericString(length(gen)));

	meter.measure([&] {
		for (auto& str : randomStringTable)
			NSLPUniqueStr(str.c_str(), true);
	});

	return;
}
);

NONIUS_BENCHMARK("NSet w/ Linear Probe: Find", [](nonius::chronometer meter)
{
	std::deque<std::string> randomStringTable;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> length(4, 15);

	for (int i = 0; i < 5000; i++)
		randomStringTable.push_back(RandomAlphaNumericString(length(gen)));

	// preload the table
	for (auto& str : randomStringTable)
		NSLPUniqueStr(str.c_str(), true);

	// now time the finds
	meter.measure([&] {
		for (auto& str : randomStringTable)
			NSLPUniqueStr(str.c_str(), false);
	});

	return;
}
);



NONIUS_BENCHMARK("std::unordered_set: Insert", [](nonius::chronometer meter)
{
	std::deque<std::string> randomStringTable;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> length(4, 15);

	for (int i = 0; i < 5000; i++)
		randomStringTable.push_back(RandomAlphaNumericString(length(gen)));

	meter.measure([&] {
		for (auto& str : randomStringTable)
			USUniqueStr(str.c_str(), true);
	});

	return;
}
);

NONIUS_BENCHMARK("std::unordered_set: Find", [](nonius::chronometer meter)
{
	std::deque<std::string> randomStringTable;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> length(4, 15);

	for (int i = 0; i < 5000; i++)
		randomStringTable.push_back(RandomAlphaNumericString(length(gen)));

	// preload the table
	for (auto& str : randomStringTable)
		USUniqueStr(str.c_str(), true);

	// now time the finds
	meter.measure([&] {
		for (auto& str : randomStringTable)
			USUniqueStr(str.c_str(), false);
	});

	return;
}
);







int main(int argc, char** argv)
{
	return nonius::main(argc, argv);
}