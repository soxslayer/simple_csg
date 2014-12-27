#ifndef __HASH__
#define __HASH__

#include "bits.h"

namespace Util
{

template<typename T>
class Hash
{
public:
  Hash (const T& obj)
    : _obj (obj)
  { reset (); }

  void reset ()
  {
    _hash = hash ();
  }

  void update ()
  {
    _hash = hash ();
  }

  uint32_t hash () const
  {
    const uint8_t* raw_ptr = (uint8_t*)&_obj;
    uint32_t h = _hash;

    for (int i = 0; i < sizeof (_obj); ++i) {
      uint32_t ho = h & bit_mask<uint32_t> (5, 27);
      h <<= 5;
      h ^= ho >> 27;
      h ^= raw_ptr[i];
    }

    return h;
  }

  bool changed () const
  {
    uint32_t old_hash = _hash;
    return old_hash != hash ();
  }

private:
  const T& _obj;
  uint32_t _hash;
};

} /* namespace Util */

#endif /* __HASH__ */
