//#include "stdafx.h"
#include "GraphM.h"
//------------------------------------------------------------------------------------
//constructor
//creates and empty graph
GraphM::GraphM()
{
  for(int i = 0; i < MAXNODES; i++)
  {
    for(int j = 0; j < MAXNODES; j++)
    {
      edgeArray[i][j] = 0;
      pathArray[i][j].pathLength = 999999999;
      pathArray[i][j].penultimatePath = 0;
      pathArray[i][j].visited = false;
    }
  }
}
//------------------------------------------------------------------------------
//destructor
//no dynamic allocation in this class so the default destructor is fine
GraphM::~GraphM()
{
}
//------------------------------------------------------------------------------
//Builds a graph from the input from the file stream
//This function assumes correctly formated input.
//If the the input is incorrectly formatted then results will be upredictable.
//After building the graph both the graphData and edge arrays will be have 
//the nodes from 1 - the numberOfNodes filled with initialised data.
//This function uses the insetEdge function.
void GraphM::buildGraph(ifstream &inFie)
{
	//get the number of nodes
	string data;
	int numberOfNodes = -1;
	inFie >> numberOfNodes;

		//check if the file input is valid
	if (numberOfNodes == -1)
	{
		getline(inFie, data);
		return;
	}


	this->numberOfNodes = numberOfNodes;

	//wierd interaction between the >> operator and getLine
	//After using the >> operator on the last item in the line, then the next getLine returns 
	//an empty string "".
	//this get line command serves to reset the stream so the next get line command will function properly
	getline(inFie, data);

	//read in data for each node, ignoring node 0.
	for (int i = 1; i <= numberOfNodes; i++)
	{
		getline(inFie, data);
		NodeData temp(data);
		graphData[i] = temp;
	}

	//read in edges and add them to the graph
	int startNode;
	int endNode;
	int wieght;

	while (true)
	{
		inFie >> startNode >> endNode >> wieght;
		//check if the first number is 0, which would indicate that this graph is done.
		if (startNode == 0)
		{
			return;
		}
		insertEdge(startNode, endNode, wieght);
	}
}
//------------------------------------------------------------------------------
//Adds edges to the edge array
//this function checks data to make sure the edge will be inserted
//to connects existing nodes.
bool GraphM::insertEdge(int startNode, int endNode, int wieght)
{
	if (startNode <= numberOfNodes && endNode <= numberOfNodes)
	{
		edgeArray[startNode][endNode] = wieght;
		return true;
	}
	else
	{
		return false;
	}
}
//------------------------------------------------------------------------------
//Resets an edge to 0, effectively removing it from the array
//this function checks data to make sure the edge to be removed
//connects existing nodes.
void GraphM::removeEdge(int startNode, int endNode)
{
	if (startNode <= numberOfNodes && endNode <= numberOfNodes)
	{
		edgeArray[startNode][endNode] = 0;
	}

}
//------------------------------------------------------------------------------
//calls path helper for each node in the array
//in this way a path from each node to every other node will be created.
void GraphM::findShortestPath()
{

	//loop through each node and use it as the starting position for the algorithm
	for (int i = 1; i <= numberOfNodes; i++)
	{
		pathHelper(i);
	}
}
//------------------------------------------------------------------------------
//starting from the cource node, dijkstra's algorithm is used to get the shortest
// path from the source node to every other node
void GraphM::pathHelper(int sourceNode)
{
	//declare variables that will be used in the next loop, and add the 
	//path to the source node to the path array
	int currentNode = sourceNode;
	pathArray[sourceNode][sourceNode].pathLength = 0;
	pathArray[sourceNode][sourceNode].visited = true;

	while (true)
	{
		// add all the potential moves that can be taken to the potentialMove array
		for (int i = 1; i <= numberOfNodes; i++)
		{
			//check for an edge at the current index
			if (edgeArray[currentNode][i] != 0)
			{
				//if the edge at the current node makes a shorter path than already existis in the path array
				// (including when the path is empty) set the path to the new length and change the penUltimatePath
				// to the current node
				if (pathArray[sourceNode][i].pathLength > (pathArray[sourceNode][currentNode].pathLength + edgeArray[currentNode][i]))
				{
					pathArray[sourceNode][i].pathLength = (pathArray[sourceNode][currentNode].pathLength + edgeArray[currentNode][i]);
					pathArray[sourceNode][i].penultimatePath = currentNode;
				}
			}
		}

		int shortestPath = 999999999;
		int nextNode = 0;
		//find the shortest path to a node that has not been visited yet
		for (int i = 1; i <= numberOfNodes; i++)
		{
			//if the path at i is the shortest path encountered
			if (pathArray[sourceNode][i].pathLength < shortestPath && !pathArray[sourceNode][i].visited)
			{
				shortestPath = pathArray[sourceNode][i].pathLength;
				nextNode = i;
			}
		}
		//check if there is a next move, if so take it, if not the algorithm is done.
		if (nextNode != 0)
		{
			pathArray[sourceNode][nextNode].visited = true;
			currentNode = nextNode;
		}
		else
		{
			return;
		}
	}
}
//------------------------------------------------------------------------------
//prints out all the shortest paths in the path array to cout.
//This function assumes that the function findShortestPaths has been called,
//otherwise it will not function as intended.
//this function uses the helper function displayPathHelper
void GraphM::displayAll()const
{
	//print out the column diescriptions, once
	cout << "Description\t\t From Node   To Node   Dijkstra's  Path" << endl;

	//loop through the pathArray and print the paths for every starting location i
	for (int i = 1; i <= numberOfNodes; i++)
	{
		//print out the NodeData, for current node
		cout << graphData[i] << endl;

		//loop through each path that starts at node i
		for (int j = 1; j <= numberOfNodes; j++)
		{
			//check if i and j are equal, if so don't print anything
			if (i != j)
			{
				//check if there is a valid path at the current index
				if (pathArray[i][j].visited)
				{
					//print out from node, to node, and length
					cout << "\t\t\t\t" << i << "\t" << j << "\t   " << pathArray[i][j].pathLength << "\t   ";

					//print out the path stored in the path array
					displayPathHelper(i, j);
				}
				else
				{
					cout << "\t\t\t\t" << i << "\t" << j << "\t   ---";
				}
				cout << endl;
			}
		}
	}
	cout << endl;
}
//------------------------------------------------------------------------------
//Recursively prints out the path from the source node to the destination node
//The function recursivley calls iteself first before printing to cout,
//in this way the path will be printed in the correct order
void GraphM::displayPathHelper(int sourceNode, int destinationNode)const
{
	//first recursively call the next iteration so that the printing occurs from the bottom of the recusrive stack up
	if (destinationNode != sourceNode)
	{
		displayPathHelper(sourceNode, pathArray[sourceNode][destinationNode].penultimatePath);
	}
	//print the destination node
	cout << destinationNode << " ";
}
void GraphM::display(int firstNode, int secondNode)const
{
	//check if the path to display actually exists
	if (firstNode <= numberOfNodes && secondNode <= numberOfNodes && pathArray[firstNode][secondNode].visited)
	{
		//print out path info
		cout << "\t" << firstNode << " \t" << secondNode <<
			"\t" << pathArray[firstNode][secondNode].pathLength << "\t";
		
		//print out path details
		displayPathHelper(firstNode, secondNode);
		cout << endl;
		//print out path details
		displayPathDetailsHelper(firstNode, secondNode);
		cout << endl;
	}
}
//------------------------------------------------------------------------------
//Recursively prints out the node data starting with
//the source node to the destination node.
//The function recursivley calls iteself first before printing to cout,
//in this way the path will be printed in the correct order
void GraphM::displayPathDetailsHelper(int sourceNode, int destinationNode)const
{
	//first recursively call the next iteration so that the printing occurs from the bottom of the recusrive stack up
	if (destinationNode != sourceNode)
	{
		displayPathDetailsHelper(sourceNode, pathArray[sourceNode][destinationNode].penultimatePath);
	}
	//print the destination node
	cout << graphData[destinationNode] << endl;
}