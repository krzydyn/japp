#include <lang/Character.hpp>
#include <net/NameService.hpp>
#include <net/ProtocolFamily.hpp>
#include <net/Socket.hpp>
#include <util/ArrayList.hpp>
#include <util/HashMap.hpp>

#include <sys/socket.h>

namespace net {

int ProtocolFamily::enumIndex = 0;

const StandardProtocolFamily StandardProtocolFamily::INET(AF_INET);
const StandardProtocolFamily StandardProtocolFamily::INET6(AF_INET6);


String Inet4Address::numericToTextFormat(uint32_t a) {
	return String::format("%d.%d.%d.%d",(a>>24)&&0xff,(a>>16)&&0xff,(a>>8)&&0xff,(a>>0)&&0xff);
}

namespace {
interface InetAddressImpl : Interface {
public:
	virtual String getLocalHostName() = 0;
	virtual Array<Shared<InetAddress>> lookupAllHostAddr(const String& hostname) = 0;
	virtual String getHostByAddr(Array<byte> addr) = 0;
	virtual Shared<InetAddress> anyLocalAddress() = 0;
	virtual Shared<InetAddress> loopbackAddress() = 0;
	//boolean isReachable(InetAddress addr, int timeout, NetworkInterface netif, int ttl) = 0;
};
class Inet4AddressImpl final : implements InetAddressImpl {
private:
	Shared<InetAddress>      mAnyLocalAddress = null;
	Shared<InetAddress>      mLoopbackAddress = null;
public:
	String getLocalHostName() { return "localhost"; }
	Array<Shared<InetAddress>> lookupAllHostAddr(const String& hostname) {
		Array<Shared<InetAddress>> ret;
		return ret;
	}
	String getHostByAddr(Array<byte> addr) {
		return "hostaddr";
	}
	Shared<InetAddress> anyLocalAddress() {
		if (mAnyLocalAddress == null) {
			mAnyLocalAddress = makeShared<Inet4Address>("localhost",0x7f000001);
		}
		return mAnyLocalAddress;
	}
	Shared<InetAddress> loopbackAddress() {
		if (mLoopbackAddress == null) {
			mLoopbackAddress = makeShared<Inet4Address>("localhost",0x7f000001);
		}
		return mLoopbackAddress;
	}
};
class IPAddressUtil {
	static const int INADDR4SZ = 4;
public:
	static Array<byte> textToNumericFormatV4(const String& src);
	static Array<byte> textToNumericFormatV6(const String& src);
};

Array<byte> IPAddressUtil::textToNumericFormatV4(const String& src) {
	Array<byte> zero_array;
	int len = src.length();
	if (len == 0 || len > 15) return zero_array;

	Array<byte> res(INADDR4SZ);
	long tmpValue = 0;
	int currByte = 0;
	for (int i = 0; i < len; i++) {
		char c = src.charAt(i);
		if (c == '.') {
			if (tmpValue < 0 || tmpValue > 0xff || currByte == 3) {
				return zero_array;
			}
			res[currByte++] = (byte) (tmpValue & 0xff);
			tmpValue = 0;
		}
		else {
			int digit = Character::digit(c, 10);
			if (digit < 0) return zero_array;
			tmpValue *= 10;
			tmpValue += digit;
		}
	}
	if (tmpValue < 0 || tmpValue >= (1L << ((4 - currByte) * 8))) {
		return zero_array;
	}
	switch (currByte) {
		case 0:
			res[0] = (byte) ((tmpValue >> 24) & 0xff);
		case 1:
			res[1] = (byte) ((tmpValue >> 16) & 0xff);
		case 2:
			res[2] = (byte) ((tmpValue >> 8) & 0xff);
		case 3:
			res[3] = (byte) ((tmpValue >> 0) & 0xff);
	}
	return res;
}
Array<byte> IPAddressUtil::textToNumericFormatV6(const String& src) {
	Array<byte> zero_array;
	int len = src.length();
	if (len < 2) return zero_array;
	return zero_array;
}

InetAddressImpl& getImpl() {
	static Inet4AddressImpl inet4;
	return inet4;
}

static InetAddressImpl& impl = getImpl();
Array<Shared<InetAddress>> unknown_array(1);

//List<Shared<NameService>>& getNameServices() {
//	static ArrayList<Shared<NameService>> nameServices;
//	return nameServices;
//}
HashMap<String,Array<Shared<InetAddress>>> addressCache;
void cacheInitIfNeeded() {
	static boolean addressCacheInit = false;
	if (addressCacheInit) return ;
	unknown_array[0] = impl.anyLocalAddress();
	addressCache.put(impl.anyLocalAddress()->getHostName(), unknown_array);
	addressCacheInit = true;
}
//void cacheAddresses(const String& hostname, const Array<Shared<InetAddress>>& addresses, boolean success) {
//	addressCache.put(hostname, addresses);
//}
const Array<Shared<InetAddress>> getCachedAddresses(const String& hostname) {
	String h = hostname.toLowerCase();
	Array<Shared<InetAddress>> ret;
	synchronized (addressCache) {
		cacheInitIfNeeded();
		const HashMap<String,Array<Shared<InetAddress>>>& cache = addressCache;
		const Array<Shared<InetAddress>>& a = cache.get(h);
		if (a != null) ret = a;
	}
	return ret;
}
Array<Shared<InetAddress>> getAddressesFromNameService(const String& host, const InetAddress& reqAddr) {
	//List<Shared<NameService>>& nameServices = getNameServices();
	//for (Shared<NameService> nameService : nameServices) { }
	return unknown_array;
}
Array<Shared<InetAddress>> priv_getAllByName0(const String& host, const InetAddress& reqAddr, boolean check) {
	Array<Shared<InetAddress>> addresses = getCachedAddresses(host);
	if (addresses.length == 0) {
		addresses = getAddressesFromNameService(host, reqAddr);
	}
	if (addresses == unknown_array) {
		throw UnknownHostException(host);
	}
	return addresses;
}
int priv_checkNumericZone(const String& s) {
	int percent = s.indexOf ('%');
	int slen = s.length();
	int digit, zone=0;
	if (percent == -1) {
		return -1;
	}
	for (int i=percent+1; i<slen; i++) {
		char c = s.charAt(i);
		if (c == ']') {
			if (i == percent+1) {
				/* empty per-cent field */
				return -1;
			}
			break;
		}
		if ((digit = Character::digit (c, 10)) < 0) {
			return -1;
		}
		zone = (zone * 10) + digit;
	}
	return zone;
}
Array<Shared<InetAddress>> priv_getAllByName(const String& host, const InetAddress& reqAddr) {
	if (host == null || host.length() == 0) {
		Array<Shared<InetAddress>> ret(1);
		ret[0] = impl.loopbackAddress();
		return ret;
	}
	boolean ipv6Expected = false;
	String h = host;
	if (h.charAt(0) == '[') {
		if (h.length() > 2 && h.charAt(host.length()-1) == ']') {
			h = h.substring(1, h.length() -1);
			ipv6Expected = true;
		}
		else throw UnknownHostException(host + ": invalid IPv6 address");
	}

	if (Character::digit(h.charAt(0), 16) != -1 || (h.charAt(0) == ':')) {
		Array<byte> addr;
		int numericZone = -1;
		String ifname;
		addr = IPAddressUtil::textToNumericFormatV4(host);
		if (addr.length == 0) {
			int pos;
			if ((pos=host.indexOf ("%")) != -1) {
				numericZone = priv_checkNumericZone(host);
				if (numericZone == -1) ifname = host.substring(pos+1);
			}
			addr = IPAddressUtil::textToNumericFormatV6(host);
			if (addr.length == 0 && host.contains(":")) {
				throw UnknownHostException(host + ": invalid IPv6 address");
			}
		}
		Array<Shared<InetAddress>> ret(1);
		if (addr.length > 0) {
			ret[0] = makeShared<Inet4Address>(null, addr);
			return ret;
		}
	}
	else if (ipv6Expected) {
		throw UnknownHostException(host);
	}

	return priv_getAllByName0(h, reqAddr, true);
}
}

Shared<InetAddress> InetAddress::anyLocalAddress() {
	return impl.anyLocalAddress();
}

Array<Shared<InetAddress>> InetAddress::getAllByName(const String& host) {
	return priv_getAllByName(host, (const InetAddress&)null_obj);
}

Shared<InetAddress> InetAddress::getByName(const String& host) {
	return InetAddress::getAllByName(host)[0];
}

int InetSocketAddress::checkPort(int port) {
	if (port < 0 || port > 0xFFFF)
		throw IllegalArgumentException("port out of range:" + port);
	return port;
}

const String& InetSocketAddress::checkHost(const String& hostname) {
	if (hostname == null)
		throw new IllegalArgumentException("hostname can't be null");
	return hostname;
}

}
