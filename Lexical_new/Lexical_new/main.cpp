#include"base.h"
#include"AdjTable.h"
#include"DFA.h"
#include"LinkedStack.h"
#include"TranTable.h"

int main()
{
	DFA test;
	test.GetRegExp();
	test.processRegExp();
	test.toSuffix();
	test.GetEdgeNumber();
	test.ThompsonConstruction();
	test.SubsetConstruction();
	test.check();
	system("pause");
	return 0;
}
/*
	finished at 2020/5/5
*/