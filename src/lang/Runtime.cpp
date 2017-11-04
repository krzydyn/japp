#include <lang/Process.hpp>
#include <lang/Runtime.hpp>
#include <thread>

namespace {
const int MAX_SYSTEM_HOOKS = 10;
Array<Runnable*> hooks(MAX_SYSTEM_HOOKS);
void runHooks() {
	for (int i=0; i < MAX_SYSTEM_HOOKS; i++) {
		Runnable *hook;
		hook = hooks[i];
		if (hook != null) hook->run();
	}
}
}

namespace lang {

void Shutdown::exit(int status) {
	runHooks();
	Shutdown::halt(status);
}
void Shutdown::halt(int status) {
	std::exit(status);
}

Runtime Runtime::currentRuntime;
int Runtime::availableProcessors() {
	return std::thread::hardware_concurrency();
}

Process& ProcessBuilder::start() {
	Process *p = null;
	return *p;
}

} //namespace lang
