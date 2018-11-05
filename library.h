#ifndef __LIBRARY_H__
#define __LIBRARY_H__
#include <fstream>
#include <vector>
#include <string>
#include "date.h"
#include "member.h"
#include "resource.h"
using namespace std;

class library {
public:
	vector<member*> mem;
	vector<resource*> rsc;
	library();
	int CHARtoINT(const string& str);
	date make_date(const string& str);
	string DATEtoSTRING(date& dat);
	date date_plus(date today, int p);
	int date_diff(date dat1, date dat2);
	void Borrow_rsc(ofstream& output,string rsc_type, string rsc_name, string mem_type, string mem_name, date dat, int cnt);
	void Return_rsc(ofstream& output, string rsc_type, string rsc_name, string mem_type, string mem_name, date dat, int cnt);
	void process();
};
#endif