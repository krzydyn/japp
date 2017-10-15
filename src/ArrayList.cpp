#include <util/ArrayList.hpp>

namespace util {

namespace {
boolean equals(int a, int b) { return a == b; }
boolean equals(const Object& a, const Object& b) { return a.equals(b); }
}

unsigned ArrayList::indexOf(const T& v,unsigned start) const {
	for (unsigned i=start; i < mSize; ++i ) {
		if (equals(mVec[(mOffs+i)%mCapa],v)) return i;
	}
	return eol;
}
} //namespace util
