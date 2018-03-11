#include <lang/System.hpp>
#include <io/File.hpp>
#include <io/FileOutputStream.hpp>
#include <io/FileInputStream.hpp>
#include <io/OutputStreamWriter.hpp>
#include <io/FileReader.hpp>

class TestTrace {
private:
public:
	TestTrace(const char *fn) {
		LOGD(">>>> %s", fn);
	}
	~TestTrace() {
		LOGD("===============");
	}
};
#define TESTTRACE TestTrace __trace(__FUNCTION__)

void test_nonexisting() { TESTTRACE;
	io::File f("non-existing.txt");
	if (f.exists()) System.out.println("err: exists");
	else System.out.println("ok: file not exits");
}
void test_write_read() { TESTTRACE;
	String testdata("test1 ĄąĆćĘę");
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

	try {
		io::FileOutputStream fos(f);
		fos.write(testdata.getBytes());
		fos.close();
	}
	catch (const io::IOException& e) {
		e.printStackTrace();
	}

	io::FileInputStream fis(f);
	char buf[100];
	int l = fis.read(buf,sizeof(buf));
	if (l != testdata.length()) System.out.printf("err: %d != %d\n", l, testdata.length());
	buf[l]=0;
	if (!testdata.equals(String::valueOf(buf))) System.out.println("err: !equals");

	if (!f.unlink()) System.out.println("err: can't delete file");
}
void test_filelist() { TESTTRACE;
	io::File f("/tmp");
	Array<String> a = f.list();
	for (String& s : a) {
		System.out.println("\t"+s);
	}
	System.out.printf("Found %d files in %s\n", a.length, f.getPath().cstr());
}

void test_writer() { TESTTRACE;
	String testdata("test2 ĄąĆćĘę");
	io::File f = io::File("/tmp/test.txt");
	io::FileOutputStream fos(f);
	io::OutputStreamWriter wr(fos);
	System.out.printf("testdata.length = %d\n", testdata.length());
	wr.write(testdata);
	if (!f.unlink()) System.out.println("err: can't delete file");
}

void test_reader() { TESTTRACE;
	String testdata("test3 ĄąĆćĘę");
	io::File f = io::File("/tmp/test.txt");
	try {
		io::FileInputStream fis(f);
	}
	catch (const io::IOException& e) {
		e.printStackTrace();
	}
	io::FileOutputStream fos(f);
	fos.write(testdata.getBytes());
	fos.close();

	io::FileInputStream fis(f);
	io::InputStreamReader isr(fis);
	Array<char> buf(100);
	int r = isr.read(buf);
	buf[r]=0;
	System.out.printf("isr.read[%d]: '%s'\n", r, &buf[0]);

	System.out.println("FileReader...");
	io::FileReader fr("/tmp/test.txt");
}

int main() {
	test_nonexisting();
	test_write_read();
	test_filelist();
	test_writer();
	test_reader();
	return 0;
}
