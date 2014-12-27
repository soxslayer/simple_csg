#ifndef __ZERO_ONE__
#define __ZERO_ONE__

#include <type_traits>

namespace Util
{

template<typename T>
class ZeroOne
{
public:
  static T zero ()
  {
    static_assert (std::is_arithmetic<T>::value, "non-arithmetic type");

    return 0;
  }

  static T one ()
  {
    static_assert (std::is_arithmetic<T>::value, "non-arithmetic type");

    return 1;
  }
};

} /* namespace Util */

#endif /* __ZERO_ONE__ */
