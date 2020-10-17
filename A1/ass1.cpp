//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Assignment 1                             //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>

using namespace std;

char words[50000*5];
int wordStart[50000];
int wordEnd[50000];
int wordCount[50000];
int charCounter{-1};
int wordCounter{-1};

bool stringCompare(char str1[], char str2[], int length);

void siftdown(int i, int length);

void makeheap();

void heapsort();

void reverseArrays(int length);

int main(int argc, char *argv[])
{
	//check for input error
	if(argc != 2)
	{
		cerr << "usage : ./<program_name> <input_file>" << endl;
		return -1;
	}

	//open file, and check status
	ifstream inputFile;
	inputFile.open(argv[1]);
	if(!inputFile)
	{
		cerr << "Could not open " << argv[1] << endl;
		return -1;
	}

	//read word from file and add to stack
	char temp;
	char tempWord[100];
	int wordLength{0};
	bool oldWord{};
	char string1[100];
	char string2[100];
	while(inputFile.get(temp))
	{
		if(isspace(temp)) //used rather than making a series of comparisons
		{
			if(wordLength > 0)	//check the word has any characters
			{
				oldWord = false;
				for(int i = 0; i <= wordCounter; i++)
				{
					if(wordLength == wordEnd[i]-wordStart[i])
					{
						for(int j = 0; j < wordLength; j++)
						{
							string1[j] = tempWord[j];
							string2[j] = words[wordStart[i]+j];
						}
						string1[wordLength] = '\0';
						string2[wordLength] = '\0';
						oldWord = stringCompare(string1,string2,wordLength);
						if(oldWord)
						{
							wordCount[i] += 1;
							break;
						}
					}
					else
					{
						continue;
					}
				}
				if(oldWord)
				{
					wordLength = 0;
					continue;
				}

				wordCounter += 1;
       	                        wordStart[wordCounter] = wordEnd[wordCounter-1] + 1;
               	                wordEnd[wordCounter] = wordStart[wordCounter] + wordLength;
                       	        for(int i = 0; i <= wordLength; i++)
                                {
       	                                words[wordStart[wordCounter]+i] = tempWord[i];
               	                }
                       	        wordLength = 0;
			}
			continue;
		}
		else if(ispunct(temp)) //used rather than making a series of comparisons
		{
			continue;
		}
		charCounter += 1;
		tempWord[wordLength] = tolower(temp);
		wordLength += 1;
	}

	heapsort();
	reverseArrays(wordCounter);

	for(int j = 0; j <= wordCounter; j++)
	{
	wordCount[j] += 1;
	for(int i = wordStart[j]; i < wordEnd[j]; i++)
	{
		cout << words[i];
	}
	cout << " counted " << wordCount[j] << endl;
	}
	//close file
	inputFile.close();

	return 0;
}

bool stringCompare(char str1[], char str2[], int length)
{
	for(int i = 0; i < length; i++)
	{
		if(str1[i] != str2[i]){return false;}
	}
	return true;
}

void siftdown(int i, int length)
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

	if(j >= length)
	{
		return;
	}
	if(wordCount[j] < wordCount[j+1])
	{
		j = j + 1;
	}
	if(wordCount[i] < wordCount[j])
	{
		int temp = wordCount[i];
		wordCount[i] = wordCount[j];
		wordCount[j] = temp;

		temp = wordStart[i];
		wordStart[i] = wordStart[j];
		wordStart[j] = temp;

		temp = wordEnd[i];
		wordEnd[i] = wordEnd[j];
		wordEnd[j] = temp;

		siftdown(j, length);
	}
}

void makeheap()
{
	for(int i = wordCounter/2; i >= 0; i--)
	{
		siftdown(i, wordCounter);
	}
}

void heapsort()
{
	makeheap();
	int temp;
	for(int i = wordCounter; i >= 0; i--)
	{
		temp = wordCount[i];
                wordCount[i] = wordCount[0];
                wordCount[0] = temp;

                temp = wordStart[i];
                wordStart[i] = wordStart[0];
                wordStart[0] = temp;

                temp = wordEnd[i];
                wordEnd[i] = wordEnd[0];
                wordEnd[0] = temp;

		siftdown(0, i);
	}
}

void reverseArrays(int length)
{
	int temp;
	int bottom{0};
	for(int bottom = 0; bottom < length; bottom++, length--)
	{
		temp = wordCount[bottom];
		wordCount[bottom] = wordCount[length];
		wordCount[length] = temp;

		temp = wordStart[bottom];
		wordStart[bottom] = wordStart[length];
		wordStart[length] = temp;

		temp = wordEnd[bottom];
		wordEnd[bottom] = wordEnd[length];
		wordEnd[length] = temp;
	}
}
