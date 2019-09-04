
/*Implementing a Heap*/

#include <iostream>
#include <fstream>
using namespace std;
void makeHeap(int i);
void swap(int c1, int c2);
void siftDown(int root);
void siftUp(int leaf);

int heap[500]; //create an array to hold 100 int
int counter = 1;
int main()
{
	cout << "Please enter the file name: ";
	char fname[20];
	cin >> fname; //read in fname
	fstream in;
	in.open(fname); // try to open
	if (!in.good())
	{
		cerr << "Can not open file.\n";
		exit(1);
	}

	int num; //the variable to store 
	while (!in.eof())
	{
		in >> num; //try to read an int 
		if (in.fail())
			break; //if it is not an int, break the loop
		heap[counter] = num;//insert that into the array
		 //after inserting, increment;	
		siftUp(counter);
		counter++;
		//
	}
	in.close();
	cout << "Make MIN heap:\n";
	for (int i = 1; i <= 5; i++)
	{
		cout << heap[i] << " "; //print the ith element of the heap
	}
	makeHeap(counter);
	cout << "\nMake MAX heap:\n";
	for (int i = 1; i <= 5; i++)
	{
	cout << heap[i] << " "; //print the ith element of the heap
	}
	
	char c;
	cin >> c;
	return 0;
}

void makeHeap(int index)//this int gets added to the end of the array 
{
	int mid = counter/2;//integer division
	for (int i = mid; i > 0; i--)
	{
		//this int index is passed into the correct position in the array
		siftDown(i); //FROM SMALL TO MID 
	}
}

void siftDown(int root) //this is to give the max heap
{
	int child = root * 2; //index of child in heap is 2i //doing this to find the child  
	if (heap[child] < heap[child + 1]) //if the last number is smaller
		child = child + 1; //find which child is the bigger child to COMPARE 
	if (heap[root] < heap[child])
	{
		swap(root, child); //swap those the size and mid of size 
		siftDown(child);//recursively do this process
	}
}

void siftUp(int leaf) //to have a min heap
{
	if (leaf == 1)//if we at first position then correct
		return;
	int parent = (leaf / 2); //integer division 
	//compare the parent with the bigger child
	if (heap[parent] <  heap[leaf]) //compare with the parent
		return; //if the new leaf is smaller than its parent
	else
	{
		swap(leaf, parent);
		siftUp(parent);
	}
}

void swap(int c1, int c2) //swap value of 2 element by reference
 {
	int tem = heap[c1];
	heap[c1] = heap[c2];
	heap[c2] = tem;
}


