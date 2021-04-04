using namespace std;

class Symbol
{
private:
  int code;
  int numberLine;
  int numberPosition;

public:
  Symbol(int code, int numberLine, int numberPosition);
};

Symbol::Symbol(int code, int numberLine, int numberPosition)
{
  this->code = code;
  this->numberLine = numberLine;
  this->numberPosition = numberPosition;
}
