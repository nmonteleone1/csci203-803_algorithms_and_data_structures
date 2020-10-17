//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week 2 Lab Exercise                      //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>

using namespace std;

string words[100];
int wordIndex{-1};

void push(string word);
void pop();
string top();
bool isEmpty();

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

	//read word from file and add to stack
	string temp;
	while(inputFile >> temp)
	{
		push(temp);
	}

	//close file
	inputFile.close();

	//check stack and display top word
	while(wordIndex>-1)
	{
		cout << top() << " ";
		pop();
	}

	return 0;
}

void push(string word)
{
	wordIndex += 1;
	words[wordIndex] = word;
}

void pop()
{
	wordIndex -= 1;
}

string top()
{
	return words[wordIndex];
}

bool isEmpty()
{
	if(wordIndex==-1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
