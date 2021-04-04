#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

/*
 * Adjastments
 */

enum Tag
{
  FINISH,
  BLOCK,
  IF,
  THEN,
  ELSE,
  WHILE,
  FUNCTION,
  PROCEDURE,
  BREAK,
  BEGIN,
  END,
  PROGRAM,
  CASE,
  OF,
  REPEAT,
  UNTIL,
  DO,
  VAR,
  TYPE,
  RECORD,
  CONSTANT,
  FOR,
  TO,
  DOWNTO,
  FORWORD,
  INT,
  CHAR,
  FLOAT,
  BOOL,
  STRING,
  IDENT,
  GT,
  LT,
  EQ,
  UNEQ,
  GE,
  LE,
  AND,
  NOT,
  OR,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  COMMA,
  COLON,
  CONCAT,
  SEMI,
  ADD,
  SUB,
  MUL,
  DIV,
  BIND
};

enum State
{
  _START,
  _END,
  _INT,
  _FLOAT,
  _STRING,
  _IDENT
};
const string humanReadableTags[] = {"FINISH", "BLOCK", "IF", "THEN", "ELSE", "WHILE", "FUNCTION",
                                    "PROCEDURE", "BREAK", "BEGIN", "END", "PROGRAM", "CASE",
                                    "OF", "REPEAT", "UNTIL", "DO", "VAR", "TYPE", "RECORD",
                                    "CONSTANT", "FOR", "TO", "DOWNTO", "FORWORD", "INT", "CHAR",
                                    "FLOAT", "BOOL", "STRING", "IDENT", "GT", "LT", "EQ",
                                    "UNEQ", "GE", "LE", "AND", "NOT", "OR", "OPEN_BRACKET",
                                    "CLOSE_BRACKET", "COMMA", "COLON", "CONCAT", "SEMI", "ADD",
                                    "SUB", "MUL", "DIV", "BIND"};

const map<string, Tag> keywords = {
    {"program", PROGRAM},
    {"if", IF},
    {"else", ELSE},
    {"while", WHILE},
    {"case", CASE},
    {"repeat", REPEAT},
    {"until", UNTIL},
    {"do", DO},
    {"break", BREAK},
    {"function", FUNCTION},
    {"procedure", PROCEDURE},
    {"begin", BEGIN},
    {"end", END},
    {"integer", INT},
    {"bool", BOOL},
    {"char", CHAR},
    {"float", FLOAT},
    {"string", STRING},
    {"or", OR},
    {"and", AND},
    {"then", THEN},
    {"of", OF},
    {"var", VAR},
    {"type", TYPE},
    {"for", FOR},
    {"to", TO},
    {"downto", DOWNTO},
    {"record", RECORD},
    {"const", CONSTANT},
    {"forword", FORWORD},
};

const char delimeters[] = {',', '.', '(', ')', '[', ']', ';', ':', '=', '<', '>', '+', '-', '*', '/', '&'};

string getHumanReadableTag(Tag t)
{
  return humanReadableTags[t];
}

bool isKeyword(string lex)
{
  //transform(lex.begin(), lex.end(), lex.begin(), [](unsigned char c) { return std::tolower(c); });
  for (auto i : keywords)
  {
    if (i.first.compare(lex) == 0)
      return true;
  }
  return false;
}

bool isDelimeter(char ch)
{
  for (auto i : delimeters)
  {
    if (i == ch)
      return true;
  }
  return false;
}

bool isWhitespace(char ch)
{
  if (ch == '\n' || ch == '\r' || ch == '\t' || ch == ' ')
    return true;
  else
    return false;
}
bool isLetter(char ch)
{
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}
bool isNum(char ch)
{
  return ch >= '0' && ch <= '9';
}
bool isLetterOrNum(char ch)
{
  return isLetter(ch) || isNum(ch);
}
bool isEndLIne(char ch)
{
  return ch == '\n';
}

/*
 * Token
 */

class Token
{
public:
  Tag tag;
  string value;
  int lineIndex;
  bool operator=(const Token &t);
  Token();
  Token(Tag tag, string value, int lineIndex);
  Token(const Token &t);
};

bool Token::operator=(const Token &t)
{
  tag = t.tag;
  value = t.value;
  lineIndex = t.lineIndex;
  return true;
}

Token::Token()
{
}

Token::Token(Tag tag, string value, int lineIndex)
{
  this->tag = tag;
  this->value = value;
  this->lineIndex = lineIndex;
}

Token::Token(const Token &t)
{
  tag = t.tag;
  value = t.value;
  lineIndex = t.lineIndex;
}

/*
 * Lexer
 */

class Lexer
{
private:
  char nextChar();
  bool readChar(char c);

public:
  State state;
  int lineIndex;
  string code;
  int curIndex;
  char curChar;
  vector<Token> tokenStream;

  void readFile(string path);
  void scan();
  void addToken(Token token);
  void print();
  Lexer();
};

Lexer::Lexer()
{
  lineIndex = 1;
  curIndex = 0;
  state = _START;
  tokenStream.clear();
}

void Lexer::readFile(string path)
{
  FILE *f = fopen(path.c_str(), "rb");
  fpos_t pos;

  if (f == NULL)
    throw new exception();

  fseek(f, 0, SEEK_END);
  fgetpos(f, &pos);
  fseek(f, 0, SEEK_SET);
  char *multiBuffer = new char[(size_t)pos + 1];
  fread(multiBuffer, 1, (size_t)pos, f);
  multiBuffer[(size_t)pos] = '\0';
  fclose(f);

  code = multiBuffer;
  delete[] multiBuffer;
}

char Lexer::nextChar()
{
  if (curIndex < code.length())
  {
    curChar = code[curIndex++];
    return curChar;
  }
  else
    return '\0';
}

bool Lexer::readChar(char c)
{
  nextChar();
  if (curChar != c)
    return false;
  curChar = ' ';
  return true;
}

void Lexer::addToken(Token token)
{
  tokenStream.push_back(token);
}

void Lexer::scan()
{
  Token *curToken = new Token;
  string lex = "";

  while (nextChar() != '\0')
  {
    bool lexemEndFlag = false;
    switch (state)
    {
    case _START:
      if (isWhitespace(curChar))
      {
        if (isEndLIne(curChar))
          lineIndex++;
        continue;
      }
      if (curChar == '\'')
      {
        state = _STRING;
        continue;
      }
      if (isNum(curChar))
      {
        state = _INT;
      }
      else if (isLetter(curChar))
      {
        state = _IDENT;
      }
      else
      {
        switch (curChar)
        {
        case ';':
          curToken->tag = SEMI;
          lexemEndFlag = true;
          break;

        case '(':
          curToken->tag = OPEN_BRACKET;
          lexemEndFlag = true;
          break;

        case ')':
          curToken->tag = CLOSE_BRACKET;
          lexemEndFlag = true;
          break;

        case ',':
          curToken->tag = COMMA;
          lexemEndFlag = true;
          break;

        case '.':
          curToken->tag = FINISH;
          lexemEndFlag = true;
          break;

        case ':':
          switch (code.at(curIndex))
          {
          case '=':
            lex.append(&curChar);
            curToken->tag = BIND;
            lexemEndFlag = true;
            nextChar();
            break;
          default:
            curToken->tag = COLON;
            lexemEndFlag = true;
            break;
          }
          break;
        case '&':
          curToken->tag = CONCAT;
          lexemEndFlag = true;
          break;
        case '+':
          curToken->tag = ADD;
          lexemEndFlag = true;
          break;
        case '-':
          curToken->tag = SUB;
          lexemEndFlag = true;
          break;
        case '*':
          curToken->tag = MUL;
          lexemEndFlag = true;
          break;
        case '/':
          curToken->tag = DIV;
          lexemEndFlag = true;
          break;
        case '<':
          switch (code.at(curIndex))
          {
          case '=':
            lex.append(&curChar);
            curToken->tag = LE;
            lexemEndFlag = true;
            nextChar();
            break;
          case '>':
            lex.append(&curChar);
            curToken->tag = UNEQ;
            lexemEndFlag = true;
            nextChar();
            break;
          default:
            curToken->tag = LT;
            lexemEndFlag = true;
            break;
          }
          break;
        case '>':
          switch (code.at(curIndex))
          {
          case '=':
            lex.append(&curChar);
            curToken->tag = GE;
            lexemEndFlag = true;
            nextChar();
            break;
          default:
            lexemEndFlag = true;
            curToken->tag = GT;
            break;
          }
          break;
        case '=':
          curToken->tag = EQ;
          lexemEndFlag = true;
          break;
        }
      }
      lex.append(&curChar);
      break;
    case _INT:
      if (curChar == '.')
      {
        lex.append(&curChar);
        state = _FLOAT;
      }
      else if (isNum(curChar))
      {
        lex.append(&curChar);
      }
      else if (isDelimeter(curChar) || isWhitespace(curChar))
      {
        lexemEndFlag = true;
      }
      break;
    case _FLOAT:
      if (isNum(curChar))
      {
        lex.append(&curChar);
      }
      else if (isDelimeter(curChar) || isWhitespace(curChar))
      {
        lexemEndFlag = true;
      }
      break;
    case _STRING:
      if (curChar != '\'')
      {
        lex.append(&curChar);
        break;
      }
      lexemEndFlag = true;
      break;
    case _IDENT:
      if (isLetterOrNum(curChar))
      {
        lex.append(&curChar);
      }
      else if (isDelimeter(curChar) || isWhitespace(curChar))
      {
        lexemEndFlag = true;
      }
      break;

    default:
      break;
    }
    if (lexemEndFlag)
    {
      switch (state)
      {
      case _INT:
        curToken->tag = INT;
        curIndex--;
        break;
      case _FLOAT:
        curToken->tag = FLOAT;
        curIndex--;
        break;
      case _STRING:
        curToken->tag = STRING;
        break;
      case _IDENT:
        if (isKeyword(lex))
        {
          curToken->tag = keywords.at(lex);
        }
        else
          curToken->tag = IDENT;
        curIndex--;
        break;
      default:
        break;
      }
      state = _START;
      curToken->value = lex;
      curToken->lineIndex = lineIndex;
      lex = "";
      addToken(*curToken);
    }
  }
  delete curToken;
}

void Lexer::print()
{
  for (auto i : tokenStream)
  {
    cout << "| line: " << i.lineIndex << ", \tid: " << getHumanReadableTag(i.tag) << ", \tlexeme: " << i.value << endl;
  }
}
