/*Dynamic Programming*/
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <float.h>
using namespace std;

string Rank[52];//for the 52cards
char Suit[52];//for the 52cards
int root[52];//the parent of each card 
int lastIndex = 0;
int cards = 0;
int Length[52];
void crazyUp();
int maxBetween(int length1, int length2);

int maxLength = 0;
string firstCard = "";
string lastCard = "";

int maxBetween(int length1, int length2)
{
	if (length1 >= length2)
		return length1; 
	else 
		return length2;
}

void crazyUp()
{
	Length[0] = 0;
	for(int i = 0; i < 52; i++)
	{
		Length[i] = 1;//default is 1 if it starts from DUMMY 
		root[i] = i;//one becomes root of itself before being changed
		
		for(int j = 0; j < i; j++)//goes to all the nodes before i 
		{
			if (Rank[i] == Rank[j] || Suit[i] == Suit[j] || Rank[i] == "8" || Rank[j] == "8")
			{
				if(Length[j]+1 > Length[i])
					Length[i] = Length[j]+1;
				root[i] = root[j];//index of that card
			}
		}
		if (maxLength < Length[i])
		{
			maxLength = Length[i]; 
			lastCard = Rank[i] + Suit[i];
			lastIndex = i;
		}
	}
}

int main(int argc, char** argv)
{
	char name[20];
	cout << "Please enter the file name: ";
	cin >> name; 
	ifstream in(name);
	if (!in.good())
	{
		cerr << "NO FILE FOUND.\n";
		exit(-1);
	}
	string details; 
	while(!in.eof())
	{
		in >>  details; 
		if(in.fail())
			break; 
		if(details.size() == 2)
		{
			Rank[cards] = details[0];
			Suit[cards] = details[1];
		}
		else //details.size = 3
		{
			Rank[cards] = details[0]; 
			Rank[cards] += details[1]; 
			Suit[cards] = details[2];
		}
		cards++;
	}
	crazyUp();
	in.close();
	cout << endl;
	cout << "The length of the longest sequence: " << maxLength << endl;
	cout << "The first card in the sequence: " << Rank[root[lastIndex]] << Suit[root[lastIndex]]<< endl;
	cout << "The last card in the sequence: " << lastCard << endl;
	 
}
