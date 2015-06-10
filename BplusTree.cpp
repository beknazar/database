#include <iostream>
#include <string>
#include "BplusTree.h"
using namespace std;




TreeNode* BplusTree::findLeafNodeShouldContainKey (int key) {
	TreeNode* node = root;
	while (!(node -> leaf)) {
		node = node -> getChild(node  -> search(key));
	}
	
	return node;
}


BplusTree::BplusTree() {
	root = new TreeNode();
	root -> leaf = 1;
}

void BplusTree::insertData(int key, void *value) {  // insert function
	TreeNode* leaf = findLeafNodeShouldContainKey(key);
	leaf -> leaf = 1;
	leaf -> insertKey(key, value);

	if (leaf -> isOverflow()) {
		TreeNode* n = leaf -> dealOverflow();
		if (n != NULL) root = n;
	}
}
void BplusTree::deleteData(int key) {    // delete function
	TreeNode* leaf = findLeafNodeShouldContainKey(key);
	leaf -> leaf = 1;

	if (leaf -> delete(key) && leaf -> isUnderflow()) {
		TreeNode* n = leaf -> dealUnderflow();
		if (n != NULL) root = n;
	}
}
void* BplusTree::searchData(int key) {  // search function (return the found value or null if not found)
	TreeNode* leaf = findLeafNodeShouldContainKey(key);
	leaf -> leaf = 1;

	int index = leaf -> search(key);
	return (index == -1) ? NULL : leaf -> getValue (index);
}

// int BplusTree::searchPathCount(int key);  // search function (return how many tree nodes were accessed 
//                              // to find the query data (root node access should be counted as 0)

// void BplusTree::readFromFile(char* fileName); // read B+-Tree from a file 
// void BplusTree::saveToFile(char* fileName); // save B+-Tree in a file 

// int BplusTree::getTreeHeight();  // return the height of the B+-Tree