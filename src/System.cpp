#include <lang/System.hpp>

namespace lang {
io::InputStream _System::in = std::cin;
io::PrintStream _System::out = std::cout;
io::PrintStream _System::err = std::cerr;
const _System System;
}
