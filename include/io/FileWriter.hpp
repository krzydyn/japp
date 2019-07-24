#ifndef IO_FILEWRITER_HPP
#define IO_FILEWRITER_HPP

#include <io/FileOutputStream.hpp>
#include <io/OutputStreamWriter.hpp>

namespace io {

class FileWriter : extends OutputStreamWriter {
	Shared<FileOutputStream> fos;
public:
	FileWriter(String fileName) : OutputStreamWriter(*new FileOutputStream(fileName)) {
		fos = Shared<FileOutputStream>((FileOutputStream*)lock);
	}
	//FileWriter(File file) : OutputStreamWriter(FileOutputStream(file)) {}
	~FileWriter() {
		System::out.println("Delete " + fos->toString());
	}
};

} //namespace io

#endif

