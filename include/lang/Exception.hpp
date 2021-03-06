#ifndef __LANG_EXCEPTION_HPP
#define __LANG_EXCEPTION_HPP

#include <util/List.hpp>

namespace io { class PrintStream; }

namespace lang {
class StackTraceElement final {
private:
	void * fptr;
	String methodName;
	String fileName;
	int lineNumber;
	void fillInfo();
public:
	StackTraceElement(const StackTraceElement& o) {
		fptr = o.fptr;
		methodName = o.methodName;
		fileName = o.fileName;
		lineNumber = o.lineNumber;
	}
	StackTraceElement& operator=(const StackTraceElement& o) {
		fptr = o.fptr;
		methodName = o.methodName;
		fileName = o.fileName;
		lineNumber = o.lineNumber;
		return *this;
	}
	StackTraceElement(StackTraceElement&& o) {
		fptr = o.fptr;
		methodName = std::move(o.methodName);
		fileName = std::move(o.fileName);
		lineNumber = o.lineNumber; o.lineNumber=0;
	}
	StackTraceElement& operator=(StackTraceElement&& o) {
		fptr = o.fptr;
		methodName = std::move(o.methodName);
		fileName = std::move(o.fileName);
		lineNumber = o.lineNumber; o.lineNumber=0;
		return *this;
	}
	~StackTraceElement() {}

	StackTraceElement(){}
	StackTraceElement(void *f) : fptr(f) {}
	StackTraceElement(const String& methodName, const String& fileName, int lineNumber) :
   		methodName(methodName), fileName(fileName), lineNumber(lineNumber) {
		fptr = null;
	}

	const String&  getMethodName() const { return methodName; }
   	const String& getFileName() const { return fileName; }
	int getLineNumber() const { return lineNumber; }
	String toString() const {
		if (fileName.length() || lineNumber) return methodName + "(" +fileName + ":" + lineNumber + ")";
		return methodName;
	}
};

class Throwable : extends Object {
private:
	String threadInfo;
	String detailMessage;
	Array<StackTraceElement> stackTrace;
	const Throwable *cause = this;
	util::List<Throwable*> *suppressedExceptions = null;
	void move(Throwable* o) {
		threadInfo = std::move(o->threadInfo);
		detailMessage = std::move(o->detailMessage);
		stackTrace = std::move(o->stackTrace);
		cause = o->cause; o->cause = o;
	}

public:
	Throwable(const Throwable& o) = delete;
	Throwable& operator=(const Throwable& o) = delete;
	Throwable(Throwable&& o) {move(&o);}
	Throwable& operator=(Throwable&& o) {move(&o); return *this;}
	~Throwable() {}

	Throwable(){}
	Throwable(const String& msg, Throwable *c=null);

	virtual const String& getMessage() const {return detailMessage;}
	virtual const String& getLocalizedMessage() const {return getMessage();}
	virtual const Throwable *getCause() const {
		return (cause==this ? null : cause);
	}
	Throwable& initCause(const Throwable *c);
	String toString() const;
	void printStackTrace() const;
	void printStackTrace(io::PrintStream& s) const;
	Throwable& fillInStackTrace();
	const Array<StackTraceElement>& getStackTrace() const { return stackTrace; }
	void setStackTrace(Array<StackTraceElement>& st) { stackTrace=std::move(st); }
	void addSuppressed(const Throwable& exception) {}
	Array<Throwable*> getSuppressed() {
		return suppressedExceptions == null ? Array<Throwable*>() : suppressedExceptions->toArray();
	}
};

class Error : extends Throwable {
public:
	using Throwable::Throwable;
};

class Exception : extends Throwable {
public:
	using Throwable::Throwable;
};

class OutOfMemoryError : extends Error {
public:
	using Error::Error;
};

class ThreadDeath : extends Error {
public:
	using Error::Error;
};

class RuntimeException : extends Exception {
public:
	using Exception::Exception;
};
class ArithmeticException : extends Exception {
public:
	using Exception::Exception;
};
class NullPointerException : extends Exception {
public:
	using Exception::Exception;
};
class CloneNotSupportedException : extends Exception {
public:
	using Exception::Exception;
};
class UnsupportedOperationException : extends Exception {
public:
	using Exception::Exception;
};

class IndexOutOfBoundsException : extends Exception {
public:
	using Exception::Exception;
	IndexOutOfBoundsException() {}
	IndexOutOfBoundsException(int i) : Exception("Index " + String::valueOf(i)) {}
};

class StringIndexOutOfBoundsException : extends IndexOutOfBoundsException {
public:
	using IndexOutOfBoundsException::IndexOutOfBoundsException;
	StringIndexOutOfBoundsException(int i) :
	   IndexOutOfBoundsException("String index out of range: " + String::valueOf(i)) {}
};

class IllegalThreadStateException : extends Exception {
public:
	using Exception::Exception;
};

class IllegalArgumentException : extends Exception {
public:
	using Exception::Exception;
};

class IllegalStateException : extends Exception {
public:
	using Exception::Exception;
};

} //namespace lang

#endif
