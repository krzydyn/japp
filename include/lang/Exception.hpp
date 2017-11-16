#ifndef __LANG_EXCEPTION_HPP
#define __LANG_EXCEPTION_HPP

#include <lang/String.hpp>
#include <cstring>

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
	}

	const String&  getMethodName() const { return methodName; }
   	const String& getFileName() const { return fileName; }
	int getLineNumber() const { return lineNumber; }
	String toString() const {
		if (fileName.length() || lineNumber) return methodName + "(" +fileName + ":" + lineNumber + ")";
		return methodName;
	}
};

class Throwable : public Object {
private:
	String threadInfo;
	String detailMessage;
	Array<StackTraceElement> stackTrace;
	Throwable *cause = null;
	void move(Throwable* o) {
		threadInfo = std::move(o->threadInfo);
		detailMessage = std::move(o->detailMessage);
		stackTrace = std::move(o->stackTrace);
		cause = o->cause; o->cause = null;
	}

public:
	Throwable(const Throwable& o) = delete;
	Throwable& operator=(const Throwable& o) = delete;
	Throwable(Throwable&& o) {move(&o);}
	Throwable& operator=(Throwable&& o) {move(&o); return *this;}
	~Throwable() {}

	Throwable() : Throwable("", null) {}
	Throwable(const String& msg, Throwable *c=null);

	virtual const String& getMessage() const {return detailMessage;}
	virtual const String& getLocalizedMessage() const {return getMessage();}
	Throwable& initCause(Throwable *c);
	String toString() const;
	void printStackTrace() const;
	void printStackTrace(const io::PrintStream& s) const;
	Throwable& fillInStackTrace();
	const Array<StackTraceElement>& getStackTrace() const { return stackTrace; }
	void setStackTrace(Array<StackTraceElement>& st) { stackTrace=std::move(st); }
};

class Error : public Throwable {
public:
	Error() : Throwable() {}
	Error(const String& msg) : Throwable(msg) {}
};

class Exception : public Throwable {
public:
	Exception() : Throwable() {}
	Exception(const String& msg) : Throwable(msg) {}
	Exception(const String& msg, Throwable *c) : Throwable(msg, c) {}
};

class OutOfMemoryError : public Error {
public:
	OutOfMemoryError() : Error() {}
	OutOfMemoryError(const String& msg) : Error(msg) {}
};

class ArithmeticException : public Exception {
public:
	ArithmeticException() : Exception() {}
	ArithmeticException(const String& msg) : Exception(msg) {}
};
class NullPointerException : public Exception {
public:
	NullPointerException() : Exception() {}
	NullPointerException(const String& msg) : Exception(msg) {}
};
class CloneNotSupportedException : public Exception {
public:
	CloneNotSupportedException() : Exception() {}
	CloneNotSupportedException(const String& msg) : Exception(msg) {}
};
class UnsupportedOperationException : public Exception {
public:
	UnsupportedOperationException() : Exception() {}
	UnsupportedOperationException(const String& msg) : Exception(msg) {}
};

class IndexOutOfBoundsException : public Exception {
public:
	IndexOutOfBoundsException() : Exception() {}
	IndexOutOfBoundsException(const String& msg) : Exception(msg) {}
	IndexOutOfBoundsException(int i) : Exception("Index "+String::valueOf(i)) {}
};

class IllegalThreadStateException : public Exception {
public:
	IllegalThreadStateException() : Exception() {}
	IllegalThreadStateException(const String& msg) : Exception(msg) {}
};

class IllegalArgumentException : public Exception {
public:
	IllegalArgumentException() : Exception() {}
	IllegalArgumentException(const String& msg) : Exception(msg) {}
	IllegalArgumentException(const String& msg, Throwable *c) : Exception(msg, c) {}
};

class IllegalStateException : public Exception {
public:
	IllegalStateException() : Exception() {}
	IllegalStateException(const String& msg) : Exception(msg) {}
	IllegalStateException(const String& msg, Throwable *c) : Exception(msg, c) {}
};

inline Throwable& Throwable::initCause(Throwable *cause) {
	if (this->cause != this)
		throw IllegalStateException("Can't overwrite cause with a null", this);
	if (cause == this)
		throw IllegalArgumentException("Self-causation not permitted", this);
	this->cause = cause;
	return *this;
}

} //namespace lang

#endif
