//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Week 10 Lab Exercise                     //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<algorithm>

using namespace std;

//structures
struct Card
{
	char rank;
	char suit;
};

//functions
int crazyUp();
bool cardCheck(Card* one, Card* two);

//global variables
Card deck[52];
int first_index;
int last_index;

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
	char card[2];
	int count{0};
	while(inputFile >> card)
	{
		Card temp;
		temp.rank = card[0];
		temp.suit = card[1];

		deck[count] = temp;

		count += 1;
	}

	//close file
	inputFile.close();

	cout << crazyUp() << endl;
	cout << "First card in sequence : " << deck[first_index].rank << deck[first_index].suit << endl;
	cout << "Last card in sequence : " << deck[last_index].rank << deck[last_index].suit << endl;

	return 0;
}

int crazyUp()
{
	int Length[52];
	Length[0] = 0;
	int maxLength{0};
	int len;
	for(int i = 1; i < 52; i++)
	{
		len = 1;
		for(int j = i-1; j >= 0; j--)
		{
			if(cardCheck(&deck[i], &deck[j]))
			{
				len = max(len, Length[j]+1);
			}

			if(len == maxLength)
			{
				first_index = j;
			}
		}
		Length[i] = len;

		if(maxLength != max(maxLength, len))
		{
			last_index = i;
		}
		maxLength = max(maxLength, len);
	}

	for(int i = 0; i < 52; i++)
        {
                cout << Length[i] << " ";
        }

	return maxLength;
};

bool cardCheck(Card* one, Card* two)
{
	if(one->rank == 8 || two->rank == 8) {return true;}

	else if(one->suit == two->suit) {return true;}

	else if(one->rank == two->rank) {return true;}

	else {return false;}
};
