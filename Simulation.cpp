/*Assignment 2: Simulation 
Name: Minh Ngoc Nguyen 
Student number: 5768354
Student account: mnn302
*/
#include <iostream>
#include <fstream>
using namespace std;

long double timenow = 0; 
int numidle1st = 0; //number of available primary server 
int numidle2nd = 0; //number of available secondary server 
int CustServed = 0; //total of customer served
long double timequeue1 = 0.0;
long double timequeue2 = 0.0;
struct Server{
	long double idleFor;
	long double busyFor;//the total time I have waited today
	bool IsBusy; //at the start they are not busy 
};

struct Cust{
	long double ccome; //this will point to heap value
	long double cgo1; //when they leave first server
	long double cgo2; //when they leave secondary server
	int serve1;//this customer has been served at this INDEX 
	int serve2;//this customer has been served here INDEX 
	long double waitFrom1;
	long double waitFrom2; 
};

struct Event{
	int type; //0==come, 1==go1, 2==go2
	long double time; //time it happens 
	long double ecome; //this will point to heap value
	long double ego1; //when they leave first server
	long double ego2; //when they leave secondary server
	int serverNo;
};
//---------------stats 
long double totalServiceTime = 0;
int maxLengthQueue1 = 0; 
int maxLengthQueue2 = 0; 
int total1st = 0; //total of servers
int total2nd = 0;//total of servers
Server server1arr[10];//server array for  servers where max size is 10
Server server2arr[10];//server array for 2nd servers where max size is 10

Event priorityHeap[21]; //heap/priority queue contains people who are should be in one of the servers
int heapIndex = 0; 

Cust queue1st[100]; //circular queue
Cust queue2nd[100];//circular queue
int noCust1 = 0;
int noCust2 = 0;
int front1 = -1; 
int front2 = -1;
int length1 = 100; //length of the queue 
int rear1 = -1;
int rear2 = -1; 
int length2 = 100; //length of the queue

//double *queue1st[MAX_LINE]; //I might need the first element of the array to store the first index of 
//double *queue2nd[MAX_LINE];
Cust createCust(long double a, long double b, long double c);
Event createEvent(long double time, long double a, long double b, long double c, int type, int serveNo);
void initialiseServers();
void swapEvent(int leaf, int parent); 
void serveFirst(long double arriveSF, long double firsttimeSF, long double secondtimeSF);
int servePrimary();
int serveSecondary();
void siftDown(int root); 
void siftUp(int leaf);
void makeHeap(int index);//this should be 21
void addToHeap(Event ev); 
Event topHeap(); //pop the value with highest priority
void enQueue(Cust queue[], int &front, int &rear, int &length, int &noCust, Cust c);
Cust deQueue(Cust queue[], int &front, int &rear, int &length, int &noCust);


void initialiseServers()
{
	int i;
	//a for loop to create primary servers 
	for(i = 0; i < total1st; i++)
	{
		//create a server struct with value P, i
		Server s1;
		s1.IsBusy = false; 
		s1.busyFor = 0.0;
		server1arr[i] = s1;
	}//last one end at num1st
	//a for loop to create secondary servers
	for(i = 0; i < total2nd; i++)
	{
		//create a server struct with value S, j
		Server s2;
		s2.busyFor = 0.0;
		s2.IsBusy = false;
		server2arr[i] = s2; 
	}
}


Cust createCust(long double a, long double b, long double c)//this act as a default constructor for CUST
{
	Cust customer;
	customer.ccome = a;
	customer.cgo1 = b;
	customer.cgo2 = c;
	customer.waitFrom1 = 0.0; 
	customer.waitFrom2 = 0.0;
	return customer; 
}

Event createEvent(long double time, long double a, long double b, long double c, int type, int serveNo)//this act as a default constructor for EVENT
{
	Event event; 
	event.type = type;
	event.time = time; 
	event.ecome = a; 
	event.ego1 = b; 
	event.ego2 = c;
	event.serverNo = serveNo;
	return event;
}

void serveFirst(long double arrive, long double firsttime, long double secondtime)//read in the first arrival and create 3 events subsequently
{
	Event v1;//create first event
	//add a 'customer arrival event' to the event priority queue
	v1.type = 0; 
	v1.time = arrive; 
	v1.ecome = arrive;
	v1.ego1 = firsttime;
	v1.ego2 = secondtime; 
	
	priorityHeap[heapIndex] = v1; 
	heapIndex++;//at this stage, I dont have to sift anything, it is earlier 
	CustServed++;//serve first customer
}

int servePrimary()
{
	int indexto = 0; 
	for(int i = 0; i < total1st; i++)//loop all in that 
	{
		if(server1arr[i].IsBusy != true)
		{
			indexto = i;
			break;
		}
		//now it will be false ==> MEAN IT IS FREE
		 //after having 1 value, I return straightaway
	}
	numidle1st--;//reduce number of server by 1 
	server1arr[indexto].IsBusy = true; //NOW I MAKE IT BUSY beause im going to use it 
	return indexto;//return the index to the server in the 
}

int serveSecondary()
{
	int indexto = 0; 
	for(int i = 0; i < total2nd; i++)//loop all in that 
	{
		if(server2arr[i].IsBusy != true)
		{
			indexto = i;//this return the index of that server in the array 
			break;
		}
	}
	numidle2nd--; //available is reduced by one
	server2arr[indexto].IsBusy = true; //NOW I MAKE IT BUSY beause im going to use it 
	return indexto;//return the index to the server in the 
}

void swapEvent(int leaf, int parent)
{
	Event temp = priorityHeap[leaf];
	priorityHeap[leaf] = priorityHeap[parent]; 
	priorityHeap[parent] = temp;
}

void siftDown(int root) //this is to give the max heap
{
	int child = 0;
	int leftchild = root * 2; //index of child in heap is 2i //doing this to find the child  
	int rightchild = leftchild+1; 
	if(root == 0)
	{ 
		leftchild = 2*root + 1; 
		rightchild = 2*root + 2;
	}
	if (rightchild <= heapIndex && priorityHeap[leftchild].time > priorityHeap[rightchild].time) //if the last number is smaller
		child = rightchild; //find which child is the smaller child to COMPARE 
	else 
		child = leftchild; 	 
	if (child > heapIndex || child == 0)
		return; //this is now time to stop
	if (priorityHeap[root].time > priorityHeap[child].time)
	{
		swapEvent(root, child);
		siftDown(child);//recursively do this process
	}
}

void siftUp(int leaf) //to have a min heap
{
	if (leaf == 0)
		return;
	int parent = leaf/2;//if leaf = 3, I wanna check value 2 first, but leaf/2 gives 1
	//compare the parent with the bigger child
	if (priorityHeap[parent].time <  priorityHeap[leaf].time) //compare with the parent
	{
		return; //if the new leaf is smaller than its parent
	}
	else
	{
		swapEvent(leaf, parent);
		siftUp(parent);
	}
}

void addToHeap(Event ev)
{ 
	priorityHeap[heapIndex] = ev;//insert that into the array
	siftUp(heapIndex);
	heapIndex++;
} 

Event topHeap()//get the first event in the HEAP
{
	Event temp; 
	temp = priorityHeap[0]; 
	priorityHeap[0] = priorityHeap[heapIndex-1];//change last to first then siftdown
	heapIndex--;
	siftDown(0);
	return temp;
}

void enQueue(Cust queue[], int &front, int &rear, int &length, int &noCust, Cust c)
{
	if((front == 0 && rear == length-1)||//array starts at 0   
		(rear == (front-1)%(length-1)))//array starts in the middle 
	{
		cout << "" << endl;
	}
	else if(front == -1)//if it starts from the last element of list
	{
		//cout << "FRONT " << front << " has changed here to be 9999999999999\n";
		front = rear = 0; 
		queue[rear] = c; //insert that customer in index 0
	}
	else if(rear == length-1 && front != 0)//if starts 
	{
		rear = 0; 
		queue[rear] = c; //insert that customer in 
	}
	else 
	{//NORMAL CASE
		rear++; 
		queue[rear] = c; 	
	}
	noCust++; 
}

Cust deQueue(Cust queue[], int &front, int &rear, int &length, int &noCust)
{
	Cust temp;
	if(front == -1)
	{
		return temp;//THIS NEVER MEANT TO HAPPEN
	}
	temp = queue[front]; //because we are taking out the first customer
	if(front == rear)//this means that the whole list is empty
	{
		front = -1; 
		rear = -1; 
	}
	else if(front == length-1)
	{
		front = 0; //this means that we reach end of list //then starts again
	}
	else 
	{
		front++; //increase the index of front to the next one 
	}
	noCust--;
	return temp;
}

int main(int argc, char** argv) {
	
	char name[20];
	//cin >> name; 
	ifstream in(name);
	if (!in.good())
	{
		cerr << "NO FILE FOUND.\n";
		exit(-1);
	}
	in >> total1st; //total number of 1st is the number of available at the beginning
	in >> total2nd; //total number of 2nd is the number of available at the beginning
	numidle1st = total1st; 
	numidle2nd = total2nd;
	initialiseServers();
	long double arrive = 0; 
	long double firsttime = 0; //service time of the 1st one
	long double secondtime = 0; //service time of the 2nd one
	in >> arrive >> firsttime >> secondtime; //read THE FIRST CUSTOMER
	serveFirst(arrive, firsttime, secondtime);
	int numberof = 0;
	while (heapIndex != 0)//there are events in the event priority queue  // main event loop
	{
		Event newevent = topHeap();//get next event from the event priority queue
		long double timebefore = timenow; //FIRST TIME IT WOULD BE 0 
		timenow = newevent.time;//set the time to this eventc
		if (newevent.type == 0)//event is a 'customer arrival event'
		{
			Cust temp;
			if (numidle1st != 0) //theres still one available primary server  
			{
				int serve0 = servePrimary();//get serverNo==>this customer is BEING SERVED//add customer to pri server array
				server1arr[serve0].busyFor += newevent.ego1;//it has spent this much time serving/BUSY
				Event newevent1 = createEvent(newevent.ecome+newevent.ego1, newevent.ecome, newevent.ego1, newevent.ego2, 1, serve0); //create 'customer finish being served in primary server event'
				addToHeap(newevent1);//add that EVENT to the event priority queue
			}
			else
			{
				temp = createCust(newevent.ecome, newevent.ego1, newevent.ego2);//create the customer 
				temp.waitFrom1 = newevent.ecome;//customer has to start waitng from here 
				enQueue(queue1st, front1, rear1, length1, noCust1, temp);;//add customer info to primary server FIFO queue
				if(noCust1 > maxLengthQueue1)
					maxLengthQueue1 = noCust1;
			}
			//since 1 arrival gone, next arrival can come
			if(!in.eof())//if theres still arrival 
			{
				in >> arrive >> firsttime >> secondtime;//read next customer arrival event from file and 
				if(arrive == 0 && firsttime == 0 && secondtime == 0)
					continue;
				Event newevent2 = createEvent(arrive, arrive, firsttime, secondtime, 0, -1) ;	//create a 'customer arival event'
				addToHeap(newevent2); //add to the event priority queue//this type has no server
				CustServed++;//trying to serve another customer 
			}
		}
		
		else if (newevent .type == 1)//this is the 'finished primary server event'
		 {
			Cust temp1; 
			server1arr[newevent.serverNo].IsBusy = false; //change the serve to free
			numidle1st++;
			//-----------------do service time stats

			if (numidle2nd != 0)//a secondary server is available
			{
				int serve = serveSecondary();//get serverNo==>this customer is BEING SERVED//add customer to pri server array
				server2arr[serve].busyFor += newevent.ego2;//it has spent this much time serving/BUSY 
				Event newevent3 = createEvent(newevent.ecome+newevent.ego1+newevent.ego2, newevent.ecome, newevent.ego1, newevent.ego2, 2, serve); //create 'customer finish being served in primary server event'
				addToHeap(newevent3);//add that EVENT to the event priority queue
			}
			else
			{
				temp1 = createCust(newevent.ecome, newevent.ego1, newevent.ego2);//insert that value into CUSTOMER in case they need to be queued
				temp1.waitFrom2 = newevent.time;//wait at the moment is been put into the queue
				enQueue(queue2nd, front2, rear2, length2, noCust2, temp1);//add customer info to secondary server FIFO queue
				if(noCust2 > maxLengthQueue2)
					maxLengthQueue2 = noCust2;		
			}
		}
		else //if event is a 'customer finish being served in secondary server event'
		{
			server2arr[newevent.serverNo].IsBusy = false;//set secondary server's idle flag to true for the server that the customer just left 			
			numidle2nd++;		
			///-------do service time stats
		}
		if (noCust2 != 0 && numidle2nd != 0)//secondary server's FIFO queue is not empty and a secondary server is available
		{
			Cust temp2 = deQueue(queue2nd, front2, rear2, length2, noCust2);//get next customer from secondary server FIFO queue
			timequeue2 += (newevent.time - temp2.waitFrom2);//the time that this has become a EVENT - when they WAITED FROM since enqueue
			temp2.serve2 = serveSecondary();//add customer to sec server array and get serverNo
			server2arr[temp2.serve2].busyFor += temp2.cgo2;//it has spent this much time serving/BUSY 
			Event newevent4 = createEvent(newevent.time+temp2.cgo2, temp2.ccome, temp2.cgo1, temp2.cgo2, 2, temp2.serve2);//add a 'customer finish being served in secondary server event' to the event priority queue
			addToHeap(newevent4);
		}
		if (noCust1 != 0 && numidle1st != 0)//primary server's FIFO queue is not empty and a primary  server is available
		{
			Cust temp3 = deQueue(queue1st, front1, rear1, length1, noCust1);//get next customer from primary server FIFO queue and 
			timequeue1 += (newevent.time - temp3.waitFrom1);
			temp3.serve1 = servePrimary();//add customer to pri server array and get serverNo
			server1arr[temp3.serve1].busyFor += temp3.cgo1;//it has spent this much time serving/BUSY 
			Event newevent5 = createEvent(newevent.time+temp3.cgo1, temp3.ccome, temp3.cgo1, temp3.cgo2, 1, temp3.serve1);//add a 'customer finish being served in primary server event' to the event priority queue
			addToHeap(newevent5);
 		}
	}
	in.close();
	long double testdata = 0;
	cout << "Number of people served: " << CustServed << endl;//because it increment after reading one
	cout << "Time last service request completed: " << timenow << endl; //because at this time the last "finishing time of a service was done"//xxx.xxx
	cout << endl; 
	cout << "Ave total time spent in queue1: " << timequeue1/CustServed << endl; //xx.xx
	cout << "Ave total time spent in queue2: " << timequeue2/CustServed << endl; //xx.xx
	cout << "Ave total time spent in queues: " << ((timequeue1/CustServed)+(timequeue2/CustServed)) << endl; //xx.xx
	cout << endl; 
	cout << "Ave length of the queue1: " << "--" << endl; //xx.xx
	cout << "Ave length of the queue2: " << " --"  << endl; //xx.xx
	cout << "Ave length of the queues: " << " " << endl; //xx.xx 
	cout << endl;
	cout << "Max length of queue1: " << maxLengthQueue1 << endl; //xx
	cout << "Max length of queue2: " << maxLengthQueue2 << endl; //xx
	cout << "Max length of queues: " << maxLengthQueue1+maxLengthQueue2 << endl;
	cout << endl; 
	cout << "Idle times of servers:\n\n"; 
	cout << "Total idle time for each server in primary servers\n"; 
	int i; //for looping
	long double busy1 = 0.0;
	long double busy2 = 0.0;
	for(i = 0; i < total1st; i++)
	{
		busy1 += server1arr[i].busyFor; 
		cout << i << ": " << timenow - server1arr[i].busyFor << "\t"; //xx.xx
	}
	cout << endl;
	cout << "Total idle time for each server in secondary servers\n";
    for(i = 0; i < total2nd; i++)
	{
		busy2 += server2arr[i].busyFor;
		cout << i << ": " << timenow - server2arr[i].busyFor << "\t"; //xx.xx	
	}  
	cout << endl;
	totalServiceTime = (timequeue1+busy1) + (timequeue2+busy2);
	cout << "Ave total service time: " << totalServiceTime/CustServed << endl ; //x.xxx
return 0;
}



