/*Breadth First Search === USING HASHTABLE/ADJACENT LIST*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <iomanip>
using namespace std; 
int vertices = 0;
int edges = 0;
struct Node{
	int vertex; 
	Node* next;
};

	int visitNodes[25];
	bool visitedNodes[25];
	Node *graph[25];//this graph contains a nodefirst however its linked to other node

int front = 0; 
int visited = 0; 
bool checkVisit(int val);
void addVertex(int v1, int v2);
void enQ(int value);
int deQ();
void printList();

void addVertex( int v1, int v2)
{
	//INSERT ONE WAY
	Node* nNode1 = new Node; 
	nNode1->vertex = v2;
	nNode1->next = NULL;//theres no value yet
	if(graph[v1] == NULL)
	{
		graph[v1] = nNode1;
	}
	else 
	{
		Node *temp1 = graph[v1]; //this is pointing to that 
		while(temp1->next != NULL)
		{
			temp1 = temp1->next; 
		}
		temp1->next = nNode1; //assign that one here 
		//after while loop, I am at last element 
	}
	
	//INSERT ANOTHER
	Node* nNode2 = new Node; 
	nNode2->vertex = v1;
	nNode2->next = NULL;//theres no value yet
	if(graph[v2] == NULL)
	{
		graph[v2] = nNode2;
	}
	else 
	{
		Node *temp2 = graph[v2]; //this is pointing to that 
		while(temp2->next != NULL)
		{
			temp2 = temp2->next; 
		}
		temp2->next = nNode2; //assign that one here 
		//after while loop, I am at last element 
	}
}

void enQ(int value)
{
	visitNodes[visited] = value; 
	visited++;//visited one more node
} 

int deQ()
{
	int pop = visitNodes[front]; 
	front++;
	return pop;
}

bool checkVisit(int val)
{
	for(int i = 0; i < visited; i++)
	{
		if(visitedNodes[i] == val)
			return true;
	}
	return false;
}

void printList()
{	
	enQ(0);//add this to the visited 
	while(visited != 25)//nothing in Q
	{
		int curr = deQ(); 
		//visitedNodes[curr] = true;//add that to be VISITED
		Node *temp = graph[curr];//this contains the first value 
		while(temp != NULL)
		{
			if(visitedNodes[temp->vertex] == false && checkVisit(temp->vertex) == false)
			{
				enQ(temp->vertex);
				visitedNodes[temp->vertex] = true;
				cout << setw(5) << curr << "\t" << temp->vertex  << setw(5)<< endl;
				//parent[curr] = temp->vertex;
			}
			temp = temp->next;
		}
	}
}

int main()
{
	char name[20];
	cout << "Please entre filename: ";
	//cin >> name;
	ifstream in(name);
	if(!in.good())
	{
		cerr << "File doesnt exist.\n";
		exit(-1);
	}
	int v1, v2; 
	in >> vertices; //get the number of vertices 
	//int parent[vertices];

	for(int i = 0; i < vertices; i++)
		visitedNodes[i] = false; //initialise it to false at first 
	//graph = new Node[vertices];
	while(!in.eof())
	{
		in >> v1 >> v2; //read 2 vertices of this edge 
		if(in.fail())
			break;
		addVertex(v1, v2);
	}
	printList();
	return 0;
} 
