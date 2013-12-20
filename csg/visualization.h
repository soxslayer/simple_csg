#ifndef __VISUALIZATION__
#define __VISUALIZATION__

namespace CSG
{

class Visualization
{
public:
  virtual ~Visualization () { }

  virtual void draw () = 0;
};

} /* namespace CSG */

#endif /* __VISUALIZATION__ */
