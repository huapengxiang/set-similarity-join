#include "utilities.h"



void adaptive_topk(const vector<vector<long>> &records,int k,int step=1 )
{
	long v_time = 0;
	auto v_s = system_clock::now();
	auto v_e = system_clock::now();
	auto start = system_clock::now();

	extern priority_queue<Node, vector<Node>, cmp> events, results;
	extern map<records_pair, int> verify_history;
	extern map<long, vector<records_pair>> inverted_list;

	init_events(records);
	init_results(records, k);

	//int step = 8;

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
		/**
		if (step < tub - sk)
		{
			step += 1;
		}
		else {
			step = tub - sk;
		}
		/**/
		for (int s = 0; s < step; s++)
		{
			long spx = similarity_upper_bound_probe(records,x,px+s);
			if (spx <= sk)
			{
				break;
			}
			long w = records[x][px + s];
			map<long, vector<records_pair>>::iterator iter;
			iter = inverted_list.find(w);
			if (iter != inverted_list.end())
			{
				vector<records_pair> list = iter->second;
				long length = list.size();
				for (long i=0; i< length; i++)
				{
					long y = list[i].first;
					long py = list[i].second;
					if (sk <= records[y].size())
					{
						long a = max(x, y);
						long b = min(x,y);
						records_pair t(a,b);
						if (verify_history.find(t) == verify_history.end())
						{	
							candidate++;
							verify_history.insert(t2i(t,0));
							v_s = system_clock::now();
							long sim = verify(records[x],px+s,records[y],py);
							v_e = system_clock::now();
							v_time += long(duration_cast<microseconds>(v_e - v_s).count());
							if (sim > sk)
							{
								results.push(Node(a,b,sim));
								results.pop();
							}
							sk = results.top().tub;
						}
					}
				}
				list.emplace_back(records_pair(x,px));
				inverted_list[w] = list;
			}
			else {
				vector<records_pair> list;
				list.emplace_back(records_pair(x,px));
				inverted_list.insert(k2v(w,list));
			}
		}
		long spx = similarity_upper_bound_probe(records, x, px + step);
		if (spx > sk)
		{
			events.push(Node(x,px+step,-spx));
		}
	}
	auto end = system_clock::now();
	auto elapsed = duration_cast<microseconds>(end - start);
	long elapsed_time = long(elapsed.count());
	cout << "elapsed£º";
	cout << elapsed_time << "Î¢Ãë" << endl;
	cout << "verify time£º";
	cout << v_time << "Î¢Ãë" << endl;

	long inverted_list_size = 0;
	for (auto e :inverted_list)
	{
		long length = e.second.size();
		inverted_list_size += length;
	}
	cout << "atopk" << endl;
	cout << "events size:	" ;
	cout << events.size() << endl;
	cout << "inverted list size:	";
	cout << inverted_list_size << endl;
	cout << "candidate size:	";
	cout << candidate<< endl;
	cout << "min of topk:	";
	long x = results.top().record_id;
	long y = results.top().pos;
	long tub = results.top().tub;
	cout << tub << endl;
	cout << "Jaccard similarity:	" << (double)(tub)/(records[x].size()+records[y].size()-tub) << endl;

	extern map<int, vector<long>> data_csv;
	data_csv[0].push_back(elapsed_time);
	data_csv[1].push_back(v_time);
	data_csv[2].push_back((long)events.size());
	data_csv[3].push_back(inverted_list_size);
	data_csv[4].push_back(candidate);
	data_csv[5].push_back(tub);

	//vector<long> data = { long(elapsed.count()) ,v_time,(long)events.size(),inverted_list_size,candidate,tub };
	//const char* path = "E:\\rochover\\dataset\\set similarity\\binary\\data.txt";
	//write(path,data);

	/*while (!results.empty())
	{
		long x = results.top().record_id;
		long y = results.top().pos;
		long tub = results.top().tub;
		cout << x << "  " << y << " " << (double)(tub) / (records[x].size() + records[y].size() - tub) << endl;
		results.pop();
	}*/
}

