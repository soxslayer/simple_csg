#ifndef __EXCEPTIONS__
#define __EXCEPTIONS__

#include <stdexcept>

namespace CSG
{

class Exception : public std::runtime_error
{
public:
  explicit Exception (const std::string& msg);
  virtual ~Exception () { }
};

class UnsupportedException : public Exception
{
public:
  explicit UnsupportedException (const std::string& what);
  virtual ~UnsupportedException () { }
};

class FileException : public Exception
{
public:
  explicit FileException (const std::string& filename,
                          const std::string& reason);
  virtual ~FileException () { }
};

}

#endif /* __EXCEPTIONS__ */
