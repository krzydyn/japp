#ifndef __ACCESSIBILITY_ACCESSIBLE_HPP
#define __ACCESSIBILITY_ACCESSIBLE_HPP

#include <lang/Object.hpp>

namespace accessibility {

class AccessibleContext;
interface Accessible : Interface {
public:
	virtual AccessibleContext* getAccessibleContext() = 0;
};

}

#endif
