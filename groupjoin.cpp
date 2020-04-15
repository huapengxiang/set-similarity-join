#include "utilities.h"

void groupjoin(const vector<vector<long>>& records, long t)
{
	long v_time = 0;
	auto v_s = system_clock::now();
	auto v_e = system_clock::now();
	auto start = system_clock::now();
	extern map<records_pair, int> verify_history;
	extern map<long, vector<records_pair>> inverted_list;
	long candidate_size = 0;
	priority_queue<OneResult, vector<OneResult>, cmpOneResult>  result;

	map<long,vector<long>> g_records = grouprecords( records,  t);
	map<long, int> g_verify_history;

	for (long x = 0; x < g_records.size(); x++)
	{
		vector<vector<long>> candidates;
		long x_size = records[g_records[x][0]].size();
		long pp = x_size - t + 1;
		for (long px = 0; px < pp; px++)
		{
			long w = records[g_records[x][0]][px];
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
					if (records[g_records[y][0]].size() >= t)
					{
						long upbound = min(records[g_records[x][0]].size() - px, records[g_records[x][0]].size() - py);//Î»ÖÃ¹ýÂË
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

		v_s = system_clock::now();
		for (auto xy : candidates)
		{
			vector<long> x_list = g_records[xy[0]];
			vector<long> y_list = g_records[xy[2]];
			for (auto i : x_list)
			{
				for (auto j: y_list)
				{
					int overlap = verify(records[i], xy[1], records[j], xy[3]);
					candidate_size++;
					if (overlap >= t)
					{
						result.push(OneResult(i, j, overlap));
					}
				}
			}
			if (x_list.size() > 1 && g_verify_history.find(xy[0])==g_verify_history.end())
			{
				g_verify_history[xy[0]] = 1;
				for (auto i : x_list)
				{
					for (auto j : x_list)
					{
						if (i > j)
						{
							int overlap = verify(records[i], 0, records[j], 0);
							candidate_size++;
							if (overlap >= t)
							{
								result.push(OneResult(i, j, overlap));
							}
						}
					}
				}
			}
			if (y_list.size() > 1 && g_verify_history.find(xy[2]) == g_verify_history.end())
			{
				g_verify_history[xy[2]] = 1;
				for (auto i : y_list)
				{
					for (auto j : y_list)
					{
						if (i > j)
						{
							int overlap = verify(records[i], 0, records[j], 0);
							candidate_size++;
							if (overlap >= t)
							{
								result.push(OneResult(i, j, overlap));
							}
						}
					}
				}
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

	cout << "groupjoin" << endl;
	cout << "inverted list size:	";
	cout << inverted_list_size << endl;
	cout << "candidate size:	";
	cout << candidate_size << endl;
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