#ifndef __DEBUG__
#define __DEBUG__

#include <iostream>

#define DBG(m) std::cerr << __PRETTY_FUNCTION__ << '[' << __LINE__ << "] " << m << std::endl;
#define DBG_VAR(v) DBG(#v << '=' << v)

#endif /* __DEBUG__ */
