#ifndef __BITS__
#define __BITS__

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace Util
{

template<typename T>
unsigned int n_bits ()
{
  return sizeof (T) * 8;
}

template<typename T>
T bit_mask (unsigned int n, unsigned int offset)
{
  T ret;

  memset (&ret, 0xff, sizeof (ret));
  ret >>= n_bits<T> () - n;
  ret <<= offset;

  return ret;
}

} /* namespace Util */

#endif /* __BITS__ */
