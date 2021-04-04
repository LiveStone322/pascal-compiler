#include <vector>
using namespace std;

template <typename T>
int findInVector(vector<T> v, T e)
{
  auto it = find(v.begin(), v.end(), e);
  if (it == v.end())
  {
    return -1;
  }
  return it;
}