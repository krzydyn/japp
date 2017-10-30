#include <lang/System.hpp>
#include <io/File.hpp>

int main() {
	io::File f("non-existing.txt");
	if (f.exists()) System.out.println("err: exists");

	return 0;
}
