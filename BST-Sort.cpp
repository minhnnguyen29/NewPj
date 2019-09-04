/*BST Sort*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

struct leaf{
	int val; 
	leaf* left; //pointer to its left
	leaf* right; //pointer to its right
};

leaf* insert_first(int value); 
void visitOrder(leaf* newLeaf); //visit and print them in order
void insert(int value, leaf* newLeaf); //returning the pointer 

int main()
{
	cout << "Please input the file name: ";
	char name[20]; 
	cin >> name; 
	ifstream in(name); 
	if (!in.good())
	{
		cerr << "File not found";
		exit(-1);
	}
	int num = 0;
	in >> num;
	leaf *root = NULL;
	root = insert_first(num); //create the BST 
	while (in >> num)
	{
		insert(num, root);
	}
	visitOrder(root);
	cout << endl;
    return 0;
}
 

leaf* insert_first(int value)
{
	leaf *root = new leaf;
	root->val = value;
	root->right = NULL; 
	root->left = NULL;
	return root;
}

void insert(int val, leaf *node)
{
	leaf * next = NULL; //create pointer 
	bool leftie; //boolean to check
	if (val == node->val)
		return; //equals, no need insert 
	//otherwise godown the tree
	else if (val < node->val)
	{
		cout << val <<" go left\n";
		next = node->left; //wanna go to the left of that node to keep checking 
		leftie = true; //newLeaf becomes the left 

	}
	else
	{
		cout << val << " go right\n";
		next = node->right; 
		leftie = false; 
	}
	if (next != NULL)
	{
		insert(val, next); //insert down again
	}
	else
	{
		next = new leaf; 
		next->val = val; 
		next->left = NULL; //have to let tho to NULL otherwise they will have a dangling point 
		next->right = NULL;
		if (leftie) //update the parent HERE 
		{
			cout << val << " stays left\n";
			node->left = next;
		}
		else	
		{
			cout << val << " stays right\n";
			node->right = next;
		}
	}
}
int i = 1;
void visitOrder(leaf* newLeaf)
{
	if (newLeaf->left != NULL)
		visitOrder(newLeaf->left); 
		cout << newLeaf->val << " ";
	if(i%10 == 0)
	{
		cout << endl;
	}
	i++; 
	
	if (newLeaf->right != NULL)
		visitOrder(newLeaf->right);
		
}

/*
		int strarrCom(int pos, char aword[10])
		{
		//(length[pos1] != length[pos2])       
			for(int i = 0; i < strlen(aword); i++)//compare with the first word
			{
				if(strpool[pos] < aword[i])
					return 1;
				else if(strpool[pos] > aword[i])
					return -1; 	
			} 
			return 0; //this will return the equals word 
		}
		*/

