#include "utilities.h"


priority_queue<Node, vector<Node>, cmp> events, results;
map<records_pair, int> verify_history;
map<long, vector<records_pair>> inverted_list;
map<int, vector<long>> data_csv;


void write(const char* path,const vector<long> &data)
{
	ofstream ofs(path, ios::out | ios::app);
	if (!ofs)
	{
		cerr << "写文件错误！" << endl << path << endl;
		exit(-1);
	}
	for (auto e : data)
	{
		ofs << e << endl;
	}
	ofs.close();
}
void write2csv(const char* path, const map<int,vector<long>>& data)
{
	ofstream ofs(path, ios::out | ios::trunc);
	if (!ofs)
	{
		cerr << "写文件错误！" << endl << path << endl;
		exit(-1);
	}
	map<int, vector<long>>::const_iterator iter;
	int i = 0;
	for (iter = data.begin();iter!=data.end();iter++)
	{
		for (int j = 0; j < iter->second.size(); j++)
		{
			if (j % 10 == 0)
				ofs << endl<<endl;
			ofs << iter->second[j] << ","<<",";
			
		}
		ofs << endl;
	}
	ofs.close();
}
vector<vector<long>> read(const char* path)
{

	vector<vector<long>> records;
	long maxrecord = 0;
	long total_length = 0;
	map<long, long> elements;
	ifstream ifs(path, ios::in | ios::binary);
	if (!ifs)
	{
		cerr << "读取文件错误！"<<endl<< path  << endl;
		exit(-1);
	}
	else
	{
		long token;
		
		while (ifs.read((char*)&token, sizeof(long)))
		{
			long id = token;
			ifs.read((char*)&token, sizeof(long));
			long length = token;
			total_length += length;
			maxrecord = max(maxrecord, length);
			vector<long> record;
			for (long i = 0; i < length; i++)
			{
				ifs.read((char*)&token, sizeof(long));
				record.emplace_back(token);
				if (elements.find(token) == elements.end())
				{
					elements.insert(pair<long,long>(token,1));
				}
				else
				{
					elements[token] += 1;
				}
			}
			records.emplace_back(record);
		}
	}
	ifs.close();
	cout << "maxsize:	" << maxrecord << endl;
	cout << "number of elements:	" << elements.size() << endl;
	cout << "number of all tokens:	" << total_length << endl;
	cout << "number of all records:	" << records.size() << endl;
	cout << "average size:	"<< (double)total_length/records.size()<<endl;


	return records;
}

void init_events(const vector<vector<long>>& records)
{

	events = priority_queue<Node, vector<Node>, cmp>();
	
	long length = records.size();
	for (long i = 0; i < length; i++)
	{
		long r_s = records[i].size();
		Node n = Node(i, 0, -r_s);
		events.emplace(n);
	}

}
int verify(const vector<long>& x, long px, const vector<long>& y, long py)
{

	int overlap = 0;

	while (px < x.size() && py < y.size())
	{
		if (x[px] == y[py])
		{
			px++;
			py++;
			overlap++;
		}
		else if (x[px] < y[py])
		{
			px++;
		}
		else {
			py++;
		}
	}
	return overlap;
}
void init_results(const vector<vector<long>>& records, const unsigned int k)
{
	long length = records.size();
	srand((unsigned)time(NULL));

	results = priority_queue<Node, vector<Node>, cmp>();
	verify_history.clear();
	inverted_list.clear();

	while (results.size() < k)
	{
		long t1 = rand() % length;
		long t2 = rand() % length;
		if (t1 != t2)
		{
			long a = max(t1, t2);
			long b = min(t1, t2);
			records_pair tt(a, b);
			if (verify_history.find(tt) == verify_history.end())
			{
				verify_history.insert(t2i(tt, 0));
				int overlap = verify(records[a], 0, records[b], 0);
				results.emplace(Node(t1, t2, overlap));
			}
		}
	}
}

long similarity_upper_bound_access(long spx, long spy)
{
	return min(spx, spy);
}
long similarity_upper_bound_index(const vector<vector<long>>& records, long x, long px)
{
	return records[x].size() - px;
}
long similarity_upper_bound_probe(const vector<vector<long>>& records, long x, long px)
{
	return records[x].size() - px;
}




long findstart(const vector<vector<long>>& records, long t)
{
	for (size_t i = 0; i < records.size(); i++)
	{
		if (records[i].size() >= t)
		{
			return i;
		}
	}
	return 0;
}


bool inline equalprefix(vector<long> &x,vector<long>&y,long maxprefix)
{
	if (x.size() != y.size()||x.size()<maxprefix || y.size() < maxprefix)
	{
		return false;
	}

	for (size_t i = 0; i < maxprefix; i++)
	{
		if (x[i] != y[i])
		{
			return false;
		}
	}
	return true;
}

map<long, vector<long>> grouprecords(const vector<vector<long>>& records, long t)
{

	map<long, vector<long>> grecords;//key: group_id , value: records
	long start = findstart(records, t);
	grecords[0].push_back(start);
	vector<long> lastrec = records[start];
	int id = 0;
	map<long, int> history;
	for (size_t i = start + 1; i < records.size(); i++)
	{
		if (history.find(i) != history.end())
		{
			continue;
		}
		size_t length = records[i].size();
		vector<long> currec = records[i];

		if (equalprefix(lastrec, currec, length-t+1))
		{
			grecords[id].push_back(i);
			lastrec = currec;
		}
		else
		{
			int increment = 0;
			while (i + increment < records.size() && length == records[i + increment].size())
			{
				currec = records[i+increment];
				if (equalprefix(lastrec, currec, length - t + 1)) {
					grecords[id].push_back(i + increment);
					history[i + increment] = 1;
				}
				increment++;
			}
			id++;
			grecords[id].push_back(i);
			lastrec = records[i];
		}
	}
	return grecords;
}