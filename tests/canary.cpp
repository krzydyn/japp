#include <lang/String.hpp>
#include <lang/Class.hpp>
#include <string.h>

int main(int argc, const char *argv[]) {TRACE;
	Object o;
	printf("%s\n", o.getClass().getName().cstr());
}
