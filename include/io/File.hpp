#ifndef __IO_FILE_HPP
#define __IO_FILE_HPP

#include <io/IOException.hpp>
#include <util/ArrayList.hpp>

namespace io {

class File;
class FileSystem : extends Object {
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
		BA_EXISTS = 0x01,
		BA_REGULAR = 0x02,
		BA_DIRECTORY = 0x04,
		BA_HIDDEN = 0x08
	};
	virtual int getBooleanAttributes(const File& f) const = 0;

	enum {
		ACCESS_READ = 0x04,
		ACCESS_WRITE = 0x02,
		ACCESS_EXECUTE = 0x01
	};
	virtual boolean checkAccess(const File& f, int access) const = 0;
	/**
	 * Set on or off the access permission (to owner only or to all) to the file
	 * or directory denoted by the given abstract pathname, based on the parameters
	 * enable, access and oweronly.
	 */
	virtual boolean setPermission(const File& f, int access, boolean enable, boolean owneronly) const = 0;
	virtual jlong getLastModifiedTime(const File& f) const = 0;
	virtual jlong getLength(const File& f) const = 0;
	virtual boolean createFileExclusively(const String& pathname) const = 0;
	virtual boolean unlink(File f) const = 0; //delete=>unlink (delete is C++ is keyword)
	virtual Array<String> list(const File& f) const = 0;
	virtual boolean createDirectory(File f) const = 0;
	virtual boolean rename(const File& f1, const File& f2) const = 0;
	virtual boolean setLastModifiedTime(const File& f, jlong time) const = 0;
	virtual boolean setReadOnly(const File& f) const = 0;
	virtual Array<File> listRoots() const = 0;

	enum {
		SPACE_TOTAL  = 0,
		SPACE_FREE   = 1,
		SPACE_USABLE = 2
	};
	virtual jlong getSpace(const File& f, int t) const = 0;
	virtual int compare(const File& f1, const File& f2) const = 0;
	virtual jint hashCode(const File& f) const = 0;
};

interface FilenameFilter : Interface {
public:
	virtual boolean accept(const File& dir, const String& name) const = 0;
};
interface FileFilter : Interface {
public:
	virtual boolean accept(const File& path) const = 0;
};

class File : extends Object {
	friend class FileSystem;
public:
	//public static final char separatorChar = fs.getSeparator();
	static const char separatorChar = '/';
	static const String separator;
	static const char pathSeparatorChar = ':';
	static const String pathSeparator;
private:
	class PathStatus {
		public:
		enum { INVALID=1, CHECKED };
	};
	static const FileSystem& fs;

	String path;
	int prefixLength;
	int status = 0;

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
	/* -- Constructors -- */
	File() {}
	File(const String& path) {
		if (null_obj == path) throw NullPointerException();
		this->path = fs.normalize(path);
		this->prefixLength = fs.prefixLength(this->path);
	}
	File(const String& parent, const String& child) {
		if (null_obj == child) throw NullPointerException();
		if (null_obj != parent) {
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
		if (null_obj == child) throw NullPointerException();
		if (null_obj != parent) {
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

	/* -- Path-component accessors -- */
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
			return (String&)null_obj; //originaly null
		}
		return path.substring(0, index);
	}
	File getParentFile() const {
		String p = this->getParent();
		//if (p == null) return null;
		return File(p, this->prefixLength);
	}
	const String& getPath() const { return path; }

	/* -- Path operations -- */
	boolean isInvalid() const {
		if (status == 0)
			const_cast<int&>(status) = (path.indexOf('\u0000') < 0) ? PathStatus::CHECKED : PathStatus::INVALID;
		return status == PathStatus::INVALID;
	}
	boolean isAbsolute() const { return fs.isAbsolute(*this); }
	String getAbsolutePath() const { return fs.resolve(*this); }
	File getAbsoluteFile() const {
		String absPath = getAbsolutePath();
		return File(absPath, fs.prefixLength(absPath));
	}
	String getCanonicalPath() const {
		if (isInvalid()) throw IOException("Invalid file path");
		return fs.canonicalize(fs.resolve(*this));
	}
	File getCanonicalFile() const {
		String canonPath = getCanonicalPath();
		return File(canonPath, fs.prefixLength(canonPath));
	}
	//URI toURI() {}

	/* -- Attribute accessors -- */
	boolean canRead() const {
		if (isInvalid()) return false;
		return fs.checkAccess(*this, FileSystem::ACCESS_READ);
	}
	boolean canWrite() const {
		if (isInvalid()) return false;
		return fs.checkAccess(*this, FileSystem::ACCESS_WRITE);
	}
	boolean exists() const {
		if (isInvalid()) return false;
		return ((fs.getBooleanAttributes(*this) & FileSystem::BA_EXISTS) != 0);
	}
	boolean isDirectory() {
		if (isInvalid()) return false;
		return ((fs.getBooleanAttributes(*this) & FileSystem::BA_DIRECTORY) != 0);
	}
	boolean isFile() {
		if (isInvalid()) return false;
		return ((fs.getBooleanAttributes(*this) & FileSystem::BA_REGULAR) != 0);
	}
	boolean isHidden() {
		if (isInvalid()) return false;
		return ((fs.getBooleanAttributes(*this) & FileSystem::BA_HIDDEN) != 0);
	}
	jlong lastModified() {
		if (isInvalid()) return 0L;
		return fs.getLastModifiedTime(*this);
	}
	jlong length() {
		if (isInvalid()) return 0L;
		return fs.getLength(*this);
	}

	/* -- File operations -- */
	boolean createNewFile() {
		if (isInvalid()) throw IOException("Invalid file path");
		return fs.createFileExclusively(path);
	}
	boolean unlink() {
		if (isInvalid()) return false;
		return fs.unlink(*this);
	}
	void deleteOnExit() {
		if (isInvalid()) return ;
		//DeleteOnExitHook.add(path);
	}
	Array<String> list() {
		if (isInvalid()) return Array<String>(); //null
		return fs.list(*this);
	}
	Array<String> list(const FilenameFilter& filter) {
		Array<String> names = list();
		if (names.length == 0) return names;
		ArrayList<String> v;
		for (int i = 0 ; i < names.length ; i++) {
			if (filter.accept(*this, names[i])) v.add(names[i]);
		}
		return v.toArray();
	}
	Array<File> listFiles() {
		Array<String> names = list();
		if (names.length == 0) return Array<File>();
		int n = names.length;
		Array<File> fs(n);
		for (int i = 0; i < n; i++)
			fs[i] = File(names[i], *this);
		return fs;
	}
	Array<File> listFiles(const FilenameFilter& filter) {
		Array<String> names = list();
		if (names.length == 0) return Array<File>();
		ArrayList<File> v;
		for (int i = 0 ; i < names.length ; i++) {
			if (filter.accept(*this, names[i]))
				v.add(File(names[i], *this));
		}
		return v.toArray();
	}
	Array<File> listFiles(const FileFilter& filter) {
		Array<String> names = list();
		if (names.length == 0) return Array<File>();
		ArrayList<File> v;
		for (int i = 0 ; i < names.length ; i++) {
			File f(names[i]);
			if (filter.accept(f))
				v.add(f);
		}
		return v.toArray();
	}
	boolean mkdir() {
		if (isInvalid()) return false;
		return fs.createDirectory(*this);
	}
	boolean mkdirs() {
		if (exists()) return false;
		if (mkdir()) return true;
		try {
			File canonFile = getCanonicalFile();
			File parent = canonFile.getParentFile();
			return (parent.mkdirs() || parent.exists()) && canonFile.mkdir();
		} catch (const IOException& e) {
			return false;
		}
	}
	boolean renameTo(const File& dest) {
		if (dest == null_obj) throw NullPointerException();
		if (isInvalid() || dest.isInvalid()) return false;
		return fs.rename(*this, dest);
	}
	boolean setLastModified(long time) {
		if (isInvalid()) return false;
		return fs.setLastModifiedTime(*this, time);
	}
	boolean setReadOnly() {
		if (isInvalid()) return false;
		return fs.setReadOnly(*this);
	}
	boolean setWritable(boolean writable, boolean ownerOnly) {
		if (isInvalid()) return false;
		fs.setPermission(*this, FileSystem::ACCESS_WRITE, writable, ownerOnly);
	}

	/* -- Filesystem interface -- */
	static Array<File> listRoots() { return fs.listRoots(); }
};

} //namespace io

#endif
