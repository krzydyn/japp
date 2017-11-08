#include <lang/System.hpp>
#include <io/File.hpp>
#include <io/FileOutputStream.hpp>

int main() {
	io::File f("non-existing.txt");
	if (f.exists()) System.out.println("err: exists");
	f = io::File("/tmp/test.txt");
	io::FileOutputStream fos(f);
	fos.write("test");
	fos.close();
	return 0;
}
