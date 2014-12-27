#ifndef __SIGNAL_ADAPTER__
#define __SIGNAL_ADAPTER__

#include <functional>
#include <map>
#include <signal.h>

namespace Util
{

class SignalAdapter
{
public:
  typedef std::function<void (int)> SignalHandler;

  static void register_handler (int sig, SignalHandler handler);
  static void unregister_handler (int sig);

private:
  static std::map<int, SignalHandler> _handlers;

  static void signal_trampoline (int sig);
};

} /* namespace Util */

#endif /* __SIGNAL_ADAPTER__ */
