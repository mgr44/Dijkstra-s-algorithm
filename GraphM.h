#pragma once

#include <iostream>
#include "NodeData.h"
using namespace std;
//------------------------------------------------------------------------------
//This class is designed to store a graph using two seperate arrays.
//The first array stores the NodeData which holds the data for each node in the
//graph
//The second array is a two dimensional edge array that stores the edges between
//nodes as a weighted value.

//This class has functions to build a graph from a filestream
// ,add edges
// ,remove edges
// ,use dijkstra's algorithm to find the shortest path from each node
// to every other node
// ,and to display nodes all together or individualy.

//This class does not do any input checking. It is assumed that all input in
//the file stream is correctly formated.
//The graph cannot be larger than 100 nodes. This maximum size is set by the
//const MAXNODES
static const int MAXNODES = 100;

class GraphM
{
public:

	GraphM();
	~GraphM();

	void buildGraph(ifstream &inFie);
	bool insertEdge(int startNode, int endNode, int wieght);
	void removeEdge(int startNode, int endNode);
	void findShortestPath();
	void displayAll()const;
	void display(int firstNode, int secondNode)const;

private:

	struct TableType
	{
		int pathLength;
		int penultimatePath;
		bool visited;
	};
	struct MoveOption
	{
		int end;
		TableType path;
	};

	void pathHelper(int sourceNode);
	void displayPathHelper(int sourceNode, int destinationNode)const;
	void displayPathDetailsHelper(int sourceNode, int destinationNode)const;

	int numberOfNodes;
	NodeData graphData[MAXNODES];
	int edgeArray[MAXNODES][MAXNODES];
	TableType pathArray[MAXNODES][MAXNODES];
};
