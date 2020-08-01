#pragma once
#ifndef DFA_H_
#define DFA_H_

#include"base.h"
#include"AdjTable.h"
#include"TranTable.h"
#include"LinkedStack.cpp"

class DFA
{
private:
	char *regex;
	char *suffix;
	char *edge;
	char word[MAX];
	int edgeNumber;
	int **DStates;
	int **Dtran;
	int **Move;
	int *AcceptStates;
	string FinalState;
	int DStatesNumber;
	int DtranNumber;
	int NFAStatesNumber;
	int DFAStatesNumber;
	AdjTable *NFATable;
	TranTable *DFATable;

	int Precedence(char symbol);

	int CompArray(int *t1, int *t2);

	int MinimizeDFAStates(int **Dtran, int *AcceptStates, int DtranNumber, int edgeNumber);
public:
	DFA();
	~DFA();

	void GetRegExp();

	void processRegExp();

	void toSuffix();

	void GetEdgeNumber();

	void ThompsonConstruction();

	void SubsetConstruction();

	void check();
};

#endif
