#include "utilities.h"



void topk(const vector<vector<long>>& records, int k)
{
	
	extern priority_queue<Node, vector<Node>, cmp> events, results;
	extern map<records_pair, int> verify_history;
	extern map<long, vector<records_pair>> inverted_list;


	long v_time = 0;
	auto v_s  =  system_clock::now();
	auto v_e = system_clock::now();
	auto start = system_clock::now();
	init_events(records);
	init_results(records, k);

	bool flag = false;
	long candidate = 0;
	while (!events.empty())
	{
		Node n = events.top();
		events.pop();

		long x = n.record_id;
		long px = n.pos;
		long tub = -1 * n.tub;

		long sk = results.top().tub;

		if (tub <= sk)
		{
			break;
		}

		long w = records[x][px];
		map<long, vector<records_pair>>::iterator iter;
		iter = inverted_list.find(w);
		if (iter != inverted_list.end())
		{
			vector<records_pair> list = iter->second;
			long length = list.size();
			for (long i = 0; i < length; i++)
			{
				long y = list[i].first;
				long py = list[i].second;
				long ax = records[x].size() - px;
				long ay = records[y].size() - py;
				if (similarity_upper_bound_access(ax,ay) < sk)
				{
					vector<records_pair> list_update;
					for (int j = 0; j < i; j++)
					{
						list_update.emplace_back(list[j]);
					}
					inverted_list[w] = list_update;
					break;
				}
				if (sk <= records[y].size())
				{
					long a = max(x, y);
					long b = min(x, y);
					records_pair t(a, b);
					if (verify_history.find(t) == verify_history.end())
					{
						verify_history.insert(t2i(t, 0));
						v_s = system_clock::now();
						long sim = verify(records[x], px, records[y], py);
						v_e = system_clock::now();
						v_time  += long(duration_cast<microseconds>(v_e - v_s).count());
						candidate++;
						if (sim > sk)
						{
							results.push(Node(a, b, sim));
							results.pop();
						}
						sk = results.top().tub;
					}
				}
			}
			if (flag == false)
			{
				long si = similarity_upper_bound_index(records,x,px);
				if (si >= sk)
				{
					vector<records_pair> list = iter->second;
					list.emplace_back(records_pair(x,px));
					inverted_list[w] = list;
				}
				else
				{
					flag = true;
				}
			}
		}
		else
		{
			vector<records_pair> list;
			list.emplace_back(records_pair(x, px));
			inverted_list.insert(k2v(w, list));
		}
		long spx = similarity_upper_bound_probe(records,x,px+1);
		if (spx > sk)
		{
			events.push(Node(x,px+1,-spx));

		}
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
	cout << "topk" << endl;
	cout << "events size:	";
	cout << events.size() << endl;
	cout << "inverted list size:	";
	cout << inverted_list_size << endl;
	cout << "candidate size:	";
	cout << candidate << endl;
	cout << "min of topk:	";
	long x = results.top().record_id;
	long y = results.top().pos;
	long tub = results.top().tub;
	cout << tub << endl;
	cout << "Jaccard similarity:	" << (double)(tub) / (records[x].size() + records[y].size() - tub) << endl;

	vector<long> data = { long(elapsed.count()) ,v_time,(long)events.size(),inverted_list_size,candidate,tub };
	const char* path = "E:\\rochover\\dataset\\set similarity\\binary\\data.txt";
	write(path, data);

	/*while (!results.empty())
	{
		long x = results.top().record_id;
		long y = results.top().pos;
		long tub = results.top().tub;
		cout << x << "  " << y << " " << (double)(tub) / (records[x].size() + records[y].size() - tub) << endl;
		results.pop();
	}*/

}