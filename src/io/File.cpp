#include <lang/System.hpp>
#include <io/File.hpp>

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE 1
#endif
#define _LARGE_TIME_API 

#include <dirent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <unistd.h>
#include <utime.h>

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
		const String& name = f.getName();
		if (name.length() > 0 && f.getName().charAt(0) == '.') attr |= BA_HIDDEN;
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
		struct stat st;
		int perm = 0;
		if (::stat(f.getPath().intern().c_str(), &st) < 0)
			return 0;
		if (access&ACCESS_READ) perm |= S_IRUSR|S_IRGRP|S_IROTH;
		if (access&ACCESS_WRITE) perm |= S_IWUSR|S_IWGRP|S_IWOTH;
		if (access&ACCESS_EXECUTE) perm |= S_IXUSR|S_IXGRP|S_IXOTH;
		if (owneronly) perm &= S_IRWXU;
		if (enable) perm = st.st_mode | perm;
		else perm = st.st_mode & (~perm);
		perm &= S_IRWXU | S_IRWXG | S_IRWXO;
		return chmod(f.getPath().intern().c_str(), perm) == 0;
	}
	virtual jlong getLastModifiedTime(const File& f) const {
		struct stat st;
		if (::stat(f.getPath().intern().c_str(), &st) < 0)
			return 0;
		#ifdef __APPLE__
		jlong millis = st.st_mtimespec.tv_sec; millis *= 1000;
		millis += st.st_mtimespec.tv_nsec / 1000000;
		#else
		jlong millis = st.st_mtim.tv_sec; millis *= 1000;
		millis += st.st_mtim.tv_nsec / 1000000;
		#endif
		return millis;
	}
	virtual jlong getLength(const File& f) const {
		struct stat st;
		if (::stat(f.getPath().intern().c_str(), &st) < 0)
			return 0;
		return st.st_size;
	}
	virtual boolean createFileExclusively(const String& pathname) const {
		return false;
	}
	virtual boolean unlink(File f) const {
		int r = ::unlink(f.getPath().intern().c_str());
		if (r == -1) {
			String emsg(std::strerror(errno));
			System.err.println("unlink error: "+emsg+"("+errno+") "+f.getPath());
		}
		return r == 0;
	}
	virtual Array<String> list(const File& f) const {
		DIR *dir = opendir(f.getPath().intern().c_str());
		struct dirent *ent;
		if (dir == null) return Array<String>();
		ArrayList<String> l;
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] == '.') continue;
			l.add(ent->d_name);
		}
		return l.toArray();
	}
	virtual boolean createDirectory(File f) const {
		return ::mkdir(f.getPath().intern().c_str(), 0777) == 0;
	}
	virtual boolean rename(const File& f1, const File& f2) const {
		return ::rename(f1.getPath().intern().c_str(),f2.getPath().intern().c_str());
	}
	virtual boolean setLastModifiedTime(const File& f, jlong time) const {
		//struct timeval times[2]; // if times is null => set date&time to current
		//utimes(path, times); <- legacy
		struct utimbuf ubuf;
		ubuf.modtime = time/1000;
		ubuf.actime = time/1000;
		return ::utime(f.getPath().intern().c_str(), &ubuf) == 0;
	}
	virtual boolean setReadOnly(const File& f) const {
		int perm = S_IRUSR|S_IRGRP|S_IROTH;
		return chmod(f.getPath().intern().c_str(), perm) == 0;
	}
	virtual Array<File> listRoots() const { Array<File> a(1); a[0]=File("/"); return a;}
	virtual jlong getSpace(const File& f, int t) const {
		struct statvfs stat;
		statvfs(f.getPath().intern().c_str(), &stat);
		if (t == SPACE_TOTAL) return stat.f_blocks * stat.f_frsize;
		if (t == SPACE_FREE) return stat.f_bavail * stat.f_frsize;
		if (t == SPACE_USABLE) return (stat.f_blocks-stat.f_bavail)*stat.f_frsize;
		return -1LL;
	}
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
