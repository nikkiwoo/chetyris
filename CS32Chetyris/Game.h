#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Well.h"
#include "UserInterface.h"
#include <string>
// [Add other #include directives as necessary.]
#include "Piece.h"
//#include <iostream>
//#include <iomanip>

class Game
{
  public:
    Game();
    void play();
    bool playOneLevel(vector<vector<char> > b);
    void displayPrompt(std::string s);
    void displayStatus();
    // [Add other members as necessary.]
    bool callPiece(vector<vector<char> >& b, BBPiece& p, int x, int y, int tt, bool& q);
    bool rowsfilled(vector<vector<char> >& b, int& filledindex) const;
    void deleterows(vector<vector<char> >& b, int& index);
    void refreshScore(int rowdel);
    void clearScreen();
    

  private:
    Well    m_well;
    Screen  m_screen;
    int     m_level;
    // [Add other members as necessary.]
    int m_score;
    int m_rowsleft;
};

#endif // GAME_INCLUDED
