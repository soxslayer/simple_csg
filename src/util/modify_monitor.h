#ifndef __MODIFY_MONITOR__
#define __MODIFY_MONITOR__

#include <functional>
#include <map>

namespace Util
{

template<typename T>
class ModifyMonitor
{
public:
  typedef std::function<void (const T&)> Callback;

  ModifyMonitor (T& obj)
    : _obj (obj), _id (0)
  { }

  int add_monitor (Callback& callback)
  {
    _monitors.emplace (++_id, callback);
    return _id;
  }

  void remove_monitor (int id)
  {
    if (_monitors.count (id) > 0)
      _monitors.erase (id);
  }

  const T& operator-> () const
  {
    return _obj;
  }

  T& operator-> () const
  {
    notify_monitors ();
    return _obj;
  }

  operator const T& () const
  {
    return _obj;
  }

  operator T& ()
  {
    notify_monitors ();
    return _obj;
  }

private:
  T& _obj;
  int _id;
  std::map<int, Callback> _monitors;

  void notify_monitors ()
  {
    auto m = _monitors.begin ();
    auto m_end = _monitors.end ();
    for (; m != m_end; ++m)
      m->second (_obj);
  }
};

} /* namespace Util */

#endif /* __MODIFY_MONITOR__ */
