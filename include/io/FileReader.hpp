#ifndef IO_FILEREADER_HPP
#define IO_FILEREADER_HPP

#include <io/FileInputStream.hpp>
#include <io/InputStreamReader.hpp>

namespace io {

class FileReader : extends InputStreamReader {
	Shared<FileInputStream> fis;
public:
	FileReader(String fileName) : InputStreamReader(*new FileInputStream(fileName)) {
		fis = Shared<FileInputStream>((FileInputStream*)lock);
	}
	//FileReader(File file) : InputStreamReader(FileInputStream(file)) {}
	~FileReader() {
		System::out.println("Delete " + fis->toString());
	}
};

} //namespace io

#endif
