#include "GraphL.h"

//------------------------------------------------------------------------------
//Default constructor that will result in an empty graph
GraphL::GraphL()
{
  for(int i = 0; i < MAXNODES; i++)
  {
    graph[i].data = NULL;
    graph[i].edgeHead = NULL;
    graph[i].visited = false;
  }
}

//------------------------------------------------------------------------------
//Destructor
//deletes the structs edgeNodes that are dynamically allocated with pointers
//calls the helper function deleteEdges
GraphL::~GraphL()
{
	//loop through all the nodes in the array that have been filled and 
	// delete the data pointer and edgehead pointer
	for (int i = 1; i <= numberOfNodes; i++)
	{
		deleteEdges(graph[i].edgeHead);
		delete graph[i].data;
	}
}
//------------------------------------------------------------------------------
//Delete all the edge Nodes n the adjacency matrix
//Helper function that is used by the destructor
void GraphL::deleteEdges(EdgeNode *&currentEdge) 
{
	//check if the current edge is NULL, if so return
	if (currentEdge != NULL)
	{
		//delete any edges further down the list
		deleteEdges(currentEdge->nextEdge);
		//delete the current edge
		delete currentEdge;
		currentEdge = NULL;
	}
}
//------------------------------------------------------------------------------
//Builds the graph from the input of the filestream
//this function does not check the input data,
//it is assumed that all input is correctly formated
//After this function is called the array of graph nodes and their edges 
//will be filled
//Uses the addEdgeHelper to add the edges to the adjacency array
void GraphL::buildGraph(ifstream &inFie)
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
	//After using the >> operator on the last item in the line, then next getLine returns 
	//an empty string.
	//this get line command serves to reset the stream so the next get line command will function properly
	getline(inFie, data);

	//read in data for each node, ignoring node 0.
	for (int i = 1; i <= numberOfNodes; i++)
	{
		getline(inFie, data);
		graph[i].data = new NodeData(data);
	}

	//read in edges and add them to the graph
	int startNode;
	int endNode;

	while (true)
	{
		inFie >> startNode >> endNode;
		//check if the first number is 0, which would indicate that this graph is done.
		if (startNode == 0)
		{
			return;
		}
		addEdgeHelper(startNode, endNode);
	}
}
//------------------------------------------------------------------------------
//Adds edges to the adjacency array
//this function does not do any checks on the input
//edges are added to the list at the front
void GraphL::addEdgeHelper(int startNode, int endNode)
{
	EdgeNode temp;
	temp.terminalNode = endNode;
	temp.nextEdge = graph[startNode].edgeHead;

	graph[startNode].edgeHead = new EdgeNode(temp);
}
//------------------------------------------------------------------------------
//Prints out the graph to cout
void GraphL::displayGraph()const
{
	cout << "Graph" << endl;
	//print each node and it's edges
	for (int i = 1; i <= numberOfNodes; i++)
	{
		//print the node
		cout << "Node " << i << "\t\t" << *(graph[i].data) << endl;

		//print the edges
		EdgeNode *current = graph[i].edgeHead;
		while (current != NULL)
		{
			cout << "   Edge " << i << " " << current->terminalNode << endl;
			current = current->nextEdge;

		}
	}
	cout << endl;
}
//------------------------------------------------------------------------------
//preforms a depth first search, and prints the path out to cout
//calls the recursiveDepthFirstHelper function
//then resets the visited bool of graphNodes in order to allow
//the depth first search to be preformed again
void GraphL::depthFirstSearch()
{
	cout << "Depth-first ordering: ";
	//recursively preform the depth first search
	recursiveDepthFirstHelper(1);

	//reset the visited values in the graph so that the search can be preformed again.
	for (int i = 1; i <= numberOfNodes; i++)
	{
		graph[i].visited = false;
	}
	cout << endl;
}
//------------------------------------------------------------------------------
//Recursivley moves through the adjacecny array
//with a depth first algorithm.
//Prints the parent Node to cout and then recursively calls itself on the next
//child in the list
void GraphL::recursiveDepthFirstHelper(int currentNodeIndex)
{
	//base case, check if the current node has already been visited. 
	//If so, return.
	if (graph[currentNodeIndex].visited)
	{
		return;
	}
	else
	{
		//print out the current Node and mark it as visited
		cout << currentNodeIndex << " ";
		graph[currentNodeIndex].visited = true;

		// recursivly print out all the children
		EdgeNode* currentEdge = graph[currentNodeIndex].edgeHead;
		while (currentEdge != NULL)
		{
			recursiveDepthFirstHelper(currentEdge->terminalNode);
			currentEdge = currentEdge->nextEdge;
		}
	}

}