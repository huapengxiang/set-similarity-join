#include "utilities.h"

void ppjoin(const vector<vector<long>>& records, long t)
{
	long v_time = 0;
	auto v_s = system_clock::now();
	auto v_e = system_clock::now();
	auto start = system_clock::now();
	extern map<records_pair, int> verify_history;
	extern map<long, vector<records_pair>> inverted_list;
	long candidate_size = 0;
	priority_queue<OneResult, vector<OneResult>, cmpOneResult>  result;
	for (long x = 0; x < records.size(); x++)
	{
		vector<vector<long>> candidates;
		long x_size = records[x].size();
		long pp = x_size - t + 1;
		for (long px = 0; px < pp; px++)
		{
			long w = records[x][px];
			map<long, vector<records_pair>>::iterator iter;
			iter = inverted_list.find(w);
			if (iter != inverted_list.end())
			{
				vector<records_pair> list = iter->second;
				long length = list.size();
				for (long i = 0; i < length; i++)
				{
					records_pair record = list[i];
					long y = record.first;
					long py = record.second;
					if (records[y].size() >= t )
					{
						long upbound = min(records[x].size()-px,records[y].size()-py);//Î»ÖÃ¹ýÂË
						if (upbound >= t)
						{
							long a = max(x, y);
							long b = min(x, y);
							records_pair ab(a, b);
							if (verify_history.find(ab) == verify_history.end())
							{
								verify_history.insert(t2i(ab, 0));
								vector<long> c_pair = { x,px,y,py };
								candidates.emplace_back(c_pair);
							}
						}
						
					}
				}
				long pi = x_size - t + 1;
				if (px < pi)
				{
					vector<records_pair> list = inverted_list[w];
					list.emplace_back(records_pair(x, px));
					inverted_list[w] = list;
				}
			}
			else
			{
				vector<records_pair> list;
				list.emplace_back(records_pair(x, px));
				inverted_list.insert(k2v(w, list));
			}
		}
		candidate_size += candidates.size();
		v_s = system_clock::now();
		for (auto xy : candidates)
		{
			int overlap = verify(records[xy[0]], xy[1], records[xy[2]], xy[3]);
			//double sim = (double)(overlap) / (records[xy[0]].size() + records[xy[2]].size() - overlap);
			if (overlap >= t)
			{
				result.push(OneResult(xy[0], xy[2], overlap));
			}
		}
		v_e = system_clock::now();
		v_time += long(duration_cast<microseconds>(v_e - v_s).count());

	}
	auto end = system_clock::now();
	auto elapsed = duration_cast<microseconds>(end - start);
	cout << "elapsed£º";
	cout << double(elapsed.count()) << "Î¢Ãë" << endl;
	cout << "verify time£º";
	cout << v_time << "Î¢Ãë" << endl;

	long inverted_list_size = 0;
	for (auto e : inverted_list)
	{
		long length = e.second.size();
		inverted_list_size += length;
	}

	cout << "ppjoin" << endl;
	cout << "inverted list size:	";
	cout << inverted_list_size << endl;
	cout << "candidate size:	";
	cout << candidate_size << endl;
	cout << "Hashmap size:	";
	cout << verify_history.size() << endl;
	cout << "result size:	";
	cout << result.size() << endl;
	cout << "min of results	";
	long x = result.top().x;
	long y = result.top().y;
	long sim = result.top().sim;
	cout << x << "	" << y << "	" << sim << endl;
	cout << "Overlap similarity:	" << verify(records[x], 0, records[y], 0) << endl;
	/*while (!result.empty())
{
long x = result.top().x;
long y = result.top().y;
double tub = result.top().sim;
cout << x << "  " << y << " " << sim << endl;
result.pop();
}*/
}