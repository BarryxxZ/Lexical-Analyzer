#pragma once
#ifndef ADJTABLE_H_
#define ADJTABLE_H_

#include"base.h"

class Edge
{
public:
	int number;	//arrival state
	int position;	//position of edge
	char weight;
	Edge *link;

	Edge();
	Edge(int num, int pos, char ch);
};

class Vertex
{
public:
	int number;
	Vertex *next;
	Edge *out;
	Vertex();
	Vertex(int num);
};

class AdjTable
{
private:
	Vertex *startVertex;
	int numOfVertices;
	int numOfEdges;
public:
	AdjTable();
	~AdjTable();

	int GetValueByPos(int pos) const;

	int GetPosByValue(int value) const;

	void SetValue(int value, int pos);

	void InsertVertex(int value);

	void InsertEdgeByPos(int v1, int v2, char weight);

	void InsertEdgeByValue(int value1, int value2, char weight);

	void RemoveAllEdges(void);

	void Clear();

	int* Closure(int *T);

	int* Move(int *T, char ch);

	void OutputNFA();
};

#endif
