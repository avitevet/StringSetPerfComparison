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
std::pair<LONGLONG, size_t> stdSearch(const T &set, const std::vector<std::string> &searchStrings) {
	LARGE_INTEGER startTick, endTick;
	auto setEnd = set.end();
	size_t numFound = 0;

	QueryPerformanceCounter(&startTick);
	for (auto &s : searchStrings) {
		numFound += set.find(s) != setEnd;
	}
	QueryPerformanceCounter(&endTick);

	return std::make_pair(endTick.QuadPart - startTick.QuadPart, numFound);
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

std::pair<LONGLONG, size_t> trieSearch(const trie &t, const std::vector<std::string> &searchStrings) {
	LARGE_INTEGER startTick, endTick;
	size_t numFound = 0;

	QueryPerformanceCounter(&startTick);
	for (auto &s : searchStrings) {
		numFound += t.contains(s);
	}
	QueryPerformanceCounter(&endTick);

	return std::make_pair(endTick.QuadPart - startTick.QuadPart, numFound);
}


int main(int argc, char ** argv) {
	if (argc != 3) {
		// user must pass arguments to the executable that are files containing the set of strings to store in the DS,
		// and the set of strings that will be searched for in the DS after the strings in the storefile have been stored
		std::cout << "Usage: " << std::endl;
		std::cout << argv[0] << " <storefile> <searchfile>" << std::endl;
		std::cout << "Called: ";
		for (int i = 0; i < argc; ++i) {
			std::cout << argv[i] << " ";
		}
		std::cout << std::endl;
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
	size_t numFound[NUM_DATA_STRUCTURES];

	// first perform tests on set
	std::set<std::string> set;
	std::pair<LONGLONG, size_t> result;
	insertTicks[SET].QuadPart = stdInsert<std::set<std::string>>(set, storeStrings);
	result = stdSearch<std::set<std::string>>(set, searchStrings);
	searchTicks[SET].QuadPart = result.first;
	numFound[SET] = result.second;

	// now unordered_set
	std::unordered_set<std::string> unordered_set;
	insertTicks[UNORDERED_SET].QuadPart = stdInsert<std::unordered_set<std::string>>(unordered_set, storeStrings);
	result = stdSearch<std::unordered_set<std::string>>(unordered_set, searchStrings);
	searchTicks[UNORDERED_SET].QuadPart = result.first;
	numFound[UNORDERED_SET] = result.second;

	// now trie
	trie t;
	insertTicks[TRIE].QuadPart = trieInsert(t, storeStrings);
	result = trieSearch(t, searchStrings);
	searchTicks[TRIE].QuadPart = result.first;
	numFound[TRIE] = result.second;

	// output CSV rows
	// std::cout << "insertfile,searchfile,insertwords,searchwords,setfound,uosfound,triefound,setinsert,uosinsert,trieinsert,setsearch,uossearch,triesearch" << std::endl;
	std::cout << argv[1] << "," << argv[2] << ","
		<< storeStrings.size() << "," << searchStrings.size() << "," 
		<< numFound[SET] << "," << numFound[UNORDERED_SET] << "," << numFound[TRIE] << ","
		<< insertTicks[SET].QuadPart << "," << insertTicks[UNORDERED_SET].QuadPart << "," << insertTicks[TRIE].QuadPart << "," 
		<< searchTicks[SET].QuadPart << "," << searchTicks[UNORDERED_SET].QuadPart << "," << searchTicks[TRIE].QuadPart 
		<< std::endl;
}