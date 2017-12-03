#ifndef __IO_APPENDABLE_HPP
#define __IO_APPENDABLE_HPP

#include <lang/Object.hpp>

namespace io {

interface Appendable : Interface {
public:
	virtual Appendable& append(const CharSequence& csq) = 0;
	virtual Appendable& append(const CharSequence& csq, int start, int end) = 0;
	virtual Appendable& append(char c) = 0;
};

} //namespace io

#endif
