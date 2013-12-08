#include "freq.h"

namespace freq {
	void Freq::readFile(const char *fileName) {
		ifstream feedBack;
		feedBack.open(fileName);
		if(feedBack.is_open()) {
			string line;
			while(getline(feedBack,line)) {
				vector<string> res = tokenize(line);			
				if (res.size()== 3) {
					stringstream ss(res[2]);
					int count = -1;
					ss>>count;
					trace.insert(pair<string,int>(res[0]+res[1],count));

				} else {
				}
			}       
			feedBack.close();
		}
	}

	vector<string> Freq::tokenize(string inputStr) {
		stringstream ss(inputStr);
		string line;
		vector<string> op;
		while(ss.good()){
			getline(ss,line,' ');
			op.push_back(line);
		}
		return op;
	}

	int Freq::getFreq(const char *caller,const  char *callee) {
		if(trace.size() == 0) {
			readFile("tracedump.log");
		}
		string callerName(caller);
		string calleeName(callee);
		map<string,int>::iterator itr = trace.find(callerName+calleeName);
		if(itr != trace.end()) {
			return itr->second;
		}
		return 0;
	}

}
