class Litera
{
private:
  int numberLine;
  int numberPosition;

public:
  char ch;
  Litera(char ch, int numberLine, int numberPosition);
};

Litera::Litera(char ch, int numberLine, int numberPosition)
{
  this->ch = ch;
  this->numberLine = numberLine;
  this->numberPosition = numberPosition;
}
