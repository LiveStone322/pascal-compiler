#include <iostream>
#include <fstream>
#include "./models/lexer.cpp"
using namespace std;

const string filePath = "./pascalSrc/program.pas";

int main(int argc, char **argv)
{
  Lexer lexer;
  lexer.readFile(filePath);
  lexer.scan();
  lexer.print();

  return 0;
}