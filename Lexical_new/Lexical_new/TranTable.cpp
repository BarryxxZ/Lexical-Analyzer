#include"TranTable.h"

TranTable::TranTable(int rowNum, int colNum)
{
	rowNumber = rowNum;
	colNumber = colNum;
	matrix = (int**)(new int*[rowNumber]);
	for (int i = 0; i < rowNumber; i++)
	{
		matrix[i] = new int[colNumber];
	}
}

TranTable::~TranTable()
{
	Clear();
}

void TranTable::SetValue(int i, int j, int value)
{
	matrix[i][j] = value;
}

int TranTable::GetValue(int i, int j)
{
	return matrix[i][j];
}

void TranTable::Clear()
{
	for (int i = 0; i < rowNumber; i++)
	{
		delete[] matrix[i];
	}
	delete matrix;
}