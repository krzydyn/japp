#ifndef __LANG_SYSTEM_HPP
#define __LANG_SYSTEM_HPP

#include <lang/Object.hpp>
#include <io/InputStream.hpp>
#include <io/PrintStream.hpp>

namespace lang {

class _System {
public:
	static io::PrintStream err;
	static io::PrintStream out;
	static io::InputStream in;

	static jlong currentTimeMillis() {
		return 0;
	}
};

extern const _System System;

} //namespace lang

#endif
