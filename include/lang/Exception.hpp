#ifndef __LANG_EXCEPTION_HPP
#define __LANG_EXCEPTION_HPP

#include <lang/String.hpp>
#include <cstring>

namespace lang {
template<class T>
class Array {
private:
	T *a;
public:
	Array<T>& operator=(const Array<T>&o) {
		const_cast<int&>(length) = o.length;
		a = new T[length];
		for (int i=0; i < length; ++i) a[i] = o.a[i];
	}
	/*Array<T>& operator=(Array<T>&& o) {
		const_cast<int&>(length) = o.length;
		a = o.a;
		o.a = nullptr;
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
	String methodName;
	String fileName;
	int    lineNumber;
public:
	StackTraceElement(StackTraceElement&& o) { methodName = std::move(o.methodName); }
	StackTraceElement(const StackTraceElement& o) { methodName = o.methodName; }
	StackTraceElement& operator=(StackTraceElement&& o) { methodName = std::move(o.methodName); }
	StackTraceElement& operator=(const StackTraceElement& o) { methodName = o.methodName; }
	~StackTraceElement() {}

	StackTraceElement(){}
	StackTraceElement(const String& methodName, const String& fileName, int lineNumber) :
   		methodName(methodName), fileName(fileName), lineNumber(lineNumber) {
	}

	const String&  getMethodName() { return methodName; }
   	const String& getFileName() { return fileName; }
	int getLineNumber() { return lineNumber; }
};

class Throwable : public Object {
public:
	Throwable() : stackTrace() {}
	Throwable(const String& msg) : detailMessage(msg), stackTrace() {}
	~Throwable() {}
	virtual const String& getMessage() const {return detailMessage;}
	virtual const String& getLocalizedMessage() const {return getMessage();}
	String toString() const {
		String s = getClass().getName();
		String message = getLocalizedMessage();
		return (message != null) ? (s + ": " + message) : s;
	}
	Throwable& fillInStackTrace() {
		captureStack(50);
		return *this;
	}
private:
	String detailMessage;
	Array<StackTraceElement> stackTrace;
	Throwable *cause;
	void captureStack(int depth);
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
};

} //namespace lang

#endif
