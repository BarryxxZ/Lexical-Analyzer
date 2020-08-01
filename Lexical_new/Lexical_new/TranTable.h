#pragma once
#ifndef TRANTABLE_H_
#define TRANTABLE_H_

#include"base.h"

class TranTable
{
private:
	int **matrix;
	int rowNumber;
	int colNumber;
public:
	TranTable(int rowNum, int colNum);
	~TranTable();

	void SetValue(int i, int j, int value);

	int GetValue(int i, int j);

	void Clear();
};

#endif
