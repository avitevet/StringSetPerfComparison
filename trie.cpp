#include "trie.h"

trie::trie() :
	nodes(1)
{}

void trie::insert(const std::string &s) {
	insert(s.c_str());
}

void trie::insert(const char *s) {
	int currentNode = 0;
	int childNode = 0;
	int childIndex = 0;

	for (; *s != '\0'; ++s) {
		childIndex = TrieNode::toChildIndex(*s);
		childNode = nodes[currentNode].children[childIndex];
		if (childNode != -1) {
			currentNode = childNode;
		}
		else {
			// create a new node
			nodes[currentNode].children[childIndex] = nodes.size();
			currentNode = nodes.size();
			nodes.emplace_back();
		}
	}

	// allows the root node to be a leaf if the empty string is inserted
	nodes[currentNode].isLeaf = true;
}

bool trie::contains(const std::string &s) const {
	return contains(s.c_str());
}

bool trie::contains(const char *s) const {
	int currentNode = 0;
	int childNode = 0;
	int childIndex = 0;

	for (; *s != '\0'; ++s) {
		childIndex = TrieNode::toChildIndex(*s);
		childNode = nodes[currentNode].children[childIndex];
		if (childNode == -1) {
			return false;
		}
		else {
			// go to child node
			currentNode = childNode;
		}
	}

	return nodes[currentNode].isLeaf;
}
