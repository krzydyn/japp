#ifndef __IO_FILE_HPP
#define __IO_FILE_HPP

#include <lang/Exception.hpp>
//#include <iostream>

namespace io {

class File;
class FileSystem : public Object {
private:
	static boolean useCanonCaches;
	static boolean useCanonPrefixCache;
	static boolean getBooleanProperty(const String& prop, boolean defaultVal);

public:
	static const FileSystem& getFileSystem();

	virtual char getSeparator() const = 0;
	virtual char getPathSeparator() const = 0;
	virtual String normalize(const String& path) const = 0;
	virtual int prefixLength(const String& path) const = 0;
	virtual String resolve(const String& parent, const String& child) const = 0;
	virtual String getDefaultParent() const = 0;
	virtual String fromURIPath(const String& path) const = 0;
	virtual boolean isAbsolute(const File& f) const;
	virtual String resolve(const File& f) const = 0;
	virtual String canonicalize(const String& path) const = 0;

	enum {
		BA_EXISTS    = 0x01,
		BA_REGULAR   = 0x02,
		BA_DIRECTORY = 0x04,
		BA_HIDDEN    = 0x08
	};
	virtual int getBooleanAttributes(const File& f) const = 0;

	enum {
		ACCESS_READ    = 0x04,
		ACCESS_WRITE   = 0x02,
		ACCESS_EXECUTE = 0x01
	};
	virtual boolean checkAccess(const File& f, int access) const = 0;
	virtual boolean setPermission(const File& f, int access, boolean enable, boolean owneronly) const = 0;
	virtual long getLastModifiedTime(const File& f) const = 0;
	virtual long getLength(const File& f) const = 0;
	virtual boolean createFileExclusively(const String& pathname) const = 0;
	virtual boolean unlink(File f) const = 0; //delete=>unlink (delete is C++ is keyword)
	virtual Array<String> list(const File& f) const = 0;
	virtual boolean rename(const File& f1, const File& f2) const = 0;
	virtual boolean setLastModifiedTime(const File& f, long time) const = 0;
	virtual boolean setReadOnly(const File& f) const = 0;
	virtual Array<File> listRoots() const = 0;

	enum {
		SPACE_TOTAL  = 0,
		SPACE_FREE   = 1,
		SPACE_USABLE = 2
	};
	virtual long getSpace(const File& f, int t) const = 0;
	virtual int compare(const File& f1, const File& f2) const = 0;
	virtual int hashCode(const File& f) const = 0;
};

class File : public Object {
	friend class FileSystem;
public:
	//public static final char separatorChar = fs.getSeparator();
	static const char separatorChar = '/';
	static const String separator;
	static const char pathSeparatorChar = ':';
	static const String pathSeparator;
private:
	static const FileSystem& fs;

	String path;
	int prefixLength;

	File(const String& path, int prefixLength) {
		this->path = path;
		this->prefixLength = prefixLength;
	}
	File(const String& child, const File& parent) {
		this->path = fs.resolve(parent.path, child);
		this->prefixLength = parent.prefixLength;
	}
	int getPrefixLength() const {return prefixLength;}
public:
	File(const String& path) {
		if (&path == null) throw NullPointerException();
		this->path = fs.normalize(path);
		this->prefixLength = fs.prefixLength(this->path);
	}
	File(const String& parent, const String& child) {
		if (&child == null) throw NullPointerException();
		if (&parent != null) {
			if (parent.equals("")) {
				this->path = fs.resolve(fs.getDefaultParent(),
				fs.normalize(child));
			} else {
				this->path = fs.resolve(fs.normalize(parent), fs.normalize(child));
			}
		} else {
			this->path = fs.normalize(child);
		}
		this->prefixLength = fs.prefixLength(this->path);
	}
	File(const File& parent, const String& child) {
		if (&child == null) throw NullPointerException();
		if (&parent != null) {
			if (parent.path.equals("")) {
				this->path = fs.resolve(fs.getDefaultParent(),
				fs.normalize(child));
			} else {
				this->path = fs.resolve(parent.path, fs.normalize(child));
			}
		} else {
			this->path = fs.normalize(child);
		}
		this->prefixLength = fs.prefixLength(this->path);
	}
	//File(cosnt URI& uri) {}
	String getName() const {
		int index = path.lastIndexOf(separatorChar);
		if (index < prefixLength) return path.substring(prefixLength);
		return path.substring(index + 1);
	}
	String getParent() const {
		int index = path.lastIndexOf(separatorChar);
		if (index < prefixLength) {
			if ((prefixLength > 0) && (path.length() > prefixLength))
				return path.substring(0, prefixLength);
			return ""; //originaly null
		}
		return path.substring(0, index);
	}
	File getParentFile() const {
		String p = this->getParent();
		//if (p == null) return null;
		return File(p, this->prefixLength);
	}
	const String& getPath() const { return path; }
	boolean isAbsolute() const { return fs.isAbsolute(*this); }
	String getAbsolutePath() const { return fs.resolve(*this); }
	File getAbsoluteFile() const {
		String absPath = getAbsolutePath();
		return File(absPath, fs.prefixLength(absPath));
	}
};

} //namespace io

#endif
