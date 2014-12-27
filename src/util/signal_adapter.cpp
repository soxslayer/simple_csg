#include "signal_adapter.h"

namespace Util
{

std::map<int, SignalAdapter::SignalHandler> SignalAdapter::_handlers;

void SignalAdapter::register_handler (int sig, SignalHandler handler)
{
  signal (sig, SIG_IGN);

  _handlers.emplace (sig, handler);

  signal (sig, SignalAdapter::signal_trampoline);
}

void SignalAdapter::unregister_handler (int sig)
{
  signal (sig, SIG_DFL);
  _handlers.erase (sig);
}

void SignalAdapter::signal_trampoline (int sig)
{
  if (_handlers.count (sig) == 0)
    return;

  _handlers[sig] (sig);
}

} /* namespace Util */
