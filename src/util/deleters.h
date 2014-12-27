#ifndef __DELETERS__
#define __DELETERS__

namespace Util
{

template<typename T>
class Deleter
{
public:
  void operator() (T* p) const
  {
    delete p;
  }
};

template<typename T>
class ArrayDeleter
{
public:
  void operator() (T* p) const
  {
    delete [] p;
  }
};

template<typename T>
class NullDeleter
{
public:
  void operator() (T* p) const
  {
  }
};

} /* namespace Util */

#endif /* __DELETERS__ */
