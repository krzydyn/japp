#include <lang/System.hpp>
#include <io/File.hpp>
#include <io/FileOutputStream.hpp>
#include <io/FileInputStream.hpp>
#include <io/OutputStreamWriter.hpp>
#include <io/Reader.hpp>

void test_nonexisting() {
	io::File f("non-existing.txt");
	if (f.exists()) System.out.println("err: exists");
	else System.out.println("ok: file not exits");
}
void test_write_read() {
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
	String testdata("test1 ĄąĆćĘę\n");
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
void test_filelist() {
	System.out.println("test_filelist");
	io::File f("/tmp");
	Array<String> a = f.list();
	for (int i=0; i < a.length; ++i)
		System.out.println(a[i]);
}

void test_writer() {
	System.out.println("test_writer");
	io::File f = io::File("/tmp/test.txt");
	io::FileOutputStream fos(f);
	io::OutputStreamWriter wr(fos);
	String testdata("test2 ĄąĆćĘę\n");
	System.out.printf("testdata.length = %d\n", testdata.length());
	wr.write(testdata);
}

int main() {
	test_nonexisting();
	test_write_read();
	test_filelist();
	test_writer();
	return 0;
}
