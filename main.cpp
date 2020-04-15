#include "algorithm.h"


int main()
{
	const char* input_path = "E:\\rochover\\dataset\\set similarity\\binary\\dblp.txt.bin";
	cout << "读文件开始..." << endl;
	clock_t start, end; 
	start = clock();
	vector<vector<long>> records = read(input_path);
	end = clock();
	cout << "读取文件结束，耗时：	";
	cout << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;
	cout << "---------------------------------------------------" << endl;
	extern map<int, vector<long>> data_csv;
	int k = 500;
	long t = 1189;
		//allpairs(records,t);
		//ppjoin(records, t);
		//groupjoin(records, t);

	for (int i = 2; i <= 10;i++) {
		for (size_t j = 1; j <= 10; j++)
		{
			k = j*500;
			adaptive_topk(records, k,i);
		}
		


		//topk(records, k);
		cout<<"************k="<<k<<"****************"<<endl;
	


	}
	const char* path = "E:\\rochover\\dataset\\set similarity\\binary\\data.csv";

	write2csv(path,data_csv);
	
	return 0;
}  