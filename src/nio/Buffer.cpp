#include <lang/Exception.hpp>
#include <nio/Buffer.hpp>

namespace nio {

class HeapCharBuffer : extends CharBuffer {
	int compareTo(const CharBuffer& o) const {
		return false;
	}
	int length() const { return 0; }
	char charAt(int index) const { return 0; }
};

CharBuffer& CharBuffer::allocate(int capacity) {
	if (capacity < 0) throw IllegalArgumentException();
	return new HeapCharBuffer(capacity, capacity);
}

} //namespace nio
