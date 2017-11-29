#ifndef __UTIL_PROPERTIES_HPP
#define __UTIL_PROPERTIES_HPP

#include <util/HashMap.hpp>

namespace util {

class Properties : extends Object {
	HashMap<String,String> props;
public:
	const String& setProperty(const String& key, const String& value) {TRACE;
		return props.put(key, value);
	}
	const String& getProperty(const String& key) const {TRACE;
		return props.get(key);
	}
	const String& getProperty(const String& key, const String& defaultValue) const {TRACE;
		String val = getProperty(key);
		return (val == null) ? defaultValue : val;
	}
	const String remove(const String& key) {
		return props.remove(key);
	}
};

} //namespace util

#endif
