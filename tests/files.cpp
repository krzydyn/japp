#include <lang/System.hpp>
#include <io/File.hpp>
#include <io/FileOutputStream.hpp>
#include <io/FileInputStream.hpp>
#include <io/Writer.hpp>

static void test_nonexisting() {
	io::File f("non-existing.txt");
	if (f.exists()) System.out.println("err: exists");
	else System.out.println("ok: file not exits");
}
static void test_write_read() {
	io::File f = io::File("/tmp/test.txt");
	try {
		io::FileInputStream fis(f);
		int r =fis.read();
		System.out.printf("read %d\n", r);
	}
	catch (const io::IOException& e) {
		e.printStackTrace();
	}

	io::FileOutputStream fos(f);
	fos.write("test\n");
	fos.close();

	io::FileInputStream fis(f);
	char buf[100];
	int l = fis.read(buf,sizeof(buf));
	if (l != 5) System.out.println("err: l != 5 " + String::valueOf(l));
	if (memcmp(buf,"test\n",5) != 0) System.out.println("err: !test");

	if (!f.unlink()) System.out.println("err: can't delete file");
}
static void test_list() {
	io::File f("/");
	Array<String> a = f.list();
	for (int i=0; i < a.length; ++i)
		System.out.println(a[i]);
}

int main() {
	test_nonexisting();
	test_write_read();
	test_list();
	return 0;
}
