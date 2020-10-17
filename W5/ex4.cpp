//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week 4 Lab Exercise                      //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<stdio.h>

using namespace std;

struct tree_node
{
	int contents;
	tree_node* left;
	tree_node* right;
};

tree_node* insert(int value, tree_node* node);
tree_node* create_node(int value);
void inorder(tree_node* root);

int counter{};

int main()
{
	//display a prompt for file name
	cout << "Please enter an input file name : ";

	//read in file name
	string fileName;
	getline(cin,fileName);

	//open file, and check status
	ifstream inputFile;
	inputFile.open(fileName);
	if(!inputFile)
	{
		cerr << "Could not open " << fileName << endl;
		return -1;
	}

	//read ints from file into array "data"
	int value;
	tree_node* root = NULL;
	while(inputFile >> value)
	{
		root = insert(value, root);
	}

	//close file
	inputFile.close();

	//inorder traversal of BST
	cout << "Inorder traversal of BST :" << endl;
	inorder(root);
	cout << endl;

	return 0;
}

tree_node* insert(int value, tree_node* node)
{
	if(node == NULL)
	{
		return create_node(value);
	}

	if(value == node->contents)
	{
		return node;
	}
	else if(value < node->contents)
	{
		node->left = insert(value, node->left);
	}
	else
	{
		node->right = insert(value, node->right);
	}
}

tree_node* create_node(int value)
{
	struct tree_node* start = new tree_node();
	start->contents = value;
	start->left = NULL;
	start->right = NULL;
	return start;
}

void inorder(tree_node* node)
{
	if(node != NULL)
	{
		inorder(node->left);
		if(counter > 9)
		{
			cout << "\n";
			counter = 0;
		}
		printf("%5d",node->contents);
		counter += 1;
		inorder(node->right);
	}
}
