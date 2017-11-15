#include <iostream>
#include "RBTree.h"
using namespace std;

int main()
{
	RBTree<char> tree;
	char value[] = {'A', 'S', 'E', 'R', 'C', 'D', 'I', 'N', 'B', 'X'};
	for (int i = 0; i < sizeof(value)/sizeof(value[0]); ++i){
		tree.insert(new Comparable<char>(value[i]));
		tree.DumpTree();
		cout << endl << "*******************************************************************************" << endl;
	}

	Comparable<char>* found = tree.search('C');
	if (found) {
		cout << found->Key() << endl;
	}

	tree.remove('X');
	tree.DumpTree();
	cout << endl << "*******************************************************************************" << endl;

	tree.remove('S');
	tree.DumpTree();
	cout << endl << "*******************************************************************************" << endl;

	tree.remove('I');
	tree.DumpTree();
	cout << endl << "*******************************************************************************" << endl;

	tree.remove('R');
	tree.DumpTree();
	cout << endl << "*******************************************************************************" << endl;

	tree.remove('B');
	tree.DumpTree();
	cout << endl << "*******************************************************************************" << endl;
	return 0;
}