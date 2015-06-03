#include <iostream>
#include <string>
#include "BplusTree.h"
using namespace std;

TreeNode::TreeNode(int l, bool leafl) {
	level = l;
	leaf = leafl;

	for (int i = 0; i < FANOUT-1; ++i) key[i] = 0;
	for (int i = 0; i < FANOUT; ++i) ptr[i] = NULL;

	numKeys = 0;
}

void TreeNode::insertNFull (int key) {
	int i = numKeys - 1;

	if (leaf) {
		while (i >= 0 && keys[i] > key) {
            keys[i+1] = keys[i];
            --i;
        }
 
        keys[i+1] = key;
        ++numKeys;
	}
	else {
		while (i >= 0 && keys[i--] > key);

        if (ptr[i+1] -> numKeys == FANOUT-1) {
            splitChild(i+1, ptr[i+1]);
 
            if (keys[i+1] < key)
                i++;
        }
        ptr[i+1] -> insertNonFull(k);
	}
}

void TreeNode::splitChild (int i, TreeNode* node) {
    TreeNode *newNode = new BTreeNode(node -> t, node -> leaf);
    newNode -> numKeys = FANOUT - 1;
 
    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];
 
    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }
 
    // Reduce the number of keys in y
    y->n = t - 1;
 
    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];
 
    // Link the new child to this node
    C[i+1] = z;
 
    // A key of y will move to this node. Find location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];
 
    // Copy the middle key of y to this node
    keys[i] = y->keys[t-1];
 
    // Increment count of keys in this node
    n = n + 1;
}

TreeNode *TreeNode::search (int key) {

    int i = 0;
    while (i < numKeys && key > keys[i])
        i++;
 
    if (keys[i] == key)
        return this;
 
    if (leaf)
        return NULL;
 
    return ptr[i]->search(key); // recursive call to its proper child
}


BplusTree::BplusTree() {
	root = NULL;
}

BplusTree::BplusTree(int _t) {
	root = NULL;
}

void BplusTree::insertData(int key, void *value) {  // insert function
	TreeNode* node = treeSearch(key, root);
	if (node -> numKeys + 1 <= FANOUT - 1) {
		node -> key[numKeys] = key;
		node -> numKeys++;
	}
	else {
		int newNumKeys = (node -> numKeys + 1) / 2;
		TreeNode* newLeaf = new TreeNode(node -> level, newNumKeys);
		for (int i = 0; i < newNumKeys; ++i) 
	}
}
void BplusTree::deleteData(int key) {    // delete function

}
void* BplusTree::searchData(int key) {  // search function (return the found value or null if not found)
	TreeNode* node = treeSearch(key, root);
	for (int i = 0; i < node -> numKeys; ++i) {
		if (key == node -> key[i]) return node -> ptr[i];
	}
	return NULL;
}

void* treeSearch (int key, TreeNode* node) {
	if (node -> level == getTreeHeight() - 1) { // if node is leaf, return
		return node;
	}
	for (int i = node -> numKeys; i > 0; --i) {
		if (key > node -> key[i - 1]) return treeSearch(key, node -> ptr[i]);
	}
	return treeSearch(key, node -> ptr[0]);
}

int BplusTree::searchPathCount(int key);  // search function (return how many tree nodes were accessed 
                             // to find the query data (root node access should be counted as 0)

void BplusTree::readFromFile(char* fileName); // read B+-Tree from a file 
void BplusTree::saveToFile(char* fileName); // save B+-Tree in a file 

int BplusTree::getTreeHeight();  // return the height of the B+-Tree