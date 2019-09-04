/*Implement a Hash table*/
#include <iostream>
#include <fstream>
using namespace std;
struct node 
{
	int key; //index value 
	int val;	
	node *same;
};
void merge(int start, int centre, int end);
void mergeSort(int start, int end);
void insert(int value);
void print();
void checkLength();

int maxLength = 1;//if nothing gets updated, it will be at least one 
int numEmpty = 100;//because at the beginning there will be 100 anyway 
node* hash[100];//mod f a 100 so it can only go from 0-99
int main()
{
	cout << "Please enter the filename: ";
	char name[10];
	cin >> name;
	ifstream in(name);
	if(!in.good())
	{
		cerr << "FILE NOT FOUND.\n";
		exit(-1);
	}
	int value;
	while(!in.eof())
	{
		in >> value; //read in number
		if(in.fail())
			break;
		insert(value);
	}
	cout << "1. The number of empty entries in the hash table: " << numEmpty << endl;
//	checkLength();//compare the length of each created node
	cout << "2. The length of the longest chain: " << maxLength << endl; 
}
void insert (int value)
{
	int thisChainLength = 0;
	int mod = value%100;  
	node *temp = new node; //create a dynamic that points to the node 
	temp->val = value;//store value at that position 
	temp->same = NULL; //assign the next of that to null 
	if (hash[mod] != NULL)//if theres already a value there
	{
		thisChainLength++;
		node *next = hash[mod];
		while(next->same != NULL)
		{
			next = next->same; //it it still has something, then has to go to next one
			thisChainLength++;
		} 
		next->same = temp; //then assign the newly created node to NEXT of that function 
		thisChainLength++; 
	}
	else //if theres nothing then
	{
		hash[mod] = temp; 
		numEmpty--; //each time a new node in bên Inserted in 
		thisChainLength++;
	}
	if(thisChainLength > maxLength)
		maxLength = thisChainLength;
}

void checkLength()
{
	int length;//if theres a length, it is 1 anyway  
	for(int i = 0; i < 100; i++)
	{
		length = 0;//each time it is reassigned to the length of each hash value
		if(hash[i] == NULL) //if theres nothing in here at all 
			continue;
		else
		{
			while(hash[i] != NULL)//if IM OKAY
			{
				length++; 
				hash[i] = hash[i]->same; //assign to next same
			}
		}
	if(length > maxLength) //if the length of chain at that node is greater than the length ==> UPDATE
		maxLength = length; 
	}
}
