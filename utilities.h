#pragma once

#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <map>
#include <ctime>
#include <tuple>
#include <chrono>   

using namespace std;
using namespace chrono;

struct Node
{
	long record_id;
	long pos;
	long tub;
	Node(long a = 0, long b = 0, long c = 0) : record_id(a), pos(b), tub(c) {}
};

struct cmp
{
	bool operator()(Node a, Node b) {
		return a.tub > b.tub;
	}
};
struct OneResult
{
	long x;
	long y;
	double sim;
	OneResult(long a = 0, long b = 0, double c = 0) :x(a), y(b), sim(c) {}
};
struct cmpOneResult
{
	bool operator()(OneResult a, OneResult b) {
		return a.sim > b.sim;
	}
};

struct records_pair
{
	long first;
	long second;
	records_pair(long a, long b) :first(a), second(b) {}
	friend bool operator<(const struct records_pair& a, const struct records_pair& b) {
		if (a.first < b.first ||
			(a.first == b.first && a.second < b.second)) {
			return true;
		}
		return false;
	}
};

typedef pair<records_pair, int> t2i;
typedef pair<long, vector<records_pair>> k2v;


void write2csv(const char* path, const map<int, vector<long>>& data);
void write(const char* path, const vector<long>& data);
vector<vector<long>> read(const char* path);


void init_events(const vector<vector<long>>& records);

int verify(const vector<long>& x, long px, const vector<long>& y, long py);


void init_results(const vector<vector<long>>& records, const unsigned int k);

long similarity_upper_bound_access(long spx,long spy);
long similarity_upper_bound_index(const vector<vector<long>>& records,long x ,long px );
long similarity_upper_bound_probe(const vector<vector<long>>& records, long x, long px);
map<long, vector<long>> grouprecords(const vector<vector<long>>& records, long t);