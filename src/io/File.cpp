#include <lang/System.hpp>
#include <io/File.hpp>

#include <sys/stat.h>
#include <unistd.h>

namespace io {
namespace {
class UnixFileSystem : public FileSystem {
private:
	String normalize(const String& path, int len, int off) const {
		if (len == 0) return path;
		int n = len;
		while ((n > 0) && (path.charAt(n - 1) == '/')) n--;
		if (n == 0) return "/";
		StringBuilder sb(path.length());
		if (off > 0) sb.append(path.substring(0, off));
		char prevChar = 0;
		for (int i = 0; i < n; i++) {
			char c = path.charAt(i);
			if ((prevChar == '/') && (c == '/')) continue;
			sb.append(c);
			prevChar = c;
		}
		return sb.toString();
	}
public:
	virtual char getSeparator() const {return '/';}
	virtual char getPathSeparator() const {return ':';}
	virtual String normalize(const String& path) const {
		int n = path.length();
		char prevChar = 0;
		for (int i = 0; i < n; i++) {
			char c = path.charAt(i);
			if ((prevChar == '/') && (c == '/'))
				return normalize(path, n, i - 1);
			prevChar = c;
		}
		if (prevChar == '/') return normalize(path, n, n - 1);
		return path;
	}
	virtual int prefixLength(const String& path) const {
		if (path.length() == 0) return 0;
		return path.charAt(0) == '/' ? 1 : 0;
	}
	virtual String resolve(const String& parent, const String& child) const {
		if (child.equals("")) return parent;
		if (child.charAt(0) == '/') {
			if (parent.equals("/")) return child;
			return parent + child;
		}
		if (parent.equals("/")) return parent + child;
		return parent + '/' + child;
	}
	virtual String getDefaultParent() const {return "/";}
	virtual String fromURIPath(const String& path) const {
		String p = path;
		if (p.endsWith("/") && (p.length() > 1)) {
			p = p.substring(0, p.length() - 1);
		}
		return path;
	}
	virtual String resolve(const File& f) const {
		if (isAbsolute(f)) return f.getPath();
		return resolve(System.getProperty("user.dir"), f.getPath());
	}
	virtual String canonicalize(const String& path) const {return path;}
	virtual int getBooleanAttributes(const File& f) const {
		struct stat st;
		if (::stat(f.getPath().intern().c_str(), &st) < 0)
			return 0;
		int attr = BA_EXISTS;
		if (st.st_mode&S_IFREG) attr |= BA_REGULAR;
		if (st.st_mode&S_IFDIR) attr |= BA_DIRECTORY;
		return attr;
	}
	virtual boolean checkAccess(const File& f, int access) const {
		int amode = 0;
		if (access&ACCESS_READ) amode |= R_OK;
		if (access&ACCESS_WRITE) amode |= W_OK;
		if (access&ACCESS_EXECUTE) amode |= X_OK;
		return ::access(f.getPath().intern().c_str(), amode) == 0;
	}
	virtual boolean setPermission(const File& f, int access, boolean enable, boolean owneronly) const {
		return false;
	}
	virtual long getLastModifiedTime(const File& f) const {return 0l;}
	virtual long getLength(const File& f) const {return 0l;}
	virtual boolean createFileExclusively(const String& pathname) const {return false;}
	virtual boolean unlink(File f) const {return false;}
	virtual Array<String> list(const File& f) const {return Array<String>();}
	virtual boolean rename(const File& f1, const File& f2) const {return false;}
	virtual boolean setLastModifiedTime(const File& f, long time) const {return false;}
	virtual boolean setReadOnly(const File& f) const {return false;}
	virtual Array<File> listRoots() const {return Array<File>();}
	virtual long getSpace(const File& f, int t) const {return 0l;}
	virtual int compare(const File& f1, const File& f2) const {return 0;}
	virtual int hashCode(const File& f) const {return 0;}
} unixfs;
}

boolean FileSystem::isAbsolute(const File& f) const {
	return f.getPrefixLength() != 0;
}
const FileSystem& FileSystem::getFileSystem() {
	return unixfs;
}

boolean FileSystem::useCanonCaches      = true;
boolean FileSystem::useCanonPrefixCache = true;
boolean FileSystem::getBooleanProperty(const String& prop, boolean defaultVal) {
	//String val = System.getProperty(prop);
	return false;
}

const String File::separator = "/";
const String File::pathSeparator = ":";
const FileSystem& File::fs = FileSystem::getFileSystem();

}
