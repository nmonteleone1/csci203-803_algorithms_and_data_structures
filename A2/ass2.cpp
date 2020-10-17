//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Assignment 2                             //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<iomanip>

using namespace std;

#define PARENT(i) ((i-2) / 2)
#define LEFT(i) ((i*2) + 1)
#define RIGHT(i) ((i*2) + 2)

//structures
struct Customer
{
	float arrival_time;
	float service_duration;
	int customer_type;
	int server_id;
};

struct Server
{
	bool busy;
	int server_id;
	float total_service_time;
};

struct Event
{
	int event_type;
	float event_time;
	float service_duration;
	Customer cust;
};

//functions
void swap_event(Event *A, Event *B);
void dequeue(int* head, int* count);
void enqueue(Customer arr[], int* tail, Customer cust, int* count);
void min_heapify(Event arr[], int i);
void print_stats(int i, Server server0_list[], Server server1_list[]);

//global variables
int server0_max, server1_max;
float current_time{0};
int event_count{0};
Customer queue0[500];
Customer queue1[500];
int queue0_head{0}, queue1_head{0};
int queue0_tail{-1}, queue1_tail{-1};
int queue0_count{0}, queue1_count{0};
bool dummy_flag = false;

int customer0_total{0};
int customer1_total{0};
float last_service_time;
float total_service_time{0};
float customer0_service_time{0};
float customer1_service_time{0};
float customer0_queue_time{0};
float customer1_queue_time{0};
float queue0_length_time{0};
float queue1_length_time{0};
float queue0_prev_time{0};
float queue1_prev_time{0};
int queue0_max{0};
int queue1_max{0};
int max_queue_length{0};
int customer0_queue_total{0};
int customer1_queue_total{0};

//MAIN FUNCTION
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

///////////////////////
//     RUN ONE       //
///////////////////////

	//read servers
        inputFile >> server1_max >> server0_max;

	//initialise server stacks
	Server server0_list[server0_max];
	int server0_idle_stack[server0_max];
	int server0_idle_count = server0_max;
	for(int i = 0; i < server0_max; i++)
	{
		server0_list[i].busy = false;
		server0_list[i].server_id = i;
		server0_list[i].total_service_time = 0;
		server0_idle_stack[i] = i;
	}

	Server server1_list[server1_max];
	int server1_idle_stack[server1_max];
	int server1_idle_count = server1_max;
        for(int i = 0; i < server1_max; i++)
        {
                server1_list[i].busy = false;
                server1_list[i].server_id = i;
                server1_list[i].total_service_time = 0;
		server1_idle_stack[i] = i;
        }

	//initialise event queue
	Event event_heap[1 + server0_max + server1_max];

	//read in first customer
	Customer first_c;
        inputFile >> first_c.arrival_time >> first_c.customer_type >> first_c.service_duration;
	//customer counter
        if(first_c.customer_type == 0) {customer0_queue_total += 1;}
        else {customer1_queue_total += 1;}

	//add to event queue
	Event first_e;
	event_heap[0].event_type = 0;
        event_heap[0].event_time = first_c.arrival_time;
        event_heap[0].cust = first_c;

	//main loop
	while(event_count >= 0)
	{
		//get next event
		Event current;
		current.event_type = event_heap[0].event_type;
		current.event_time = event_heap[0].event_time;
		current.cust = event_heap[0].cust;

		//set time
		current_time = current.event_time;

		//if arrival event
		if(current.event_type == 0)
		{
			//deal with arrival event
			//if server available
			if(current.cust.customer_type == 0)
			{
				if(server0_idle_count > 0)
				{
					//change arrival event into service completion event
                                        event_heap[0].event_type = 1;
                                        event_heap[0].event_time = current_time + current.cust.service_duration;
					event_heap[0].cust.server_id = server0_idle_stack[server0_idle_count-1];
					//remove server from idle list
					server0_list[server0_idle_stack[server0_idle_count-1]].busy = true;
					server0_idle_count -= 1;
				}
				else
				{
					//queue stat
					queue0_length_time += (queue0_count * (current_time - queue0_prev_time));
					queue0_prev_time = current_time;

					//put customer in queue
					enqueue(queue0, &queue0_tail, current.cust, &queue0_count);

					//more queue stat
					if(queue0_count > queue0_max) {queue0_max = queue0_count;}

					//remove completed arrival event
	                                swap_event(&event_heap[0], &event_heap[event_count]);
        	                        event_count -= 1;
				}
			}
			else
			{
				if(server1_idle_count > 0)
                                {
					//change arrival event into service completion event
                                        event_heap[0].event_type = 1;
                                        event_heap[0].event_time = current_time + current.cust.service_duration;
                                        event_heap[0].cust.server_id = server1_idle_stack[server1_idle_count-1];
                                        //remove server from idle list
                                        server1_list[server1_idle_stack[server1_idle_count-1]].busy = true;
                                        server1_idle_count -= 1;
                                }
                                else
                                {
					//queue stat
                                        queue1_length_time += (queue1_count * (current_time - queue1_prev_time));
					queue1_prev_time = current_time;

                                        //put customer in queue
					enqueue(queue1, &queue1_tail, current.cust, &queue1_count);

					//more queue stat
					if(queue1_count > queue1_max) {queue1_max = queue1_count;}

					//remove completed arrival event
                                        swap_event(&event_heap[0], &event_heap[event_count]);
                                        event_count -= 1;
                                }
			}
			if(!dummy_flag)
			{
				//read next customer
				Customer next_c;
	        		inputFile >> next_c.arrival_time >> next_c.customer_type >> next_c.service_duration;
				if(next_c.arrival_time == 0 && next_c.customer_type == 0 && next_c.service_duration == 0)
				{
					dummy_flag = true;
				}

				//add next arrival event
				else
				{
					//customer counter
	                                if(next_c.customer_type == 0) {customer0_queue_total += 1;}
        	                        else {customer1_queue_total += 1;}

					event_count += 1;
				        event_heap[event_count].event_type = 0;
				        event_heap[event_count].event_time = next_c.arrival_time;
			        	event_heap[event_count].cust = next_c;
				}
			}
		}
		//else service completion event
		else
		{
			//deal with service completion event
			if(current.cust.customer_type == 0)
			{
				server0_list[current.cust.server_id].total_service_time += current.cust.service_duration;

				//business queue time
				customer0_queue_time += (current_time - current.cust.arrival_time) + current.cust.service_duration;
				customer0_service_time += current.cust.service_duration;
				customer0_total += 1;
			}
			else
			{
				server1_list[current.cust.server_id].total_service_time += current.cust.service_duration;

				//tourist queue time
                                customer1_queue_time += (current_time - current.cust.arrival_time) + current.cust.service_duration;
				customer1_service_time += current.cust.service_duration;
				customer1_total += 1;
			}

			//service completion stat
			last_service_time = current_time + current.cust.service_duration;
			total_service_time += (current_time - current.cust.arrival_time) + current.cust.service_duration;

			//if there is a queue for the opened server type
			if(current.cust.customer_type == 0 && queue0_count > 0)
			{
				//add service completion event from front of customer queue
				Customer next_c;
				next_c = queue0[queue0_head];
				event_heap[0].event_type = 1;
                                event_heap[0].event_time = current_time + next_c.service_duration;
                                event_heap[0].cust = next_c;
				event_heap[0].cust.server_id = current.cust.server_id;

				//queue stat
                                queue0_length_time += (queue0_count * (current_time - queue0_prev_time));
                                queue0_prev_time = current_time;

				dequeue(&queue0_head, &queue0_count);

			}
			else if(current.cust.customer_type == 1 && queue1_count > 0)
			{
				Customer next_c;
				next_c = queue1[queue1_head];
                                event_heap[0].event_type = 1;
                                event_heap[0].event_time = current_time + next_c.service_duration;
                                event_heap[0].cust = next_c;
                                event_heap[0].cust.server_id = current.cust.server_id;

				//queue stat
                                queue1_length_time += (queue1_count * (current_time - queue1_prev_time));
                                queue1_prev_time = current_time;

				dequeue(&queue1_head, &queue1_count);
			}

			//else make server idle
			else
			{
				//make server idle
				int tempID = current.cust.server_id;
				if(current.cust.customer_type == 0)
				{
					server0_list[tempID].busy = false;
					server0_idle_stack[server0_idle_count] = tempID;
					server0_idle_count += 1;
				}
				else
				{
                                        server1_list[tempID].busy = false;
                                        server1_idle_stack[server1_idle_count] = tempID;
                                        server1_idle_count += 1;
				}

				//remove completed service event
				swap_event(&event_heap[0], &event_heap[event_count]);
				event_count -= 1;
			}
		}
		min_heapify(event_heap, 0);
	}

	//calculate and printout statistics
	print_stats(1, server0_list, server1_list);


	//close file
	inputFile.close();

///////////////////////
//     RUN TWO       //
///////////////////////

        //initialisations
        current_time = 0;
        event_count = 0;
        queue0_head = 0;
        queue1_head = 0;
        queue0_tail = -1;
        queue1_tail = -1;
        queue0_count = 0;
        queue1_count = 0;
        dummy_flag = false;

        customer0_total = 0;
        customer1_total = 0;
        last_service_time = 0;
        total_service_time = 0;
        customer0_service_time = 0;
        customer1_service_time = 0;
        customer0_queue_time = 0;
        customer1_queue_time = 0;
        queue0_length_time = 0;
        queue1_length_time = 0;
        queue0_prev_time = 0;
        queue1_prev_time = 0;
        queue0_max = 0;
        queue1_max = 0;
        max_queue_length = 0;

        //open file, and check status
        inputFile.open(fileName);
        if(!inputFile)
        {
                cerr << "Could not open " << fileName << endl;
                return -1;
        }

        //read servers
        inputFile >> server1_max >> server0_max;

        //initialise server stacks
        for(int i = 0; i < server0_max; i++)
        {
                server0_list[i].busy = false;
                server0_list[i].server_id = i;
                server0_list[i].total_service_time = 0;
                server0_idle_stack[i] = i;
        }

        for(int i = 0; i < server1_max; i++)
        {
                server1_list[i].busy = false;
                server1_list[i].server_id = i;
                server1_list[i].total_service_time = 0;
                server1_idle_stack[i] = i;
        }

        //read in first customer
        inputFile >> first_c.arrival_time >> first_c.customer_type >> first_c.service_duration;
        //customer counter
        if(first_c.customer_type == 0) {customer0_queue_total += 1;}
        else {customer1_queue_total += 1;}

        //add to event queue
        event_heap[0].event_type = 0;
        event_heap[0].event_time = first_c.arrival_time;
        event_heap[0].cust = first_c;

        //main loop
        while(event_count >= 0)
        {
                //get next event
                Event current;
                current.event_type = event_heap[0].event_type;
                current.event_time = event_heap[0].event_time;
                current.cust = event_heap[0].cust;

                //set time
                current_time = current.event_time;

                //if arrival event
                if(current.event_type == 0)
                {
                        //deal with arrival event
                        //if server available
                        if(current.cust.customer_type == 0)
                        {
                                if(server0_idle_count > 0)
                                {
                                        //change arrival event into service completion event
                                        event_heap[0].event_type = 1;
                                        event_heap[0].event_time = current_time + current.cust.service_duration;
                                        event_heap[0].cust.server_id = server0_idle_stack[server0_idle_count-1];
                                        //remove server from idle list
                                        server0_list[server0_idle_stack[server0_idle_count-1]].busy = true;
                                        server0_idle_count -= 1;
                                }
				else if(server1_idle_count > 0 && queue1_count == 0)
                                {
                                        //change arrival event into service completion event
                                        event_heap[0].event_type = 1;
                                        event_heap[0].event_time = current_time + current.cust.service_duration;
                                        event_heap[0].cust.server_id = server1_idle_stack[server1_idle_count-1];

					//TOURIST CLASS CUSTOMER IS CONVERTED INTO BUSINESS CLASS CUSTOMER FOR SERVICE
					event_heap[0].cust.customer_type = 1;

			                //remove server from idle list
                                        server1_list[server1_idle_stack[server1_idle_count-1]].busy = true;
                                        server1_idle_count -= 1;
                                }
                                else
                                {
                                        //queue stat
                                        queue0_length_time += (queue0_count * (current_time - queue0_prev_time));
                                        queue0_prev_time = current_time;

                                        //put customer in queue
                                        enqueue(queue0, &queue0_tail, current.cust, &queue0_count);

                                        //more queue stat
                                        if(queue0_count > queue0_max) {queue0_max = queue0_count;}

                                        //remove completed arrival event
                                        swap_event(&event_heap[0], &event_heap[event_count]);
                                        event_count -= 1;
                                }
                        }
			else
                        {
                                if(server1_idle_count > 0)
                                {
                                        //change arrival event into service completion event
                                        event_heap[0].event_type = 1;
                                        event_heap[0].event_time = current_time + current.cust.service_duration;
                                        event_heap[0].cust.server_id = server1_idle_stack[server1_idle_count-1];
                                        //remove server from idle list
                                        server1_list[server1_idle_stack[server1_idle_count-1]].busy = true;
                                        server1_idle_count -= 1;
                                }
                                else
                                {
                                        //queue stat
                                        queue1_length_time += (queue1_count * (current_time - queue1_prev_time));
                                        queue1_prev_time = current_time;

                                        //put customer in queue
                                        enqueue(queue1, &queue1_tail, current.cust, &queue1_count);

                                        //more queue stat
                                        if(queue1_count > queue1_max) {queue1_max = queue1_count;}

                                        //remove completed arrival event
                                        swap_event(&event_heap[0], &event_heap[event_count]);
                                        event_count -= 1;
                                }
                        }
                        if(!dummy_flag)
                        {
                                //read next customer
                                Customer next_c;
                                inputFile >> next_c.arrival_time >> next_c.customer_type >> next_c.service_duration;
                                if(next_c.arrival_time == 0 && next_c.customer_type == 0 && next_c.service_duration == 0)
                                {
                                        dummy_flag = true;
                                }

                                //add next arrival event
                                else
                                {
                                        //customer counter
                                        if(next_c.customer_type == 0) {customer0_queue_total += 1;}
                                        else {customer1_queue_total += 1;}

                                        event_count += 1;
                                        event_heap[event_count].event_type = 0;
                                        event_heap[event_count].event_time = next_c.arrival_time;
                                        event_heap[event_count].cust = next_c;
                                }
                        }
                }
                //else service completion event
                else
                {
                        //deal with service completion event
                        if(current.cust.customer_type == 0)
                        {
                                server0_list[current.cust.server_id].total_service_time += current.cust.service_duration;

                                //business queue time
                                customer0_queue_time += (current_time - current.cust.arrival_time) + current.cust.service_duration;
                                customer0_service_time += current.cust.service_duration;
				customer0_total += 1;
                        }
                        else
                        {
                                server1_list[current.cust.server_id].total_service_time += current.cust.service_duration;

                                //tourist queue time
                                customer1_queue_time += (current_time - current.cust.arrival_time) + current.cust.service_duration;
                                customer1_service_time += current.cust.service_duration;
				customer1_total += 1;
                        }

                        //service completion stat
                        last_service_time = current_time + current.cust.service_duration;
                        total_service_time += (current_time - current.cust.arrival_time) + current.cust.service_duration;

                        //if there is a queue for the opened server type
                        if(current.cust.customer_type == 0 && queue0_count > 0)
                        {
                                //add service completion event from front of customer queue
                                Customer next_c;
                                next_c = queue0[queue0_head];
                                event_heap[0].event_type = 1;
                                event_heap[0].event_time = current_time + next_c.service_duration;
                                event_heap[0].cust = next_c;
                                event_heap[0].cust.server_id = current.cust.server_id;

                                //queue stat
                                queue0_length_time += (queue0_count * (current_time - queue0_prev_time));
                                queue0_prev_time = current_time;

                                dequeue(&queue0_head, &queue0_count);

                        }
                        else if(current.cust.customer_type == 1 && queue1_count > 0)
                        {
                                Customer next_c;
                                next_c = queue1[queue1_head];
                                event_heap[0].event_type = 1;
                                event_heap[0].event_time = current_time + next_c.service_duration;
                                event_heap[0].cust = next_c;
                                event_heap[0].cust.server_id = current.cust.server_id;

                                //queue stat
                                queue1_length_time += (queue1_count * (current_time - queue1_prev_time));
                                queue1_prev_time = current_time;

                                dequeue(&queue1_head, &queue1_count);
                        }
			//if business class server has no queue, they will server a tourist class customer
			else if(current.cust.customer_type == 1 && queue0_count > 0)
			{
                                //add service completion event from front of customer queue
                                Customer next_c;
                                next_c = queue0[queue0_head];
                                event_heap[0].event_type = 1;
                                event_heap[0].event_time = current_time + next_c.service_duration;
                                event_heap[0].cust = next_c;
                                event_heap[0].cust.server_id = current.cust.server_id;

			//CUSTOMER IS CONSIDERED TOURIST CLASS UP UNTIL THEY ARE SERVED
			//THEY ARE THEN CONSIDERED BUSINESS CLASS FOR FUTURE COUNTS
				event_heap[0].cust.customer_type = 1;

                                //queue stat
                                queue0_length_time += (queue0_count * (current_time - queue0_prev_time));
                                queue0_prev_time = current_time;

                                dequeue(&queue0_head, &queue0_count);
			}

                        //else make server idle
                        else
                        {
                                //make server idle
                                int tempID = current.cust.server_id;
                                if(current.cust.customer_type == 0)
                                {
                                        server0_list[tempID].busy = false;
                                        server0_idle_stack[server0_idle_count] = tempID;
                                        server0_idle_count += 1;
                                }
                                else
                                {
                                        server1_list[tempID].busy = false;
                                        server1_idle_stack[server1_idle_count] = tempID;
                                        server1_idle_count += 1;
                                }

                                //remove completed service event
                                swap_event(&event_heap[0], &event_heap[event_count]);
                                event_count -= 1;
                        }
                }
                min_heapify(event_heap, 0);
        }

        //calculate and printout statistics
        print_stats(2, server0_list, server1_list);


        //close file
        inputFile.close();

	return 0;
}

void swap_event(Event *A, Event *B)
{
	Event temp = *A;
	*A = *B;
	*B = temp;
}

void dequeue(int* head, int* count)
{
	*head = (*head + 1) % 500;
	*count -= 1;
}

void enqueue(Customer arr[], int* tail, Customer cust, int* count)
{
	*tail = (*tail + 1) % 500;
	arr[*tail] = cust;
	*count += 1;

	if((queue0_count + queue1_count) > max_queue_length) {max_queue_length = queue0_count + queue1_count;}
}

void min_heapify(Event arr[], int i)
{
	int smallest = i;
	if(LEFT(i) <= event_count && arr[LEFT(i)].event_time < arr[i].event_time)
	{
		smallest = LEFT(i);
	}

	if(RIGHT(i) <= event_count && arr[RIGHT(i)].event_time < arr[smallest].event_time)
	{
		smallest = RIGHT(i);
	}

	if(smallest != i)
	{
		swap_event(&arr[i], &arr[smallest]);
		min_heapify(arr, smallest);
	}
}

void print_stats(int i, Server server0_list[], Server server1_list[])
{
	cout << endl;
	if(i == 1) {cout << "Pass 1: Business severs exclusively server business class" << endl;}
	else if(i == 2) {cout << "Pass 2: Idle business servers may serve tourist class" << endl;}

	cout << left << setw(50) << "Number of people served:" << " " << customer0_total + customer1_total << endl;
	cout << left << setw(50) << "Time last service is completed:" << " " << last_service_time << endl << endl;

	cout << left << setw(50) << "Business class customers:" << endl;
	cout << left << setw(50) << "Average total service time:" << " " << (customer1_service_time + customer1_queue_time) / customer1_total << endl;
	cout << left << setw(50) << "Average total time in queue:" << " " << customer1_queue_time / customer1_queue_total << endl;
	cout << left << setw(50) << "Ave length of queue:" << " " << queue1_length_time / last_service_time << endl;
	cout << left << setw(50) << "Maximum number queued:" << " " << queue1_max << endl << endl;

        cout << left << setw(50) << "Tourist class customers:" << endl;
        cout << left << setw(50) << "Average total service time:" << " " << (customer0_service_time + customer0_queue_time) / customer0_total << endl;
        cout << left << setw(50) << "Average total time in queue:" << " " << customer0_queue_time / customer0_queue_total << endl;
        cout << left << setw(50) << "Ave length of queue:" << " " << queue0_length_time / last_service_time << endl;
        cout << left << setw(50) << "Maximum number queued:" << " " << queue0_max << endl << endl;

        cout << left << setw(50) << "All customers:" << endl;
        cout << left << setw(50) << "Average total service time:" << " " << (total_service_time + customer0_queue_time + customer1_queue_time) / (customer0_total + customer1_total) << endl;
        cout << left << setw(50) << "Average total time in queue:" << " " << (customer0_queue_time + customer1_queue_time) / (customer0_queue_total + customer1_queue_total) << endl;

	//total average length of queue, different to equation from lecture notes
	//taking the average of two average queue lengths required division by 2
	//could be wrong, but that is my reasoning
        cout << left << setw(50) << "Ave length of queue:" << " " << (queue0_length_time + queue1_length_time) / last_service_time / 2 << endl;
        cout << left << setw(50) << "Maximum number queued:" << " " << max_queue_length << endl << endl;

	cout << left << setw(50) << "Business class servers:" << endl;
        for(int i = 0; i < server1_max; i++)
	{
		cout << left << "Total idle time for business class server " << i << setw(7) << ":" << " " << last_service_time - server1_list[i].total_service_time << endl;
	}
	cout << endl;

        cout << left << setw(50) << "Tourist class servers:" << endl;
        for(int i = 0; i < server0_max; i++)
        {
                cout << left << "Total idle time for tourist class server " << i << setw(8) << ":" << " " << last_service_time - server0_list[i].total_service_time << endl;
        }
        cout << endl << endl;
}
