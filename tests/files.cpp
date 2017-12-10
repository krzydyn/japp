#include <lang/System.hpp>
#include <io/File.hpp>
#include <io/FileOutputStream.hpp>
#include <io/FileInputStream.hpp>
#include <io/OutputStreamWriter.hpp>

static void test_nonexisting() {
	io::File f("non-existing.txt");
	if (f.exists()) System.out.println("err: exists");
	else System.out.println("ok: file not exits");
}
static void test_write_read() {
	io::File f = io::File("/tmp/test.txt");
	f.unlink();
	try {
		io::FileInputStream fis(f);
		int r =fis.read();
		System.out.printf("read %d\n", r);
	}
	catch (const io::IOException& e) {
		e.printStackTrace();
	}

	io::FileOutputStream fos(f);
	String testdata("test ĄąĆćĘę\n");
	fos.write(testdata.getBytes());
	fos.close();

	io::FileInputStream fis(f);
	char buf[100];
	jint l = fis.read(buf,sizeof(buf));
	if (l != testdata.length()) System.out.printf("err: %d != %d\n", l, testdata.length());
	buf[l]=0;
	if (!testdata.equals(String::valueOf(buf))) System.out.println("err: !equals");

	//if (!f.unlink()) System.out.println("err: can't delete file");
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
