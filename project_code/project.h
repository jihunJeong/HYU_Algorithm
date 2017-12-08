#ifndef _PROJECT_H__
#define _PROJECT_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

typedef vector<vector<int> > matrix;

extern matrix arr;

int parseLine(char *line);
int getValue();

string File_Name(string str);
matrix Make_Matrix(int x_size, int y_size);
int Max(int y, int x);
void Local_Align(char *d, char *q, const char *o);

#endif