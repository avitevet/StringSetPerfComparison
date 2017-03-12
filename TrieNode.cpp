#include "TrieNode.h"

TrieNode::TrieNode()
	: children(TrieNode::ALPHABET_SIZE, -1), isLeaf(false)
{}

int TrieNode::toChildIndex(char c) {
	return c - TrieNode::BASE_CHAR;
}