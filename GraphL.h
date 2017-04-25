#pragma once
#include <iostream>
#include "NodeData.h"
#include "GraphM.h"
//------------------------------------------------------------------------------
//this class is designed to store a graph in an adjacency matrix.
//The matrix consits of an array that holds the Nodes, with a 
// linked list of pointers that represent the edges of the grapgh

//This class has functions to buid the graph from a file stream
// ,display the graph
// , and display a depth first search of the graph starting from
// , the first Node

//This class does not do any input checking. It is assumed that all input in
//the file stream is correctly formated.
//The graph cannot be larger than 100 nodes. This maximum size is set by the
//const MAXNODES in graphm.h

class GraphL
{
public:
	GraphL();
	~GraphL();

	void buildGraph(ifstream &inFie);
	void displayGraph()const;
	void depthFirstSearch();

private:
	struct EdgeNode
	{
		int terminalNode;
		EdgeNode* nextEdge;
	};
	struct  GraphNode
	{
		NodeData* data;
		EdgeNode* edgeHead;
		bool visited;
	};

	void addEdgeHelper(int startNode, int endNode);
	void recursiveDepthFirstHelper(int currentNodeIndex);
	void deleteEdges(EdgeNode *&currentEdge);

	GraphNode graph[MAXNODES];
	int numberOfNodes;
};

