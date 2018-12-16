#include "Well.h"
#include "UserInterface.h"



//comment out later
//#include <iostream>

void Well::display(Screen& screen, vector<vector<char> >& b, int x, int y) const
{   //passes in 0, 0
    //displays the well with walls (@) in appropriate places
    for (int c = 0; c < 12; c++) {
        for (int r = 0; r < 19; r++) {
            screen.gotoXY(x + c, y + r);
            if (c == x + 0 || c == x + 11 || r == y + 18) {
                screen.printChar('@');
                b[r][c] = '@';
            } else {
                screen.printChar(' ');
            }
        }
    }
    
}

