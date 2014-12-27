#include "exceptions.h"

using namespace std;

namespace CSG
{

Exception::Exception (const string& msg)
  : runtime_error (msg)
{
}



UnsupportedException::UnsupportedException (const string& what)
  : Exception (string ("Unsupported feature ") + what)
{
}



FileException::FileException (const string& filename, const string& reason)
  : Exception (string ("FileException in") + filename + string (", ") + reason)
{
}

}
