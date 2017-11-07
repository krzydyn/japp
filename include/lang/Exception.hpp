#ifndef __LANG_EXCEPTION_HPP
#define __LANG_EXCEPTION_HPP

#include <lang/String.hpp>
#include <cstring>

namespace io { class PrintStream; }

namespace lang {
template<class T>
class Array {
private:
	T *a;
public:
	Array<T>& operator=(const Array<T>&o) {
		if (this == &o) return *this;
		delete [] a;
		const_cast<int&>(length) = o.length;
		a = new T[length];
		for (int i=0; i < length; ++i) a[i] = o.a[i];
		return *this;
	}
	/*Array<T>& operator=(Array<T>&& o) {
		const_cast<int&>(length) = o.length;
		a = o.a;
		o.a = null;
		return *this;
	}*/

	const int length;
	Array() : length(0) { a = null; }
	Array(int l) : length(l) { a = new T[l]; }
	~Array() { delete [] a; }
	T& operator[](int i) { return a[i]; }
	const T& operator[](int i) const { return a[i]; }
};

class StackTraceElement {
private:
	void * fptr;
	String methodName;
	String fileName;
	int    lineNumber;
	void fillInfo();
public:
	StackTraceElement(StackTraceElement&& o) {
		fptr = std::move(o.fptr);
		methodName = std::move(o.methodName);
		fileName = std::move(o.fileName);
		lineNumber = o.lineNumber; o.lineNumber=0;
	}
	StackTraceElement(const StackTraceElement& o) {
		fptr = o.fptr;
		methodName = o.methodName;
		fileName = o.fileName;
		lineNumber = o.lineNumber;
	}
	StackTraceElement& operator=(StackTraceElement&& o) {
		fptr = std::move(o.fptr);
		methodName = std::move(o.methodName);
		fileName = std::move(o.fileName);
		lineNumber = o.lineNumber; o.lineNumber=0;
		return *this;
	}
	StackTraceElement& operator=(const StackTraceElement& o) {
		fptr = o.fptr;
		methodName = o.methodName;
		fileName = o.fileName;
		lineNumber = o.lineNumber;
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
public:
	~Throwable() {}

	Throwable() {fillInStackTrace();}
	Throwable(const String& msg) : detailMessage(msg) {fillInStackTrace();}
	Throwable(const String& msg, Throwable *c) : detailMessage(msg), cause(c) {fillInStackTrace();}

	virtual const String& getMessage() const {return detailMessage;}
	virtual const String& getLocalizedMessage() const {return getMessage();}
	Throwable& initCause(Throwable *c);
	String toString() const;
	void printStackTrace() const;
	void printStackTrace(const io::PrintStream& s) const;
	Throwable& fillInStackTrace();
	const Array<StackTraceElement>& getStackTrace() const { return stackTrace; }
private:
	String detailMessage;
	Array<StackTraceElement> stackTrace;
	Throwable *cause = null;
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
	IndexOutOfBoundsException(int i) : Exception(String("Index ")+i) {}
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
