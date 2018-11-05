#ifndef __DATE_H__
#define __DATE_H__
class date {
public:
	int y, m, d;
	date() {}
	date(int y, int m, int d) : y(y), m(m), d(d) {} 
	bool operator>(const date& ref) {
		if(y > ref.y)
			return true;
		else if(y == ref.y && m > ref.m)
			return true;
		else if(y == ref.y && m == ref.m && d > ref.d)
			return true;
		return false;
	}
};
#endif
