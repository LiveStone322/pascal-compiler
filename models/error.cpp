#include <string>
using namespace std;

class Error
{
public:
  string code;
  string description;
  Error(string code, string description);
};

Error::Error(string code, string description)
{
  this->code = code;
  this->description = description;
}
