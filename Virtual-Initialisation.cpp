/*Virtual Initialization*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
void checkProbe(int pos);

int index = 0;
int info[100]; //the array contains 100 elements 
int forwards[100]; //array of valid position
int backwards[100];//array of count //the inverse value with FORWARD
//the order it being putin 


int main()
{
	//int data[100]; //the array contains 100 elements 

	cout << "Please enter filename: ";
	char name[20];
	cin >> name;
	ifstream in(name);
	if (!in.good())
	{
		cerr << "File doesnt exists.";
		exit(-1);
	}	
	int what, where;

	string data2;
	while (!in.eof())
	{
		in >> what >> where; 
		if (what == -1 || where == -1)
		{
			break; 
		}
		else
		{
			index++; 
			info[where] = what; 
			forwards[index] = where; 
			backwards[where] = index; 
		}
	}
	int probe;
	while (!in.eof())
	{
		in >> probe; 
		if (probe == -1)
			break;
		checkProbe(probe);
	}
    return 0;
}

void checkProbe(int pos)
{
	if (backwards[pos] <= index && backwards[pos] > 0)
	{
		if (forwards[backwards[pos]] == pos)
			cout << "Position " << pos << " has been initialized to value " << info[pos] << ".\n";
		else
			return;
	}
	else
		cout << "Position " << pos << " has not been initialized." << endl;
}

