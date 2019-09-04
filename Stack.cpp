/*Implementing a Stack */
#include <iostream>
#include <fstream>
#include <cstring>
#include <string.h>
using namespace std;

string wordstr[30];
int last = 0;
bool IsEmpty();
void top();
void pop();
void push(char name[20]);

int main() {
	char name[20];
	cout << "Please input the file name: ";
	cin >> name;
	
	ifstream in;
	in.open(name); //open the file name input by user
	if(!in.good())
	{
		cerr << "File not open.\n";
		exit(1);
	}

	char word[20];
	while (!in.eof())
	{
		in >> word;
		if(in.fail())
			break;
		push(word);
	}
	cout << endl;
	in.close(); 

	while(!IsEmpty())
	{
		top();
		pop();
	}	
	return 0;
}

bool IsEmpty()
{
	if (last == 0)
		return true;
	return false;	
		
}

void top()
{
	cout << wordstr[last-1] << " " ;
}

void pop()
{

	last--;
}

void push(char word[20])
{
	wordstr[last] = word; 
	cout << wordstr[last++] << " " ;
	last++;
}

