//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week 6 Lab Exercise                      //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>

using namespace std;

//structures
struct Node
{
	int value;		//for this example assume 0 is empty, 1 is full
	Node* next;

	Node() {value = 0;}
};

//functions
int hash_me(int key);
Node* create_node(Node* node, Node* parent);

//global variables
Node* hash_table[100];

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

	//read ints from file and initialize into nodes, linking where required
	int key;
	int hashed_key;
	while(inputFile >> key)
	{
		hashed_key = hash_me(key);
		Node* new_node = new Node();

		hash_table[hashed_key] = create_node(new_node, hash_table[hashed_key]);
	}

	//close file
	inputFile.close();

	//count entries, and longest chain depth
	int full_counter{0};
	int depth_counter{0};
	int temp_counter{0};
	Node* node_check;
	for(int i = 0; i < 100; i++)
	{
		temp_counter = 0;
		if(hash_table[i] != NULL)
		{
			full_counter += 1;
			node_check = hash_table[i]->next;
			while(node_check != NULL)
			{
				temp_counter += 1;
				node_check = node_check->next;
			}
		}
		if(temp_counter > depth_counter)
		{
			depth_counter = temp_counter;
		}
	}

	//output number of empty entries, and length of longest chain
	cout << 100-full_counter << " " << depth_counter << endl;

	return 0;
}

Node* create_node(Node* node, Node* parent)
{
	if(parent == NULL)		//assign to reference node if non-existent
	{
		node->value = 1;
		node->next = NULL;
		return node;
	}
	if(parent->value == 1)		//assign to next node in chain if full
	{
		parent->next = create_node(node, parent->next);
	}
	else				//assign if value is not full
	{
		node->value = 1;
		node->next = NULL;
		return node;
	}
}

int hash_me(int key)
{
	return key%100;
}
