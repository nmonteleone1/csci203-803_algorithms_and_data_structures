//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week 7 Lab Exercise                      //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;

//structures

//functions
int hash_me(char str[], int len);
void KRSearch(char target[], char search[]);
int roll(int hash, int pref, int suff, int q);

//global variables
int prime = 17;	//17 was chosen as it is the lowest prime higher than the alphabet of 4 characters, and the word size of 10 characters
int index_of_match[5000];
int valid_count = 0;

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
	char target[5000];
	char search[10];
	inputFile.getline(target,5000);
	inputFile.getline(search,10);

	//close file
	inputFile.close();

	//complete karp-rabin search
	KRSearch(target, search);

	//output matching locations
	for(int i = 0; i < valid_count; i++)
	{
		cout << index_of_match[i] << " ";
	}
	cout << endl;

	return 0;
}

int hash_me(char str[], int len)
{
	int hashed = prime*str[0] + str[1];
	for(int i = 2; i < len; i++)
	{
		hashed = str[i] + prime*hashed;
	}
	return hashed;
}

void KRSearch(char target[], char search[])
{
	//find length of the sequences
	int search_length;
        int target_length;
        for(search_length = 0; search[search_length+1] != '\0'; search_length++);
        for(target_length = 0; target[target_length+1] != '\0'; target_length++);

	//compute initial hash of sequences
	int hash_s = hash_me(search, search_length);
	int hash_t = hash_me(target, search_length);
	int prev_power = pow(prime, search_length-1);

	for(int i = 0; i <= target_length-search_length; i++)
	{
		if(hash_s == hash_t)
		{
			for(int j = 0; j < search_length; j++)
			{
				if(search[j] != target[i+j])
				{
					break;
				}
				if(j == search_length-1)
				{
					index_of_match[valid_count++] = i;
				}
			}
		}
		hash_t = roll(hash_t,target[i], target[i+search_length], prev_power);
	}
}

int roll(int hash, int pref, int suff, int q)
{
	return prime * (hash - q * pref) + suff;
}
