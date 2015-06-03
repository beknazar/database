#ifndef __BPT_H__
#define __BPT_H__

// for debugging purpose a small page size will be helpful.
#define PGSIZE 100  

// When you are done with debugging, 
// change the PGSIZE to 4K and see if it makes performance difference.
//#define PGSIZE 4096 

#define FANOUT ((PGSIZE-2*sizeof(int)-sizeof(void*))/(sizeof(int)+sizeof(void*))) + 1

class TreeNode
{
  private:
  	int level;  // level 
  	int numKeys ; // number of keys in this node  
  	int key[FANOUT-1] ; // the values in this node
  	void *ptr[FANOUT] ; // pointers to child nodes or data if this is a leaf node
    bool leaf;

        // you may add any private members here if you need
  public:
        TreeNode() {

        }

        TreeNode(int l, bool leafl) {
          level = l;
          leaf = leafl;

          for (int i = 0; i < FANOUT-1; ++i) key[i] = 0;
          for (int i = 0; i < FANOUT; ++i) ptr[i] = NULL;

          numKeys = 0;
        }

        ~TreeNode() {

        }

        // you may add public members here if you need
};

class BplusTree
{
  private:
        TreeNode *root;
        // you may add any private members here if you need

        void* treeSearch (int key, TreeNode* node);

  public:
        BplusTree(); // initialize a new B+-Tree instance.
        ~BplusTree(); // destruct B+-Tree instance.

        void insertData(int key, void *value);  // insert function
        void deleteData(int key);    // delete function
        void* searchData(int key);  // search function (return the found value or null if not found)
        int searchPathCount(int key);  // search function (return how many tree nodes were accessed 
                                     // to find the query data (root node access should be counted as 0)

        void readFromFile(char* fileName); // read B+-Tree from a file 
        void saveToFile(char* fileName); // save B+-Tree in a file 

        int getTreeHeight();  // return the height of the B+-Tree
};

#endif
