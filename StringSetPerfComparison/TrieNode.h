#pragma once

#include <vector>

typedef struct TrieNode {
	// This is the most flexible trie node, allowing all ASCII characters between space & tilde, effectively including every typable character.
	// Of course it uses the most memory: 95 bytes per node.  It could be refactored so that the alphabet size and the base character are
	// template parameters, or even constructor parameters, but I opted not to do that here.
	static const size_t ALPHABET_SIZE = '~' - ' ' + 1;
	static const char BASE_CHAR = ' ';

	/// Converts a given character to the index in the children array for that character
	static int toChildIndex(char c);

	TrieNode();

	/// The trie will be stored in an array.  The root node is array[0].  The children
	/// array here will have elements which are either -1 (no children for that letter)
	/// or an index into the tree array container.
	std::vector<int> children;

	/// True when this node is a leaf, false otherwise
	bool isLeaf;
} TrieNode;
