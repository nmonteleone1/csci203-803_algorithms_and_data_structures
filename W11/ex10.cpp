//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week X Lab Exercise                      //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>
#define INT_MAX  2147483647;

using namespace std;

//structures
struct Matrix
{
	int row;
	int col;
};

//functions
int cost(Matrix R[], int i, int j, int k);
int best(Matrix R[], int i, int j);

//global variables

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
	int matrix_count;
	inputFile >> matrix_count;
	Matrix R[matrix_count];

	int row, column;
	int count{0};
	while(inputFile >> row >> column)
	{
		Matrix temp;
		temp.row = row;
		temp.col = column;
		R[count] = temp;
		count += 1;
	}

	//close file
	inputFile.close();

	cout << best(R, 0, matrix_count) << endl;

	return 0;
}

int cost(Matrix R[], int i, int j, int k)
{
	if(i == j || i == k || j == k)
	{
		return 0;
	}
	return R[i].row * R[k-1].col * R[j].col;
}

int best(Matrix R[], int i, int j)
{
	if(i == j)
	{
		return 0;
	}

	int b = INT_MAX;
	int min;

	for(int k = i+1; k < j; k++)
	{
		min = cost(R,i,j,k) + best(R,i,k) + best(R,k,j);
		if(b > min)
		{
			b = min;
		}
	}

	return b;
}
