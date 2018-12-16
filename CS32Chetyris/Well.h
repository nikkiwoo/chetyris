#ifndef WELL_INCLUDED
#define WELL_INCLUDED

#include <vector>
using namespace std;
class Screen;

class Well
{
  public:
    void display(Screen& screen, vector<vector<char> >& b, int x, int y) const;
    
};

#endif // WELL_INCLUDED
