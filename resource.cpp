#include "resource.h"

resource::resource() { isborrowed = false; }
resource::resource(string type, string name) : type(type), name(name) {
	isborrowed = false;
}
book::book(string name) : resource("Book", name) {}