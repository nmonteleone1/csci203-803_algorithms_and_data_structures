//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week 3 Lab Exercise                      //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>

using namespace std;

int heap[100];
int finalIndex{-1};

void siftdown(int i);
void makeheap();

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

	//read int from file and add to array
	int temp;
	while(inputFile >> temp && finalIndex <= 100)
	{
		finalIndex += 1;
		heap[finalIndex] = temp;
	}

	//close file
	inputFile.close();

	//make array into a heap
	makeheap();

	//display elements 1 to 5
	for(int i = 1; i < 6; i++)
	{
		cout << heap[i] << " ";
	}

	cout << endl;

	return 0;
}

void siftdown(int i)
{
	int j;
	if(i == 0)
	{
		j = 1;
	}
	else
	{
		j = i * 2;
	}

	if(j >= finalIndex)
	{
		return;
	}
	if(heap[j] < heap[j+1])
	{
		j = j + 1;
	}
	if(heap[i] < heap[j])
	{
		int temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;


		siftdown(j);
	}
}

void makeheap()
{
	for(int i = finalIndex/2; i >= 0; i--)
	{
		siftdown(i);
	}
}
