#include <lang/Number.hpp>

#include "XBaseWindow.hpp"
#include "XConstants.hpp"

namespace awt { namespace x11 {

class XRootWindow : extends XBaseWindow {
private:
	XRootWindow() : XBaseWindow(XCreateWindowParams().put(EVENT_MASK, XConstants::StructureNotifyMask)) {}
public:
	static XRootWindow *getInstance();
	static long getXRootWindow();

	void postInit(XCreateWindowParams& params);
};

}}
