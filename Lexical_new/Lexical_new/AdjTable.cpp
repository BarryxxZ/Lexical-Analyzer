#include"AdjTable.h"

Edge::Edge()
{
	number = -1;
	position = -1;
	link = NULL;
}

Edge::Edge(int num, int pos, char ch) : number(num), position(pos), weight(ch), link(NULL) {}

Vertex::Vertex()
{
	number = -1;
	next = NULL;
	out = NULL;
}

Vertex::Vertex(int num)
{
	number = num;
	next = NULL;
	out = NULL;
};

AdjTable::AdjTable()
{
	numOfVertices = 1;
	numOfEdges = 0;
	startVertex = new Vertex();
}

AdjTable::~AdjTable()
{
	Vertex *p;
	Edge *q;
	p = startVertex;
	for (int i = 0; i < numOfVertices; i++)
	{
		q = p->out;
		while (q)
		{
			p->out = q->link;
			delete q;
			q = p->out;
		}
		p = p->next;
	}
}

int AdjTable::GetValueByPos(int pos) const
{
	if ((pos >= 0) && (pos < numOfVertices))
	{
		Vertex *p = startVertex;
		for (int i = 0; i < pos; i++)
		{
			p = p->next;
		}
		return p->number;
	}
	return -1;
}

int AdjTable::GetPosByValue(int value) const
{
	Vertex *p = startVertex;
	for (int i = 0; i < numOfVertices; i++)
	{
		if (p->number == value)
		{
			return i;
		}
		p = p->next;
	}
	return -1;
}

void AdjTable::SetValue(int value, int pos)
{
	if ((pos < 0) || (pos >= numOfVertices))
	{
		cout << "node does not exist!" << endl;
		_getch();
		exit(1);
	}
	Vertex *p = startVertex;
	for (int i = 0; i < pos; i++)
	{
		p = p->next;
	}
	p->number = value;
}

void AdjTable::InsertVertex(int value)
{
	int pos = GetPosByValue(value);
	if ((pos >= 0) && (pos < numOfVertices))
	{
		cout << "node is existing!" << endl;
		_getch();
		exit(1);
	}
	Vertex *p = startVertex;
	while (p->next)
	{
		p = p->next;
	}
	Vertex *newVertex = new Vertex(value);
	p->next = newVertex;
	numOfVertices++;
}

void AdjTable::InsertEdgeByPos(int v1, int v2, char weight)
{
	if ((v1 < 0) || (v1 >= numOfVertices) || (v2 < 0) || (v2 >= numOfVertices))
	{
		cout << "node does not exist!" << endl;
		_getch();
		exit(1);
	}
	Vertex *p = startVertex;
	for (int i = 0; i < v1; i++)
	{
		p = p->next;
	}
	Edge *q = p->out;
	Edge *newEdge = new Edge(GetValueByPos(v2), v2, weight);
	if (!q)
	{
		p->out = newEdge;
		numOfEdges++;
		return;
	}
	while ((q->position != v2) && (q->link))
	{
		q = q->link;
	}
	if (q->position == v2)
	{
		cout << "edge is existing!" << endl;
		_getch();
		exit(1);
	}
	if (!q->link)
	{
		q->link = newEdge;
		numOfEdges++;
	}
}

void AdjTable::InsertEdgeByValue(int value1, int value2, char weight)
{
	int v1 = GetPosByValue(value1), v2 = GetPosByValue(value2);
	InsertEdgeByPos(v1, v2, weight);
}

void AdjTable::RemoveAllEdges(void)
{
	Vertex *p = startVertex;
	for (int i = 0; i < numOfVertices; i++)
	{
		Edge *q = p->out;
		while (q)
		{
			p->out = q->link;
			delete q;
			q = p->out;
		}
		p = p->next;
	}
	numOfEdges = 0;
}

void AdjTable::Clear()
{
	RemoveAllEdges();
	Vertex *p = startVertex->next;
	while (p)
	{
		startVertex->next = p->next;
		delete p;
		p = startVertex->next;
	}
	numOfVertices = 1;
}

int* AdjTable::Closure(int *T)
{
	int i = 0, j, k = 0, l, len = 0;
	vector<int> temp;
	int *_temp = new int[MAX];
	Vertex *p;
	Edge *q;
	while (T[len] != -1)
	{
		len++;
	}
	while (T[i] != -1)
	{
		for (l = 0; l < temp.size(); l++)
		{
			if (T[i] == temp[l])
			{
				break;
			}
		}
		if (l == temp.size())
		{
			temp.push_back(T[i]);
		}
		int pos = GetPosByValue(T[i]);
		p = startVertex;
		for (j = 0; j < pos; j++)
		{
			p = p->next;
		}
		q = p->out;
		while (q)
		{
			if (q->weight == '$')
			{
				for (l = 0; l < temp.size(); l++)
				{
					if (q->number == temp[l])
					{
						break;
					}
				}
				if (l == temp.size())
				{
					temp.push_back(q->number);
					T[len++] = q->number;
					T[len] = -1;
				}
			}
			q = q->link;
		}
		i++;
	}
	temp.push_back(-1);
	for (int i = 0; i < temp.size(); i++) {
		_temp[i] = temp[i];
	}
	return _temp;
}

int* AdjTable::Move(int *T, char ch)
{
	int i = 0, j, k = 0, l;
	int *temp = new int[MAX];
	Vertex *p;
	Edge *q;
	while (T[i] != -1)
	{
		int pos = GetPosByValue(T[i]);
		p = startVertex;
		for (j = 0; j < pos; j++)
		{
			p = p->next;
		}
		q = p->out;
		while (q)
		{
			if (q->weight == ch)
			{
				for (l = 0; l < k; l++)
				{
					if (q->number == temp[l])
					{
						break;
					}
				}
				if (l == k)
				{
					temp[k] = q->number;
					k++;
				}
			}
			q = q->link;
		}
		i++;
	}
	temp[k] = -1;
	return temp;
}

void AdjTable::OutputNFA()
{
	Vertex *p = startVertex;
	Edge *q = new Edge();
	cout << "State   (weight)Arrive" << endl;
	for (int i = 0; i < numOfVertices; i++)
	{
		cout << p->number;
		cout << setw(8);
		q = p->out;
		//cout << setw(8);
		if (q) 
		{
			while (q) 
			{
				cout << "(" << q->weight << ")" << q->number << " ";
				q = q->link;
			}
		}
		else 
		{
			cout << setw(12) << "Final";
		}
		cout << endl;
		p = p->next;
	}
}