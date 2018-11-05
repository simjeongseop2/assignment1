#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "date.h"
#include "member.h"
#include "resource.h"
#include "library.h"
#define MAX_BOOK_LEN 100
using namespace std;

int main(void) {
	library lib;
	lib.process();
	return 0;
}