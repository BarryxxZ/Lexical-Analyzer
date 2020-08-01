#include"DFA.h"

DFA::DFA()
{
	regex = new char[MAX];
	suffix = new char[MAX];
	edge = new char[MAX];
	edgeNumber = 0;
	NFAStatesNumber = 0;
	DFAStatesNumber = 0;
	DStatesNumber = 0;
	DtranNumber = 0;
	NFATable = new AdjTable();
}

DFA::~DFA()
{
	delete[] regex;
	delete[] suffix;
	delete[] edge;
	delete[] AcceptStates;
	NFATable->Clear();
	DFATable->Clear();
}

void DFA::GetRegExp()
{
	cout << "Please input regex(program will connect each charactes with '&'):" << endl;
	cin >> regex;
}

void DFA::processRegExp()
{
	int i = 0, j, len = strlen(regex);
	while (regex[i + 1] != '\0')
	{
		if (((regex[i] != '(' && regex[i] != '&' && regex[i] != '|')
			|| regex[i] == ')'
			|| regex[i] == '*')
			&& (regex[i + 1] != ')' && regex[i + 1] != '&' && regex[i + 1] != '|' && regex[i + 1] != '*'))
		{
			for (j = len; j > i + 1; j--)
			{
				regex[j] = regex[j - 1];
			}
			regex[i + 1] = '&';
			len++;
			regex[len] = '\0';
			i++;
		}
		i++;
	}
}

int DFA::Precedence(char symbol)
{
	int priority;
	switch (symbol)
	{
	case '|':
		priority = 1;
		break;
	case '&':
		priority = 2;
		break;
	case '*':
		priority = 3;
		break;
	default:
		priority = 0;
		break;
	}
	return priority;
}

void DFA::toSuffix()
{
	int i = 0, j = 0;
	char ch, cl;
	strcpy_s(suffix, 1, "\0");
	LinkedStack<char> *ls = new LinkedStack<char>();
	ls->Clear();
	ls->Push('#');
	ch = regex[i];
	while (ch != '\0')
	{
		if (ch == '(')
		{
			ls->Push(ch);
			ch = regex[++i];
		}
		else if (ch == ')')
		{
			while (ls->Front() != '(')
			{
				suffix[j++] = ls->Pop();
			}
			ls->Pop();
			ch = regex[++i];
		}
		else if ((ch == '|') || (ch == '*') || (ch == '&'))
		{
			cl = ls->Front();
			while (Precedence(cl) >= Precedence(ch))
			{
				suffix[j++] = cl;
				ls->Pop();
				cl = ls->Front();
			}
			ls->Push(ch);
			ch = regex[++i];
		}
		else
		{
			suffix[j++] = ch;
			ch = regex[++i];
		}
	}
	ch = ls->Pop();
	while ((ch == '|') || (ch == '*') || (ch == '&'))
	{
		suffix[j++] = ch;
		ch = ls->Pop();
	}
	suffix[j] = '\0';
	ls->Clear();
}

void DFA::GetEdgeNumber()
{
	int count = 0;
	int i = 0;
	int flag[94] = { 0 };
	while (suffix[i] != '\0') 
	{
		if ((suffix[i] - '!' >= 0 && suffix[i] - '!' < 94) &&
			(suffix[i] != '&' && suffix[i] != '|' && suffix[i] != '*'))
		{
			if (flag[suffix[i] - ' '] == 0) 
			{
				edge[count] = suffix[i];
				count++;
				flag[suffix[i] - ' '] = 1;
			}
		}
		i++;
	}
	edgeNumber = count;
	edge[count] = '\0';
}

void DFA::ThompsonConstruction()
{
	int i, j;
	char ch;
	int s1, s2;
	LinkedStack<int> *states = new LinkedStack<int>();
	states->Clear();
	if (strlen(suffix) < 1)
	{
		cout << "Regex is invalid!" << endl;
		_getch();
		exit(1);
	}
	NFATable->SetValue(0, 0);
	i = 1;
	j = 0;
	ch = suffix[j];
	while (ch != '\0')
	{
		if (ch == '&')
		{
			s2 = states->Pop();
			int temp1 = states->Pop();
			int temp2 = states->Pop();
			s1 = states->Pop();
			NFATable->InsertEdgeByValue(temp2, temp1, '$');
			states->Push(s1);
			states->Push(s2);
		}
		else if (ch == '|')
		{
			s2 = states->Pop();
			int temp1 = states->Pop();
			int temp2 = states->Pop();
			s1 = states->Pop();
			NFATable->InsertVertex(i);
			NFATable->InsertVertex(i + 1);
			NFATable->InsertEdgeByValue(i, s1, '$');
			NFATable->InsertEdgeByValue(i, temp1, '$');
			NFATable->InsertEdgeByValue(temp2, i + 1, '$');
			NFATable->InsertEdgeByValue(s2, i + 1, '$');
			s1 = i;
			s2 = i + 1;
			states->Push(s1);
			states->Push(s2);
			i += 2;
		}
		else if (ch == '*')
		{
			s2 = states->Pop();
			s1 = states->Pop();
			NFATable->InsertVertex(i);
			NFATable->InsertVertex(i + 1);
			NFATable->InsertEdgeByValue(i, i + 1, '$');
			NFATable->InsertEdgeByValue(s2, s1, '$');
			NFATable->InsertEdgeByValue(i, s1, '$');
			NFATable->InsertEdgeByValue(s2, i + 1, '$');
			s1 = i;
			s2 = i + 1;
			states->Push(s1);
			states->Push(s2);
			i += 2;
		}
		else
		{
			NFATable->InsertVertex(i);
			NFATable->InsertVertex(i + 1);
			NFATable->InsertEdgeByValue(i, i + 1, ch);
			s1 = i;
			s2 = i + 1;
			states->Push(s1);
			states->Push(s2);
			i += 2;
		}
		j++;
		ch = suffix[j];
	}
	s2 = states->Pop();
	s1 = states->Pop();
	NFATable->InsertEdgeByValue(0, s1, '$');
	if (!states->IsEmpty())
	{
		cout << "regex is wrong!" << endl;
		_getch();
		exit(1);
	}
	NFAStatesNumber = s2 + 1;
}

int DFA::CompArray(int *t1, int *t2)
{
	int len1 = 0, len2 = 0;
	while (t1[len1] != -1)
	{
		len1++;
	}
	while (t2[len2] != -1)
	{
		len2++;
	}
	sort(t1, t1 + len1);
	sort(t2, t2 + len2);
	if (equal(t1, t1 + len1, t2)) {
		return 1;
	}
	else {
		return 0;
	}
}

int DFA::MinimizeDFAStates(int **Dtran, int *AcceptStates, int DtranNumber, int edgeNumber)
{
	int h, i, j, k, l;
	for (i = 0; i < DtranNumber - 1; i++)
	{
		for (j = i + 1; j < DtranNumber; j++)
		{
			if (AcceptStates[i] == AcceptStates[j])
			{
				for (k = 0; k < edgeNumber; k++)
				{
					if (Dtran[i][k] != Dtran[j][k])
					{
						break;
					}
				}
				if (k == edgeNumber)
				{
					for (l = j; l < DtranNumber - 1; l++)
					{
						for (k = 0; k < edgeNumber; k++)
						{
							Dtran[l][k] = Dtran[l + 1][k];
						}
						AcceptStates[l] = AcceptStates[l + 1];
					}
					for (l = 0; l < DtranNumber - 1; l++)
					{
						for (k = 0; k < edgeNumber; k++)
						{
							if (Dtran[l][k] == j)
							{
								Dtran[l][k] = i;
							}
						}
					}
					for (h = j; h < DtranNumber; h++)
					{
						for (l = 0; l < DtranNumber - 1; l++)
						{
							for (k = 0; k < edgeNumber; k++)
							{
								if (Dtran[l][k] == h + 1)
								{
									Dtran[l][k] = h;
								}
							}
						}
					}
					DtranNumber--;
					j--;
				}
			}
		}
	}
	return DtranNumber;
}

void DFA::SubsetConstruction()
{
	int i, j, k;
	DStatesNumber = 0;
	DtranNumber = 0;
	cout << "*******************************" << endl;
	cout << "The table of NFA:" << endl;
	NFATable->OutputNFA();
	DStates = (int**)(new int*[NFAStatesNumber + 1]);
	for (i = 0; i < NFAStatesNumber + 1; i++)
	{
		DStates[i] = new int[NFAStatesNumber + 1];
	}
	Dtran = (int**)(new int*[NFAStatesNumber + 1]);
	for (i = 0; i < NFAStatesNumber + 1; i++)
	{
		Dtran[i] = new int[edgeNumber + 1];
	}
	for (i = 0; i < NFAStatesNumber + 1; i++)
	{
		for (j = 0; j < edgeNumber + 1; j++)
		{
			Dtran[i][j] = -1;
		}
	}
	AcceptStates = new int[NFAStatesNumber + 1];
	for (i = 0; i < NFAStatesNumber + 1; i++)
	{
		AcceptStates[i] = 0;
	}
	int *T = new int[NFAStatesNumber + 1];
	int *temp = new int[NFAStatesNumber + 1];
	T[0] = 0;
	T[1] = -1;
	T = NFATable->Closure(T);
	DStates[DStatesNumber] = T;
	DStatesNumber++;
	k = 0;
	while (k < DStatesNumber)
	{
		for (i = 0; edge[i] != '\0'; i++)
		{
			temp = NFATable->Closure(NFATable->Move(T, edge[i]));
			if (temp[0] != -1)
			{
				for (j = 0; j < DStatesNumber; j++)
				{
					if (CompArray(temp, DStates[j]) == 1)
					{
						Dtran[k][i] = j;
						break;
					}
				}
				if (j == DStatesNumber)
				{
					DStates[DStatesNumber] = temp;
					Dtran[k][i] = DStatesNumber;
					DStatesNumber++;
				}
			}
		}
		k++;
		T = DStates[k];
	}
	DtranNumber = k;
	for (i = 0; i < DStatesNumber; i++)
	{
		for (j = 0; DStates[i][j] != -1; j++)
		{
			if (DStates[i][j] == NFAStatesNumber - 1)
			{
				AcceptStates[i] = 1;
				break;
			}
		}
	}
	cout << "*******************************" << endl;
	for (i = 0; i < DStatesNumber; i++)
	{
		cout << "Epsilon-Closure(" << i << ")= {";
		j = 0;
		while (DStates[i][j] != -1)
		{
			cout << DStates[i][j] << " ";
			j++;
		}
		cout << "}" << endl;
	}
	cout << "Table of DFA:" << endl;
	cout << setw(7) << "states";
	for (j = 0; j < edgeNumber; j++)
	{
		cout << setw(7) << edge[j];
	}
	cout << setw(11) << "isEND" << endl;
	for (i = 0; i < DtranNumber; i++)
	{
		cout << setw(7) << i;
		for (j = 0; j < edgeNumber; j++)
		{
			if (Dtran[i][j] >= 0) {
				cout << setw(7) << Dtran[i][j];
			}
			else {
				cout << setw(7) << " ";
			}
		}
		if (AcceptStates[i] == 1) {
			cout << setw(11) << "(END)";
		}
		cout << endl;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	int DtranNumberAfterMinimization = MinimizeDFAStates(Dtran, AcceptStates, DtranNumber, edgeNumber);
	while (DtranNumberAfterMinimization != DtranNumber)
	{
		DtranNumber = DtranNumberAfterMinimization;
		DtranNumberAfterMinimization = MinimizeDFAStates(Dtran, AcceptStates, DtranNumber, edgeNumber);
	}
	DFATable = new TranTable(DtranNumber, edgeNumber);
	for (i = 0; i < DtranNumber; i++)
	{
		for (j = 0; j < edgeNumber; j++)
		{
			DFATable->SetValue(i, j, Dtran[i][j]);
		}
	}
	cout << "*******************************" << endl;
	cout << "Table of MinDFA:" << endl;
	cout << setw(7) << "states";
	for (j = 0; j < edgeNumber; j++)
	{
		cout << setw(7) << edge[j];
	}
	cout << setw(11) << "isEND" << endl;
	for (i = 0; i < DtranNumber; i++)
	{
		cout << setw(7) << i;
		for (j = 0; j < edgeNumber; j++)
		{
			if (DFATable->GetValue(i, j) >= 0) {
				cout << setw(7) << DFATable->GetValue(i, j);
			}
			else {
				cout << setw(7) << " ";
			}
		}
		if (AcceptStates[i] == 1)
		{
			FinalState.append(to_string(i));
			cout << setw(11) << "(END)";
		}
		cout << endl;
	}
	Move = (int**)(new int*[DtranNumber]);
	for (j = 0; j < DtranNumber; j++) {
		Move[j] = new int[edgeNumber];
	}
	for (j = 0; j < DtranNumber; j++) {
		for (int k = 0; k < edgeNumber; k++)
		{
			Move[j][k] = DFATable->GetValue(j, k);
		}
	}
	for (i = 0; i < NFAStatesNumber + 1; i++)
	{
		delete[] DStates[i];
		delete[] Dtran[i];
	}
	delete[] DStates;
	delete[] Dtran;
}

void DFA::check()
{
	cout << "please input string checked:" << endl;
	cin >> word;
	int size = strlen(word);
	int i = 0, col = 0;
	int row = 0;
	bool flag = false;
	while (i < size) {
		for (int j = 0; j < edgeNumber; j++) {
			if (word[i] == edge[j]) {
				flag = true;
				col = j;
				break;
			}
		}
		if (flag == false) {
			cout << "string is not matching!" << endl;
		}
		row = Move[row][col];
		if (i == size - 1) {
			if (FinalState.find(to_string(row)) != string::npos) {
				cout << "string is matching!" << endl;
				return;
			}
			if (FinalState.find(to_string(row)) == string::npos) {
				cout << "string is not matching!" << endl;
				return;
			}
		}
		i++;
	}
}