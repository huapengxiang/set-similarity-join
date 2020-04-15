#pragma once


#include "utilities.h"

void adaptive_topk(const vector<vector<long>>& records, int k,int step);
void topk(const vector<vector<long>>& records, int k);
void allpairs(const vector<vector<long>>& records, long t);
void ppjoin(const vector<vector<long>>& records, long t);
void groupjoin(const vector<vector<long>>& records, long t);