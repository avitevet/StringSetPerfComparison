#include <windows.h>
#include <iostream>
#include <fstream>
#include <set>
#include <unordered_set>
#include <vector>
#include <string>
#include <stdexcept>

#ifdef _WIN32
#include <io.h> 
#define access    _access_s
#else
#include <unistd.h>
#endif

#include "trie.h"

/// @param filename  Path to a valid readable file
/// @return  A vector of strings - one string for each line in the file, up to but not including the newline
std::vector<std::string> stringsFromFile(std::ifstream &inFile) {
	std::vector<std::string> inStrings;
	std::string s;
	while (inFile.good()) {
		std::getline(inFile, s);
		inStrings.emplace_back(s);
	}

	return inStrings;
}

template <typename T>
LONGLONG stdInsert(T &set, const std::vector<std::string> &insertStrings) {
	LARGE_INTEGER startTick, endTick;
	QueryPerformanceCounter(&startTick);
	set.insert(insertStrings.begin(), insertStrings.end());
	QueryPerformanceCounter(&endTick);

	return endTick.QuadPart - startTick.QuadPart;
}

template <typename T>
LONGLONG stdSearch(const T &set, const std::vector<std::string> &searchStrings) {
	LARGE_INTEGER startTick, endTick;
	auto setEnd = set.end();

	QueryPerformanceCounter(&startTick);
	for (auto &s : searchStrings) {
		set.find(s) != setEnd;
	}
	QueryPerformanceCounter(&endTick);

	return endTick.QuadPart - startTick.QuadPart;
}

LONGLONG trieInsert(trie &t, const std::vector<std::string> &insertStrings) {
	LARGE_INTEGER startTick, endTick;
	QueryPerformanceCounter(&startTick);
	for (auto &s : insertStrings) {
		t.insert(s);
	}
	QueryPerformanceCounter(&endTick);

	return endTick.QuadPart - startTick.QuadPart;
}

LONGLONG trieSearch(const trie &t, const std::vector<std::string> &searchStrings) {
	LARGE_INTEGER startTick, endTick;
	size_t numFound = 0;

	QueryPerformanceCounter(&startTick);
	for (auto &s : searchStrings) {
		numFound += t.contains(s);
	}
	QueryPerformanceCounter(&endTick);

	std::cout << numFound << std::endl;

	return endTick.QuadPart - startTick.QuadPart;
}


int main(int argc, char ** argv) {
	if (argc != 3) {
		// user must pass arguments to the executable that are files containing the set of strings to store in the DS,
		// and the set of strings that will be searched for in the DS after the strings in the storefile have been stored
		std::cout << "Usage: " << std::endl;
		std::cout << argv[0] << " <storefile> <searchfile>" << std::endl;
		return 1;
	}

	// check that the files are valid
	std::vector<std::ifstream> inFiles;
	inFiles.emplace_back(std::ifstream(argv[1]));
	inFiles.emplace_back(std::ifstream(argv[2]));
	for (size_t i = 0; i < inFiles.size(); ++i) {
		auto &inFile = inFiles[i];
		if (!inFile.good()) {
			std::cerr << "The file at " << argv[i + 1] << " could not be read!  Check permissions, and that it exists.";
			return 1;
		}
	}

	std::vector<std::string> storeStrings = stringsFromFile(inFiles[0]);
	std::vector<std::string> searchStrings = stringsFromFile(inFiles[1]);

	// data point 1: time to insert all data from storefile
	// data point 2: time to check presence for every word in the searchfile
	const size_t NUM_DATA_STRUCTURES = 3;
	const size_t SET = 0;
	const size_t UNORDERED_SET = 1;
	const size_t TRIE = 2;
	LARGE_INTEGER insertTicks[NUM_DATA_STRUCTURES], searchTicks[NUM_DATA_STRUCTURES];

	// first perform tests on set
	std::set<std::string> set;
	insertTicks[SET].QuadPart = stdInsert<std::set<std::string>>(set, storeStrings);
	searchTicks[SET].QuadPart = stdSearch<std::set<std::string>>(set, searchStrings);
	
	// now unordered_set
	std::unordered_set<std::string> unordered_set;
	insertTicks[UNORDERED_SET].QuadPart = stdInsert<std::unordered_set<std::string>>(unordered_set, storeStrings);
	searchTicks[UNORDERED_SET].QuadPart = stdSearch<std::unordered_set<std::string>>(unordered_set, searchStrings);

	// now trie
	trie t;
	insertTicks[TRIE].QuadPart = trieInsert(t, storeStrings);
	searchTicks[TRIE].QuadPart = trieSearch(t, searchStrings);

	// create a simple CSV
	std::cout << "datastructure,insertwords,searchwords,insert,search" << std::endl;
	std::cout << "set," << storeStrings.size() << "," << searchStrings.size() << "," << insertTicks[SET].QuadPart << "," << searchTicks[SET].QuadPart << std::endl;
	std::cout << "unordered_set," << storeStrings.size() << "," << searchStrings.size() << "," << insertTicks[UNORDERED_SET].QuadPart << "," << searchTicks[UNORDERED_SET].QuadPart << std::endl;
	std::cout << "trie," << storeStrings.size() << "," << searchStrings.size() << "," << insertTicks[TRIE].QuadPart << "," << searchTicks[TRIE].QuadPart << std::endl;
}