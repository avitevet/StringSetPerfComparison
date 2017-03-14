#pragma once

#include <string>
#include <vector>

#include "TrieNode.h"

class trie {
public:
	trie();

	/// Inserts the given string s into the set.  If s is already a member of the set,
	/// does nothing.
	/// @param s  The string to insert
	void insert(const std::string &s);

	/// Inserts the given string s into the set.  If s is already a member of the set,
	/// does nothing.
	/// @param s  The string to insert
	void insert(const char *s);

	/// @param s  The string to check for membership in the set
	/// @return true if the given string s is present in the set, false otherwise.
	bool contains(const std::string &s) const;

	/// @param s  The string to check for membership in the set
	/// @return true if the given string s is present in the set, false otherwise.
	bool contains(const char *s) const;

protected:
	std::vector<TrieNode> nodes;
};