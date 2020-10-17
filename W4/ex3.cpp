//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week 4 Lab Exercise                      //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>

using namespace std;

int data[100];
int forwardCheck[100];
int backwardCheck[100];
int counter{0};

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

	//SEQUENCE ONE : read ints from file and initialize into array "data"
	int what, where;
	while(inputFile >> what && inputFile >> where)
	{
		//if data pair is -1 -1, break this sequence
		if(what == -1 && where == -1)
		{
			break;
		}
		counter += 1;
		data[where] = what;
		forwardCheck[counter] = where;
		backwardCheck[where] = counter;
	}

	//SEQUENCE TWO : test if an array location has been initialized
	int probe;
	while(inputFile >> probe)
	{
		//if test location is -1, break sequence
		if(probe == -1)
		{
			break;
		}
		if(backwardCheck[probe] > 0 && backwardCheck[probe] <= counter && forwardCheck[backwardCheck[probe]] == probe)
		{
			cout << "Position " << probe << " has been initialized to value " << data[probe] << ". " << endl;
		}
		else
		{
			cout << "Position " << probe << " has not been initalized. " << endl;
		}
	}

	//close file
	inputFile.close();

	return 0;
}
