#include <lang/System.hpp>
#include <io/File.hpp>
#include <io/FileOutputStream.hpp>
#include <io/FileInputStream.hpp>

int main() {
	io::File f("non-existing.txt");
	if (f.exists()) System.out.println("err: exists");
	f = io::File("/tmp/test.txt");
	io::FileOutputStream fos(f);
	fos.write("test\n");
	fos.close();

	io::FileInputStream fis(f);
	char buf[100];
	int l = fis.read(buf,sizeof(buf));
	if (l != 5) System.out.println("err: l != 5 " + String::valueOf(l));

	if (!f.unlink()) System.out.println("err: can't delete file");
	return 0;
}
