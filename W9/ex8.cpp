//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week 9 Lab Exercise                      //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>

using namespace std;

//structures

//functions
void push(int stack[], int data);
void pop();
void array_doubler(int*& old_stack, int old_size);

//global variables
int count{0};
int stack_size;

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

	//read from file and process
	inputFile >> stack_size;

	int* stack;
	stack = new int[stack_size];

	string stack_action;
	int new_data;
	while(inputFile >> stack_action)
	{
		if(stack_action == "push")
		{
			if(count == stack_size)
			{
				array_doubler(stack, stack_size);
			}
			inputFile >> new_data;
			push(stack, new_data);
		}
		else if(stack_action == "pop")
		{
			if(count == 0)
			{
				continue;
			}
			else
			{
				pop();
			}
		}
		else
		{
			cerr << "unknown stack action" << endl;
			return -1;
		}
	}

	//close file
	inputFile.close();

	cout << "Stack contains " << count << " entries." << endl;

	return 0;
}

void push(int stack[], int data)
{
	count += 1;
	stack[count] = data;
}

void pop()
{
	count -= 1;
}

void array_doubler(int*& old_stack, int old_size)
{
	int new_size = old_size * 2;
	int* new_stack = new int[new_size];

	for(int i = 0; i < old_size; i++)
	{
		new_stack[i] = old_stack[i];
	}

	delete[] old_stack;
	old_stack = new_stack;
	stack_size = new_size;

	cout << "Stack doubled from " << old_size << " to " << new_size << "." << endl;
}
