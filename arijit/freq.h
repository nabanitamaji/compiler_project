#ifndef FREQ_HH
#define FREQ_HH

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
using namespace std;

namespace arijit {
	class Freq {
		public:
			map<string,int> trace;

		private:

			vector<string> tokenize(string inputStr);
			void readFile(const char *fileName);

		public:
			int getFreq(const char *, const char *);

	};

}

#endif
