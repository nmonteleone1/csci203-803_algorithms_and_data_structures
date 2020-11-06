//////////////////////////////////////////////
// CSCI803 - Algorithms and Data Structures //
//                                          //
// Assignment 3                             //
//                                          //
// Nicholas Monteleone              5055076 //
//////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;

//definitions
#define INT_MAX 2147483647

//structures

//functions
int minFind(double distances[], bool candidates[]);

//global variables
int nVertices, nEdges;
bool second_path = false;
double second_length;

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

	//read from file and process
	inputFile >> nVertices >> nEdges;

	//initilizations
	double graph[nVertices][2];
	graph[0][0] = 0;
	graph[0][1] = 0;
	double weights[nVertices][nVertices]{0};
        double temp_weights[nVertices][nVertices]{0};
	int path[nVertices];
	int prev;
	int m, n;

	//more file reading and processing
	int counter = 0;
	int label;
	while(counter < nVertices)
	{
		inputFile >> label >> graph[counter][0] >> graph[counter][1];
		counter += 1;
	}

	int edge1, edge2;
	double temp_weight, temp_distance;
	counter = 0;
	while(counter < nEdges)
	{
		inputFile >> edge1 >> edge2 >> temp_weight;
		edge1 -= 1;
		edge2 -= 1;

		//if the edge has been defined before with a lower value, keep that value
		if(weights[edge1][edge2] != 0 && (weights[edge1][edge2] < temp_weight || weights[edge2][edge1] < temp_weight))
		{
			counter += 1;
			continue;
		}

		//if the assigned weight is lower than the euclidean distance, use the distance instead (SORRY IF INCORRECT, BASED ON POINT 3 IN DATA FILE CONSTRUCTION LAYOUT)
		temp_distance = sqrt( pow(graph[edge1-1][1] - graph[edge2-1][1],2) + pow(graph[edge1-1][2] - graph[edge2-1][2],2) );
		if(temp_weight < temp_distance)
		{
			weights[edge1][edge2] = temp_distance;
	                weights[edge2][edge1] = temp_distance;
		}

		else
		{
        	        weights[edge1][edge2] = temp_weight;
	                weights[edge2][edge1] = temp_weight;
		}

		counter += 1;
	}

	//define source and destination vertices
	int src, end;
	inputFile >> src >> end;
	src -= 1;
	end -= 1;

	//close file
	inputFile.close();

	//dijkstra's algorithm//
	int min_index;
	double distances[nVertices];
	bool candidates[nVertices];
	int parents[nVertices];

	//goto flag
	DIJKSTRA:
	//make all nodes unvisited, and make their distance infinity
	for(int i = 0; i < nVertices; i++)
	{
		distances[i] = INT_MAX;
		candidates[i] = false;
		parents[i] = src;
	}

	distances[src] = 0;	//source node actually has no distance

	for(int i = 0; i < nVertices; i++)
	{
		//find minimum distance in set of unread nodes
		min_index = minFind(distances, candidates);
		candidates[min_index] = true;

		for(int j = 0; j < nVertices; j++)
		{
			if(!candidates[j] && weights[min_index][j] && distances[min_index] != INT_MAX && distances[min_index] + weights[min_index][j] < distances[j])
			{
				distances[j] = distances[min_index] + weights[min_index][j];
				parents[j] = min_index;
			}
		}
	}
	//END DIJKSTRA//

	//if the processing for the second path has already begun skip first path outputs
	if(second_path)
	{
		goto LOOP;
	}

	//finding the path taken
	prev = parents[end];
	counter = 0;
	path[0] = end;
	while(path[counter] != src)
	{
		counter += 1;
		path[counter] = prev;
		prev = parents[prev];
	}

	//outputs
	cout << "\nPath from " << src+1 << " to " << end+1 << " visited " << counter+1 << " nodes." << endl;
	for(int i = counter; i > 0; i--)
	{
		cout << path[i]+1 << " -> ";
	}
	cout << path[0]+1 << endl;
	cout << "Path Weight : " << distances[path[0]] << endl;

	//second shortest path
	second_path = true;
	second_length = INT_MAX;

	//remove connections from each node that was in the shortest path, and rerun dijkstra for that instance
	for(m = counter; m >= 0; m--)
	{
		//if it is a 2 node path, remove connections just between those two nodes
		if(path[m] == end && parents[path[m]] == src)
		{
			n = src;
                        temp_weights[path[m]][n] = weights[path[m]][n];
                        temp_weights[n][path[m]] = weights[n][path[m]];
                        weights[path[m]][n] = 0;
                        weights[n][path[m]] = 0;
			goto DIJKSTRA;
		}
		if(path[m] == src)
		{
			continue;
		}
		for(n = 0; n < nVertices; n++)
		{
			temp_weights[path[m]][n] = weights[path[m]][n];
			temp_weights[n][path[m]] = weights[n][path[m]];
			weights[path[m]][n] = 0;
			weights[n][path[m]] = 0;
		}

		goto DIJKSTRA;
		//goto flag
		LOOP:
		//if this path is shorter than all previous (except first)
		if(distances[end] < second_length)
		{
			second_length = distances[end];

		        //finding the path taken
		        prev = parents[end];
		        counter = 0;
		        path[0] = end;
		        while(path[counter] != src)
		        {
		                counter += 1;
		                path[counter] = prev;
                		prev = parents[prev];
        		}
		}
                for(n = 0; n < nVertices; n++)
                {
                        weights[path[m]][n] = temp_weights[path[m]][n];
                        weights[n][path[m]] = temp_weights[n][path[m]];
                }

	}

        //second path outputs
        cout << "\nSecond shortest path from " << src+1 << " to " << end+1 << " visited " << counter+1 << " nodes." << endl;
        for(int i = counter; i > 0; i--)
        {
                cout << path[i]+1 << " -> ";
        }
        cout << path[0]+1 << endl;
        cout << "Second Shortest Path Weight : " << distances[path[0]] << endl;

	return 0;
}

int minFind(double distances[], bool candidates[])
{
	int min_value = INT_MAX;
	int min_index;

	for(int i = 0; i < nVertices; i++)
	{
		if(candidates[i] == false && distances[i] <= min_value)
		{
			min_value = distances[i];
			min_index = i;
		}
	}

	return min_index;
}
