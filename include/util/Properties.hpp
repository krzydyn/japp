#ifndef __UTIL_PROPERTIES_HPP
#define __UTIL_PROPERTIES_HPP

#include <util/HashMap.hpp>

namespace util {

class Properties : public Object {
	HashMap<String,String> props;
public:
	const String& setProperty(const String& key, const String& value) {
		return props.put(key, value);
	}
	const String& getProperty(const String& key) const {
		return props.get(key);
	}
	const String& getProperty(const String& key, const String& defaultValue) const {
		String val = getProperty(key);
		return (val == null) ? defaultValue : val;
	}
};

} //namespace util

#endif
