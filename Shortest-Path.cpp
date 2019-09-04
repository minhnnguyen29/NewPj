 /*Find Shortest Path
*/
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <float.h>
#include <math.h>//use this for compiler 
using namespace std;

int noNodes = 0;
int noEdges = 0;
double coordX[100]; 
double coordY[100];
int minParent[100];
int minParent_inverse[100];
double edges[100][100]; //matrix
bool inHeap1[100];
bool inHeap2[100];

int sIndex = 0; 
int p1Index = 0;
bool Candidate[100];//index node-2
bool Candidate_inverse[100];
int goalStart = 0; 
int goalEnd = 0;
int heapIndex = 0;
int inverseIndex = 0;
//int parent[100]; //store nodes and its parents 
double nodedistance[100];
double nodeheuristic[100];//index node-1
double nodecost[100];
double nodecostinverse[100];
int heap[100];//this heap contains the index of node in order of a heap 
int heapinverse[100];

void swapNodeIndex(int leaf, int parent, int isHeap[100]);  
void siftDown(int root, double nodelist[100], int &hIndex, int isHeap[100]); 
void siftUp(int leaf, double nodelist[100]);
void makeHeap(int index, double nodelist[100]);//this should be 21
int topHeap(int &hIndex, int isHeap[100], double nodelist[100]); //pop the value with highest priority
void addVertex(int v1, int v2, double price);
double getShorter(double v1, double v2);//pass these 2 nodes in because they have the same COST
int getMinIndex();
void getPath(int end, int parent[100]);
void getPath_Reverse(int end, int parent[100]);
void reset();
int getPositionInHeap(int pos, int isHeap[100]);
double getDistance(int node1, int node2);
void moveForward(int &hIndex, int start, double nodelist[100], int count, int isHeap[100], int parent[100], bool inHeap[100]);	
int Dijkstra();
int AStar();
int Double_Dijkstra();
/* 

Djikstra is a heuristic to find a minimal spanning tree by starting from one node 
to a specific end node. 

A* has a additional heuristic that outstands Djikstra in ways that it looks
at the direction it is actually going to and consider if that helps it reach the direction 
that heads towards the end node.  

The priority queue will not only prioritise the nodes using the smallest cost to get to it 
but also that 
*/
//----------------------MUTUAL Methods--------------------------
void reset()
{
	heapIndex = 0;
	sIndex = 0;
	p1Index = 0;
	for(int i = 0; i < noNodes; i++)//reset all the data to default mode so it erased all the previous work 
	{ 
		nodedistance[i] = DBL_MAX;
		nodecost[i] = DBL_MAX;
		nodecostinverse[i] = DBL_MAX; 
		minParent[i] = i;
		Candidate[i] = true;
		Candidate_inverse[i] = true;
		heap[i] = 0;
		inHeap1[i] = false;//if the element is included in the heap
		inHeap2[i] = false;
	}
} 

double getShorter(double v1, double v2)
{
	if(v1 < v2)//if the v1 points to smaller cost than v2
		return v1;
	else 
		return v2;
}

void addVertex( int v1, int v2, double price)//this.index is the index of the EDGE 
{
	edges[v1-1][v2-1] = price;//add the other direction 
	edges[v2-1][v1-1] = price;//add 1 direction
}

void swapNodeIndex(int leaf, int parent, int isHeap[100])//
{
	int temp = isHeap[leaf];
	isHeap[leaf] = isHeap[parent];
	isHeap[parent] = temp;  
}

void getPath(int end, int parent[100])
{
	if(end == 0)
		return;
	p1Index++;
	getPath(parent[end], parent);
	cout << end+1 << " ";
}

void getPath_Reverse(int end, int parent[100])
{
	if(end == 0)
		return;
	p1Index++;
	cout << end+1 << " ";
	getPath_Reverse(parent[end], parent);
}

void siftDown(int root, double nodelist[100], int &hIndex, int isHeap[100]) //this is to give the max heap
{
	int child = 0;
	int leftchild = root * 2; //index of child in heap is 2i //doing this to find the child  
	int rightchild = leftchild+1; 
	if(root == 0)
	{ 
		leftchild = 2*root + 1; 
		rightchild = 2*root + 2;
	}
	if (rightchild <= hIndex && nodelist[isHeap[leftchild]] > nodelist[isHeap[rightchild]]) //if the last number is smaller
		child = rightchild; //find which child is the smaller child to COMPARE 
	else 
		child = leftchild; 	 
	if (child >= hIndex || child == 0)
		return; //this is now time to stop
	if (nodelist[isHeap[root]] > nodelist[isHeap[child]])
	{
		swapNodeIndex(root, child, isHeap);
		siftDown(child, nodelist, hIndex, isHeap);//recursively do this process
	}
}

void siftUp(int leaf, double nodelist[100], int isHeap[100]) //to have a min heap
{
	if (leaf == 0)
		return;
	int parent = leaf/2;//if leaf = 3, I wanna check value 2 first, but leaf/2 gives 1
	//compare the parent with the bigger child
	if (nodelist[isHeap[parent]] <  nodelist[isHeap[leaf]]) //compare with the parent
	{
		return; //if the new leaf is smaller than its parent
	}
	else
	{
		swapNodeIndex(leaf, parent, isHeap);
		siftUp(parent, nodelist, isHeap);
	}
}

int topHeap(int &hIndex, int isHeap[100], double nodelist[100])//get the first event in the HEAP
{
	int temp = isHeap[0]; 
	isHeap[0] = isHeap[hIndex-1];//change last to first then siftdown
	hIndex--;
	siftDown(0, nodelist, hIndex, isHeap);
	return temp;//this return the index to the smallest weight
}

int findPositionInHeap(int index, int isHeap[100])
{
	for(int i = 0; i < heapIndex; i++)
	{
		if (isHeap[i] == index)
			return i; //return where that index is located so then I could switch to first 
	}
	return -1; //if can find 
}
//-------------------------Dijsktra Method----------------------------
int Dijkstra()
{
	//the distance has been stored 
	nodecost[goalStart-1] = 0; //BECAUSE I wanna start from this node
	heap[0] = goalStart-1; 
	heapIndex++;
	//cout << "This " << goalStart << " is inserted at node " << heapIndex-1 << endl;
	minParent[goalStart-1] = 0;
	bool reachedEnd = false;
	while(!reachedEnd)
	{
		int min = topHeap(heapIndex, heap, nodecost); 	//return the index of the smallest node 
		if (nodecost[min] == DBL_MAX)
		Candidate[min] = false; //remove from Candidate
		//if min is popped then it is not in the list 
		if(min == goalEnd-1)
			reachedEnd = true;
		for(int count = 0; count < noNodes; count++)//search in in Candidate list
		{
			if(Candidate[count] && edges[min][count] != DBL_MAX)//&& nodedistance[count] > nodedistance[min] + edge[min][count])
			{
				moveForward(heapIndex, min, nodecost, count, heap, minParent, inHeap1);
			}		
		}
		sIndex++;
		if(sIndex == noNodes)
		{
			cout << "There's no path between " << goalStart << " and " << goalEnd << endl;
			return -1; 
		}		
	} 
	return 0;
}

void moveForward(int &hIndex, int start, double nodelist[100], int count, int isHeap[100], int parent[100], bool inHeap[100])
{
	double temp = nodelist[count]; 
	nodelist[count] = getShorter(nodelist[count], nodelist[start] + edges[start][count]);
	int pos = findPositionInHeap(count, isHeap);
		
	if(inHeap[count])//if this count is in the heap
	{
		int pos = findPositionInHeap(count, heap);
		if(pos != -1)
		siftUp(pos, nodelist, isHeap);
	}
	else //if it is not in the heap, then you INSERT for the first time
	{
		isHeap[hIndex] = count;//insert that into the array
		siftUp(hIndex, nodelist, isHeap);//start from the root 
		hIndex++;
		inHeap[count] = true;
	}	
		
	if(nodelist[count] != temp)
		parent[count] = start;
}

//-------------------------A* Method----------------------------
double getDistance(int node1, int node2)
{
	double distance = 0;
	distance = sqrt(pow(coordX[node1] - coordX[node2], 2) + (pow(coordY[node1] - coordY[node2], 2)));
	if (distance < 0 )	
		return (-distance);
	else 
		return distance;
}

int AStar()
{
	//the distance has been stored 
	nodecost[goalStart-1] = 0; //BECAUSE I wanna start from this node
	heap[0] = goalStart-1;
	heapIndex++;
	minParent[goalStart-1] = 0;
	bool reachedEnd = false;
	while(!reachedEnd)
	{
		int min = topHeap(heapIndex, heap, nodedistance); 	//return the index of the smallest node 
		if (nodecost[min] == DBL_MAX)
		{
			cout << "There's no path between " << goalStart << " and " << goalEnd << endl;
			return -1;
		}
		Candidate[min] = false; //remove from Candidate
		//if min is popped then it is not in the list 
		if(min == goalEnd-1)
			reachedEnd = true;
		for(int count = 0; count < noNodes; count++)//search in in Candidate list
		{
			if(Candidate[count] && edges[min][count] != DBL_MAX)//&& nodedistance[count] > nodedistance[min] + edge[min][count])
			{
				double temp = nodecost[count]; 
				nodecost[count] = getShorter(nodecost[count], nodecost[min] + edges[min][count]);
				if(nodecost[count] != temp)
					minParent[count] = min;
				nodedistance[count] = nodecost[count] + getDistance(goalEnd-1, count);
				if(inHeap1[count])//if this count is in the heap
				{
					int pos = findPositionInHeap(count, heap);
					if(pos != -1)
						siftUp(pos, nodedistance, heap);
				}
				else //if it is not in the heap, then you INSERT for the first time
				{
					heap[heapIndex] = count;//insert that into the array
					siftUp(heapIndex, nodedistance, heap);//start from the root 
					heapIndex++;
					inHeap1[count] = true;
				}
			}		
		}
		sIndex++;
		if(sIndex == noNodes)
		{
			cout << "There's no path between " << goalStart << " and " << goalEnd << endl; 
			return -1;
		}
	} 	
	return 0;
}

int Double_Dijkstra()
{
	nodecost[goalStart-1] = 0; //BECAUSE I wanna start from this node
	nodecostinverse[goalEnd-1] = 0;//also starts from 
	heap[0] = goalStart-1; 
	heapinverse[0] = goalEnd-1;
	heapIndex++;
	inverseIndex++;
	//cout << "This " << goalStart << " is inserted at node " << heapIndex-1 << endl;
	minParent[goalStart-1] = 0;
	minParent_inverse[goalEnd-1] = 0;
	bool metHer = false;
	while(!metHer)
	{
		int min1 = topHeap(heapIndex, heap, nodecost);//return the index of the smallest node 
		int min2 = topHeap(inverseIndex, heapinverse, nodecostinverse);
		Candidate[min1] = false; //remove from Candidate
		Candidate_inverse[min2] = false; 
		//if min is popped then it is not in the list 
		sIndex++; 
		if(!Candidate_inverse[min1])
		{
			metHer = true;
			return min1;
		}
		sIndex++;
		if(!Candidate[min2])
		{
			metHer = true; 
			return min2;
		}
		for(int count = 0; count < noNodes; count++)//search in in Candidate list
		{//if this node hasnt been visited
			if(Candidate[count] && edges[min1][count] != DBL_MAX)//&& nodedistance[count] > nodedistance[min] + edge[min][count])
			{
				moveForward(heapIndex, min1, nodecost, count, heap, minParent, inHeap1);
			}
			if(Candidate_inverse[count] && edges[min2][count] != DBL_MAX)
			{
				moveForward(inverseIndex, min2, nodecostinverse, count, heapinverse, minParent_inverse, inHeap2);
			}
		}
		if(sIndex == noNodes*2)
		{
			cout << "There's no path between " << goalStart << " and " << goalEnd;
			return -1;
		}
	}
}

int main(int argc, char** argv) {
	
	char name[20];
	cout << "Please enter the file name: ";
	cin >> name; 
	ifstream in(name);
	if (!in.good())
	{
		cerr << "NO FILE FOUND.\n";
		exit(-1);
	}
	int tempdata1 = 0;
	int i = 0; //for loop iteration 
	in >> noNodes;//rea in number of nodes in the graph 
	for(i = 0; i < noNodes; i++)//each triplet for each edges //the node number will be index+1
	{
		in >> tempdata1 >> coordX[i] >> coordY[i];
		nodecost[i] = DBL_MAX;
	}
	in >> noEdges;
	int start, end;
	double cost;
	in >> start >> end >> cost;//insert the first pair index 0
	addVertex(start, end, cost);
	Candidate[start-1] = true;
	for(int n1 = 0; n1 < noNodes; n1++)
	{
		inHeap1[n1] = false;//none of them is in the heap
		for(int n2 = 0; n2 < noNodes; n2++)
			edges[n1][n2] = DBL_MAX;
	}
	for(i = 1; i < noEdges; i++)//insert from index 1
	{
		in >> start >> end >> cost; //read in start + end + cost of edge
		addVertex(start, end, cost);
		Candidate[start-1] = true;
		Candidate[end-1] = true;
	}
	in >> goalStart >> goalEnd;
	goalEnd = 21;
	in.close();
	
	cout << endl;
	cout << "------------Dijkstra Method----------------" << endl;
	int response = Dijkstra();
	if(response == 0)
	{
		cout << "The length of the shortest path from solution 1: " << nodecost[goalEnd-1] << endl; 
	cout << "The path from solution 1: ";
	getPath(goalEnd-1, minParent);
	cout << endl;
	cout <<  "The number of additional nodes in the solution 1: " << sIndex-p1Index<< endl;
	}
	cout << "\n-----------------A* Method-----------------" << endl;
	reset();//call reset function to make it back
	response = AStar();
	if(response == 0)
	{
		cout << "The length of the shortest path from solution 2: " << nodecost[goalEnd-1]<< endl;
		cout << "The path from solution 2: ";
		getPath(goalEnd-1, minParent);
		cout << endl;
		cout << "The number of additional nodes in the solution2: " << sIndex- p1Index << endl;
	}
	cout << "\n-----------Double Dijkstra Method------------" << endl;
	reset();//call reset function to make it back
	int nodemet = Double_Dijkstra();//this is the node where they met 
	if(nodemet != -1)
	{
		cout << "The length of the shortest path from solution 3: " << nodecost[nodemet] + nodecostinverse[nodemet]<< endl;
		cout << "The path from solution 3: " << endl;
		getPath(nodemet, minParent);
		getPath_Reverse(minParent_inverse[nodemet], minParent_inverse);
		cout << endl;
		cout << "The number of additional nodes in the solution 3: " << sIndex - p1Index << endl;
	}
return 0;
}



