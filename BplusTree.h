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
  	int numKeys ; // number of keys in node  
  	int key[FANOUT-1] ; // the values in node
  	void *ptr[FANOUT] ; // pointers to child nodes or data if is a leaf node
    bool leaf;

    TreeNode* parentNode;
    TreeNode* leftSibling;
    TreeNode* rightSibling;

    void setValue (int index, void* value) {
      ptr[index] = value;
    }

    void* getValue (int index) {
      return ptr[index];
    }


    void insertAt(int index, int key, TreeNode* leftChild, TreeNode* rightChild) {
      for (int i = getNumKeys() + 1; i > index; --i) {
        setChild(i, getChild(i - 1));
      }
      for (int i = getNumKeys(); i > index; --i) {
        setKey(i, getKey(i - 1));
      }
      
      setKey(index, key);
      setChild(index, leftChild);
      setChild(index + 1, rightChild);
      numKeys++;
    }

    void insertKey (int key, void* value) {
      int index = 0;
      while (index < getNumKeys() && getKey(index) < key)
        ++index;
      insertAt(index, key, value);
    }

    void insertAt (int index, int key, void* value) {
      for (int i = getNumKeys() - 1; i >= index; --i) {
        setKey(i + 1, getKey(i));
        setValue(i + 1, getValue(i));
      }
      
      setKey(index, key);
      setValue(index, value);
      ++numKeys;
    }

    void deleteAt(int index) {
      int i = 0;
      for (i = index; i < getNumKeys() - 1; ++i) {
        setKey(i, getKey(i + 1));
        setChild(i + 1, getChild(i + 2));
      }
      setKey(i, 0);
      setChild(i + 1, NULL);
      --numKeys;
      return;
    }

        // you may add any private members here if you need
  public:
        TreeNode() {
          level = 0;
          numKeys = 0;
          leaf = 0;
          parentNode = NULL;
          leftSibling = NULL;
          rightSibling = NULL;
        }

        ~TreeNode() {

        }

        int getNumKeys() {
          return numKeys;
        }

        int getKey(int index) {
          return key[index];
        }

        void setKey(int index, int k) {
          key[index] = k;
        }

        TreeNode* getParent() {
          return parentNode;
        }
        
        void setParent(TreeNode* node) {
          parentNode = node;
        }

        int search(int key) {
          if (leaf) {
            for (int i = 0; i < getNumKeys(); ++i) {
              if (getKey(i) == key) return i;
              else if (getKey(i) > key) return -1;
            }
            return -1;
          }
          int i = 0;
          for (i = 0; i < getNumKeys(); ++i) {
            if (getKey(i) == key) return i + 1;
            else if (getKey(i) > key) return i;
          }
          return i;
        }

        bool isOverflow() {
          return getNumKeys() == FANOUT-1;
        }

        TreeNode* dealOverflow() {
          int midIndex = getNumKeys() / 2;
          int upKey = getKey(midIndex);

          TreeNode* newRNode = split();

          if (getParent() == NULL) {
            setParent(new TreeNode());
          }
          newRNode->setParent(getParent());

          newRNode->setLeftSibling(this);
          newRNode->setRightSibling(rightSibling);
          if (getRightSibling() != NULL)
            getRightSibling()->setLeftSibling(newRNode);
          setRightSibling(newRNode);
          
          return getParent() -> pushUpKey(upKey, this, newRNode);
        }

        TreeNode* split() {
          int midIndex = getNumKeys() / 2;
          TreeNode* newRNode = new TreeNode();

          if (leaf) { // if its leaf node
            newRNode -> leaf = 1;
            for (int i = midIndex; i < getNumKeys(); ++i) {
              newRNode->setKey(i - midIndex, getKey(i));
              newRNode->setValue(i - midIndex, getValue(i));
              setKey(i, 0);
              setValue(i, NULL);
            }
            newRNode -> leaf = 0;
            newRNode -> numKeys = getNumKeys() - midIndex;
            numKeys = midIndex;
            
            return newRNode;
          }
          for (int i = midIndex + 1; i < getNumKeys(); ++i) {
            newRNode -> setKey(i - midIndex - 1, getKey(i));
            setKey(i, 0);
          }
          for (int i = midIndex + 1; i <= getNumKeys(); ++i) {
            newRNode->setChild(i - midIndex - 1, getChild(i));
            newRNode->getChild(i - midIndex - 1)->setParent(newRNode);
            setChild(i, NULL);
          }
          setKey(midIndex, 0);
          newRNode->numKeys = getNumKeys() - midIndex - 1;
          numKeys = midIndex;
          
          return newRNode;
        }

        TreeNode* pushUpKey(int key, TreeNode* leftChild, TreeNode* rightNode) {
          int index = search(key);
          
          insertAt(index, key, leftChild, rightNode);

          if (isOverflow()) {
            return dealOverflow();
          }
          return getParent() == NULL ? this : NULL;
        }

        // For deletion part
        bool isUnderflow() {
          return getNumKeys() < ((FANOUT-1) / 2);
        }
        
        bool canLendAKey() {
          return getNumKeys() > ((FANOUT-1) / 2);
        }
        
        TreeNode* getLeftSibling() {
          if (leftSibling != NULL && leftSibling -> getParent() == getParent())
            return leftSibling;
          return NULL;
        }

        void setLeftSibling(TreeNode* sibling) {
          leftSibling = sibling;
        }

        TreeNode* getRightSibling() {
          if (rightSibling != NULL && rightSibling -> getParent() == getParent())
            return rightSibling;
          return NULL;
        }

        void setRightSibling(TreeNode* silbling) {
          rightSibling = silbling;
        }

        bool delete(int key) {
          int index = search(key);
          if (index == -1)
            return 0;
          
          deleteAt(index);
          return 1;
        }

        void setChild(int index, TreeNode* candidateChild) {
          ptr[index] = candidateChild;
          if (candidateChild != NULL)
            candidateChild -> setParent(this);
        }

        TreeNode* getChild(int index) {
          return (TreeNode*) ptr[index];
        }
        
        TreeNode* dealUnderflow() {
          if (getParent() == NULL)
            return NULL;
          
          TreeNode* leftSibling = getLeftSibling();
          if (leftSibling != NULL && leftSibling -> canLendAKey()) {
            getParent()->processChildrenTransfer(this, leftSibling, leftSibling -> getNumKeys() - 1);
            return NULL;
          }
          
          TreeNode* rightSibling = getRightSibling();
          if (rightSibling != NULL && rightSibling -> canLendAKey()) {
            getParent()->processChildrenTransfer(this, rightSibling, 0);
            return NULL;
          }
          
          if (leftSibling != NULL) {
            return getParent()->processChildrenFusion(leftSibling, this);
          }
          return getParent()->processChildrenFusion(this, rightSibling);
        }

        void processChildrenTransfer(TreeNode* borrower, TreeNode* lender, int borrowIndex) {
          int borrowerChildIndex = 0;
          while (borrowerChildIndex < getNumKeys() + 1 && getChild(borrowerChildIndex) != borrower)
            ++borrowerChildIndex;
          
          if (borrowIndex == 0) {
            int upKey = borrower -> transferFromSibling(getKey(borrowerChildIndex), lender, borrowIndex);
            setKey(borrowerChildIndex, upKey);
          }
          else {
            int upKey = borrower -> transferFromSibling(getKey(borrowerChildIndex - 1), lender, borrowIndex);
            setKey(borrowerChildIndex - 1, upKey);
          }
        }
        
        TreeNode* processChildrenFusion(TreeNode* leftChild, TreeNode* rightChild) {
          int index = 0;
          while (index < getNumKeys() && getChild(index) != leftChild)
            ++index;
          int sinkKey = getKey(index);
          
          leftChild -> fusionWithSibling(sinkKey, rightChild);
          
          deleteAt(index);
          
          if (isUnderflow()) {
            if (getParent() == NULL) {
              if (getNumKeys() == 0) {
                leftChild -> setParent(NULL);
                return leftChild;
              }
              else {
                return NULL;
              }
            }
            
            return dealUnderflow();
          }
          
          return NULL;
        }

        void fusionWithSibling(int sinkKey, TreeNode* rightSibling) {
          int j = getNumKeys();
          if (leaf) {
            TreeNode* siblingLeaf = rightSibling;
            siblingLeaf -> leaf = 1;

            for (int i = 0; i < siblingLeaf->getNumKeys(); ++i) {
              setKey(j + i, siblingLeaf->getKey(i));
              setValue(j + i, siblingLeaf->getValue(i));
            }
            numKeys += siblingLeaf->getNumKeys();
            
            setRightSibling(siblingLeaf->getRightSibling());
            if (siblingLeaf->getRightSibling() != NULL)
              siblingLeaf->getRightSibling()->setLeftSibling(this);
            return;
          }

          TreeNode* rightSiblingNode = rightSibling;
          rightSiblingNode -> leaf = 0;
    
          setKey(j++, sinkKey);
          
          for (int i = 0; i < rightSiblingNode->getNumKeys(); ++i) {
            setKey(j + i, rightSiblingNode->getKey(i));
          }
          for (int i = 0; i < rightSiblingNode->getNumKeys() + 1; ++i) {
            setChild(j + i, rightSiblingNode->getChild(i));
          }
          numKeys += 1 + rightSiblingNode -> getNumKeys();
          
          setRightSibling(rightSiblingNode -> getRightSibling());
          if (rightSiblingNode -> getRightSibling() != NULL)
            rightSiblingNode -> getRightSibling() -> setLeftSibling(this);
        }
  
        int transferFromSibling(int sinkKey, TreeNode* sibling, int borrowIndex) {
          TreeNode* siblingNode = sibling;
          if (leaf) {
            siblingNode -> leaf = 1;

            insertKey(siblingNode -> getKey(borrowIndex), siblingNode -> getValue(borrowIndex));
            siblingNode -> deleteAt(borrowIndex);
            
            return borrowIndex == 0 ? sibling -> getKey(0) : getKey(0);
          }

          siblingNode -> leaf = 0;
          int upKey = 0;
          if (borrowIndex == 0) {
            int index = getNumKeys();
            setKey(index, sinkKey);
            setChild(index + 1, siblingNode -> getChild(borrowIndex));      
            numKeys += 1;
            
            upKey = siblingNode -> getKey(0);
            siblingNode -> deleteAt(borrowIndex);
          }
          else {
            insertAt(0, sinkKey, siblingNode -> getChild(borrowIndex + 1), getChild(0));
            upKey = siblingNode -> getKey(borrowIndex);
            siblingNode -> deleteAt(borrowIndex);
          }
          
          return upKey;
        }
        
friend class BplusTree;
        // you may add public members here if you need
};

class BplusTree
{
  private:
        TreeNode *root;
        // you may add any private members here if you need

        TreeNode* findLeafNodeShouldContainKey(int key);
        

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
