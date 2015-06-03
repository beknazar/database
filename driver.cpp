#include <iostream>
#include <string>
#include "BplusTree.h"
using namespace std;

int main()
{
    void *ret; 
    BplusTree tree; 

    for(int i=0;i<100;i++) {

        tree.insertData(i, (void*) i); 

        if( i>10 && i%5==1 ) tree.deleteData(i);
    }

    ret = tree.searchData(21);
    if(ret != NULL) cout << ret << endl;

    ret = tree.searchData(25);
    if(ret != NULL) cout << ret << endl;

    cout << "Tree Height: " << tree.getTreeHeight() << endl;

    tree.saveToFile("./bplustree.idx"); 

    for(int i=0;i<100;i++) {
        tree.deleteData(i);
    }

    tree.readFromFile("./bplustree.idx"); 

    ret = tree.searchData(21);
    if(ret != NULL) cout << ret << endl;

    ret = tree.searchData(25);
    if(ret != NULL) cout <<  ret << endl;

}
