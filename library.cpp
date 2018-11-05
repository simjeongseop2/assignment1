#include "library.h"

library::library() {
	mem.resize(0); rsc.resize(0);
	ifstream source;
	source.open("resource.dat");
	string str;
	for(int i = 0; i < 2; i++) { source >> str; } //get dump
	while(1) {
		source >> str;
		if(source.eof())
			break;
		if(str == "Book") {
			source >> str;
			book* temp = new book(str);
			rsc.push_back(temp);
		}
	}
	source.close();
}
int library::CHARtoINT(const string& str) {
	return (str[0] - '0') * 10 + (str[1] - '0');
}
date library::make_date(const string& str) {
	date ret;
	ret.y = CHARtoINT(str.substr(0, 2));
	ret.m = CHARtoINT(str.substr(3, 2));
	ret.d = CHARtoINT(str.substr(6, 2));
	return ret;
}
string library::DATEtoSTRING(date& dat) {
	char y[3], m[3], d[3];
	y[0] = (dat.y / 10) + '0'; y[1] = (dat.y % 10) + '0'; y[2] = '\0';
	m[0] = (dat.m / 10) + '0'; m[1] = (dat.m % 10) + '0'; m[2] = '\0';
	d[0] = (dat.d / 10) + '0'; d[1] = (dat.d % 10) + '0'; d[2] = '\0';
	string y1(y), m1(m), d1(d);
	string slash = "/";
	string ret = y1 + slash + m1 + slash + d1;
	return ret;
}
date library::date_plus(date today, int p) {
	today.d += p;
	while(today.d > 30) {
		today.m++;
		today.d -= 30;
	}
	while(today.m > 12) {
		today.y++;
		today.m -= 12;
	}
	return today;
}
int library::date_diff(date dat1, date dat2) { //dat1 > dat2
	int n1 = dat1.y * 360 + dat1.m * 30 + dat1.d;
	int n2 = dat2.y * 360 + dat2.m * 30 + dat2.d;
	return n1 - n2;
}
void library::Borrow_rsc(ofstream& output,string rsc_type, string rsc_name,
						string mem_type, string mem_name, date dat, int cnt) {
	int idx = -1;
	for(int i = 0; i < rsc.size(); i++) {
		if(rsc[i]->type == rsc_type &&
			rsc[i]->name == rsc_name) {
			idx = i; break;
		}
	}
	if(idx == -1) {
		output << cnt << "\t1\tNon exist resource.\n"; 
		return;
	}
	member* m = NULL;
	for(int i = 0; i < mem.size(); i++)
		if(mem[i]->name == mem_name && mem[i]->type == mem_type) {
			m = mem[i]; break;
		}
	if(m == NULL) {
		if(mem_type == "Undergraduate")
			mem.push_back(m = new undergraduate(mem_name));
	}
	if(m->max_num == m->book_name.size()) {
		output << cnt << "\t2\tExceeds your possible number of borrow. Possible# of borrows: " << m->max_num << endl;
		return;
	}
	for(int i = 0; i < m->book_name.size(); i++)
		if(rsc_name == m->book_name[i]) {
			output << cnt << "\t4\tYou already borrowed this book at " << DATEtoSTRING(m->lent_date[i]);
			return;
		}
	if(rsc[idx]->isborrowed) {
		output << cnt << "\t5\tOther member already borrowed this book. This book will be returned at " << DATEtoSTRING(rsc[idx]->return_date) << endl;
		return;
	}
	if(!m->is_able(dat)) {
		output << cnt << "\t6\tRestricted member until " << DATEtoSTRING(m->disable) << endl;
		return;
	}
	m->book_name.push_back(rsc_name);
	m->lent_date.push_back(dat);
	rsc[idx]->isborrowed = true;
	rsc[idx]->user = mem_name;
	rsc[idx]->return_date = date_plus(dat, m->max_lent - 1);
	output << cnt << "\t0\tSuccess.\n";
	return;
}
void library::Return_rsc(ofstream& output, string rsc_type, string rsc_name,
			string mem_type, string mem_name, date dat, int cnt) {
	int idx = -1;
	for(int i = 0; i < rsc.size(); i++)
		if(rsc[i]->type == rsc_type &&
			rsc[i]->name == rsc_name) {
			idx = i; break;
		}
	if(idx == -1) {
		output << cnt << "\t1\tNon exist resource.\n"; 
		return;
	}
	if(rsc[idx]->user != mem_name) {
		output << cnt << "\t3\tYou did not borrow this book.\n";
		return;
	}
	member* m = NULL;
	for(int i = 0; i < mem.size(); i++)
		if(mem[i]->name == mem_name && mem[i]->type == mem_type) {
			m = mem[i]; break;
		}
	if(dat > rsc[idx]->return_date) {
		int diff = date_diff(dat, rsc[idx]->return_date);
		date disable = date_plus(dat, diff);
		if(disable > m->disable)
			m->disable = disable;
		output << cnt << "\t7\tDelayed return. You'll be resricted untile " << DATEtoSTRING(disable) << endl;
	}
	else
		output << cnt << "\t0\tSuccess.\n";
	int bidx = 0;
	for(int i = 0; i < m->book_name.size(); i++)
		if(m->book_name[i] == rsc_name) {
			bidx = i; break;
		}
	m->book_name.erase(m->book_name.begin() + bidx);
	m->lent_date.erase(m->lent_date.begin() + bidx);
	rsc[idx]->isborrowed = false;
	rsc[idx]->user = "NULL";
	rsc[idx]->return_date = dat;
}
						
void library::process() {
	ifstream input;
	input.open("input.dat");
	ofstream output; 
	output.open("output.dat");
	output << "Op_#\tReturn_code\tDescription\n";
	string str;
	for(int i = 0; i < 6; i++) { input >> str; } //get dump
	int cnt = 1;
	while(1) {
		input >> str;
		if(input.eof())
			break;
		date dat = make_date(str);
		string rsc_type, rsc_name, oper , mem_type, mem_name;
		input >> rsc_type >> rsc_name >> oper >> mem_type >> mem_name;
		if(oper == "B")
			Borrow_rsc(output, rsc_type, rsc_name, mem_type, mem_name, dat, cnt);
		else
			Return_rsc(output, rsc_type, rsc_name, mem_type, mem_name, dat, cnt);
		cnt++;
	}
	input.close(); output.close();
}