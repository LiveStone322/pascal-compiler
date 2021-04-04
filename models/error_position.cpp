#include <string>
#include "error.cpp"

using namespace std;

class ErrorPosition
{
public:
  int numberLine;
  int position;
  Error *error;
  ErrorPosition(int numberLine, int position, Error *error);
};

ErrorPosition::ErrorPosition(int numberLine, int position, Error *error)
{
  this->numberLine = numberLine;
  this->position = position;
  this->error = error;
}