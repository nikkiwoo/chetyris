#include "Piece.h"

//    PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
//    PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES
//bounding box overlap- how to test if its viable for a piece to fall in a certain area


BBPiece::BBPiece() {
    //initializes to defualt values
    m_vector = vector<vector<char> > (4, vector<char> (4, ' '));    //bounding box of piece
    type = NUM_PIECE_TYPES;
    orientation = 0;    //orientation- used later to rotate
}

PieceType BBPiece::ptype() const {
    return type;
}

BBPiece& BBPiece::operator=(const BBPiece& og) {
    orientation = og.orientation;
    type = og.type;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m_vector[i][j] = og.m_vector[i][j];
        }
    }
    return *this;
}

//creates piece based on enumerated value
void BBPiece::initializepiece(PieceType rand) {
    //sets type to rand, clears out m_vector of piece and reinitializes m_vector so that the blocks correspond to the set up of piecetype rand
    type = rand;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m_vector[i][j] = ' ';
        }
    }
    switch(rand) {
        case PIECE_I:
            //b = new ipiece(v);
            for (int i = 0; i < 4; i++) {
                m_vector[1][i] = '#'; //row by column
            }
            break;
        case PIECE_L:
            //initialize piece
            for (int i = 0; i < 3; i++) {
                m_vector[1][i] = '#';
            }
            m_vector[2][0] = '#';
            break;
        case PIECE_J:
            //initialize piece
            for (int i = 1; i < 4; i++) {
                m_vector[1][i] = '#';
            }
            m_vector[2][3] = '#';
            break;
        case PIECE_T:
            //initialize piece
            for (int i = 0; i < 3; i++) {
                m_vector[1][i] = '#';
            }
            m_vector[0][1] = '#';
            break;
        case PIECE_O:
            //initialize piece
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    m_vector[i][j] = '#';
                }
            }
            break;
        case PIECE_S:
            //initialize piece
            for (int i = 0; i < 2; i++) {
                m_vector[2][i] = '#';
            }
            m_vector[1][1] = '#';
            m_vector[1][2] = '#';
            break;
        case PIECE_Z:
            //initialize piece
            m_vector[1][0] = '#';
            m_vector[1][1] = '#';
            m_vector[2][1] = '#';
            m_vector[2][2] = '#';
            break;
        case PIECE_VAPOR:
            //initialize piece
            m_vector[0][1] = '#';
            m_vector[0][2] = '#';
            break;
        case PIECE_FOAM:
            //initialize piece
            m_vector[1][1] = '#';
            break;
        case PIECE_CRAZY:
            //initialize piece
            m_vector[0][0] = '#';
            m_vector[1][1] = '#';
            m_vector[1][2] = '#';
            m_vector[0][3] = '#';
            break;
        case NUM_PIECE_TYPES:
            //do nothing
            break;
    }
}

void BBPiece::display(Screen& s, int x, int y) const {
    for (int i = x; i < 4 + x; i++) {
        for (int j = y; j < 4 + y; j++) {
            if (m_vector[j - y][i - x] == '#') {
                s.gotoXY(i, j); //displays piece onto screen
                s.printChar('#');
            }
        }
    }
    
}

void BBPiece::printNextPiece(Screen& s, int x, int y) const {
    //prints out the piece under the "next piece" display on the screen
    for (int i = x; i < x+4; i++) {
        for (int j = y; j < 4 + y; j++) {
            s.gotoXY(i, j);
            s.printChar(' ');
        }
    }
    for (int i = x; i < x + 4; i++) {
        for (int j = y; j < 4 + y; j++) {
            if (m_vector[j - y][i - x] == '#') {
                s.gotoXY(i, j);
                s.printChar('#');
            }
        }
    }
}


void BBPiece::erasepiece(Screen& s, int x, int y) const {
    //erases piece from screen based on where the '#' are in the piece
    for (int i = x; i < 4 + x; i++) {
        for (int j = y; j < 4 + y; j++) {
            if (m_vector[j - y][i - x] == '#') {
                s.gotoXY(i, j);
                s.printChar(' ');
                
            }
        }
    }
}




bool BBPiece::isValidDown(vector<vector<char> >& b, int x, int y) const {
    //checks if it is valid for the piece to move down one row on the current screen
    for (int j = y; j < y + 4; j++) {
        for (int i = x; i < x + 4; i++) {
            if (m_vector[j - y][i - x] != ' ' && b[j][i] != ' ') {
                return false;
            }
        }
    }

    return true;
}


bool BBPiece::isValidRight(vector<vector<char> >& b, int x, int y) const {
    //checks if it is valid for the piece to move right one column on the current screen
    for (int i = x; i < x + 4; i++) {
        for (int j = y; j < y + 4; j++) {
            if (m_vector[j - y][i - x] == '#' && (i + 1) <= 11) {
                if (b[j][i + 1] != ' ') {
                    return false;
                }
            }
        }
    }

    return true;
}

bool BBPiece::isValidLeft(vector<vector<char> >& b, int x, int y) const {
    //checks if it is valid for the piece to move left one column on the current screen
    for (int i = x; i < x + 4; i++) {
        for (int j = y; j < y + 4; j++) {
            if (m_vector[j - y][i - x] == '#' && (i - 1) >= 0) {
                if (b[j][i - 1] != ' ') {
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool BBPiece::isValidRotate(Screen& s, vector<vector<char> > & b, int x, int y) {
    vector<vector<char> > temp (4, vector<char>(4, ' '));   //temporary vectorvectorchar to check if can validly rotate m_vector
    if (type == PIECE_O || type == PIECE_VAPOR || type == PIECE_FOAM) {
        return true;
    }
    for (int xbb = 0; xbb < 4; xbb++) {
        for (int ybb = 0; ybb < 4; ybb++) {
            temp[ybb][xbb] = m_vector[3 - xbb][ybb];    //rotates temp vvc clockwise
        }
    }
    if(type == PIECE_CRAZY) {
        //crazy shape clockwise do nothing beyond the clockwise rotation
    } else if (type == PIECE_I) {   //rotation of i piece depends on orientation
        //if orientation is odd, subsequent orientation does not shift row
        if (orientation % 2 == 0) {
            //if orientation is even, subsequent orientation shifts left one column
            for (int r = 0; r < 4; r++) {
                temp[r][1] = '#';
                temp[r][2] = ' ';
            }
        }
        
    } else if (type == PIECE_L) {   //rotation of l piece depends on orientation
        //shift left one row if going from orientation 1 to 2
        if (orientation == 1) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    if (j == 3) {
                        temp[k][j] = ' ';
                    } else {
                        temp[k][j] = temp[k][j+1];
                    }
                    
                }
            }
        }
        //shift down one if going from orientation 2 to 3
        if (orientation == 2) {
            for (int j = 0; j < 4; j++) {
                for (int k = 3; k >= 0; k--) {
                    if (k == 0) {
                        temp[k][j] = ' ';
                    } else {
                        temp[k][j] = temp[k - 1][j];
                    }
                    
                }
            }
        }
    } else if (type == PIECE_J) {
        //rotation of j piece depends on orientation
        //shift right one if going from orientation 1 to 2
        if (orientation == 1) {
            for (int j = 3; j >= 0; j--) {
                for (int k = 0; k < 4; k++) {
                    if (j == 0) {
                        temp[k][j] = ' ';
                    } else {
                        temp[k][j] = temp[k][j-1];
                    }
                }
            }
        }
        //shift up one if going from orientation 2 to 3
        if (orientation == 2) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    if (k == 3) {
                        temp[k][j] = ' ';
                    } else {
                        temp[k][j] = temp[k+1][j];
                    }
                }
            }
        }
        
    } else if (type == PIECE_T) {
        //rotation of t piece
        //shift left one row every time
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    if (j == 3) {
                        temp[k][j] = ' ';
                    } else {
                        temp[k][j] = temp[k][j+1];
                    }
                    
                }
            }
        
        
    } else if (type == PIECE_S || type == PIECE_Z) {
        //rotation of s and z piece depends on orientation
        if (orientation % 2 == 1) { //shift left
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    if (j == 3) {
                        temp[k][j] = ' ';
                    } else {
                        temp[k][j] = temp[k][j+1];
                    }
                    
                }
            }
        }
        
    }
    
    
    //check to see if rotated m_vector clashes with anything on current vvc board
    for (int i = x; i < x + 4; i++) {
        for (int j = y; j < y + 4; j++) {
            if (temp[j - y][i - x] != ' ' && b[j][i] != ' ') {
                return false;
            }
            
        }
    }
    this->erasepiece(s, x, y);
    m_vector = temp;    //if no clashing, displays rotated piece onto screen
    orientation++;  //increments rotation
    if (orientation == 4) {
        orientation = 0;
    }
    return true;
}


bool BBPiece::isGameOver(vector<vector<char> >& b, Screen& s) const {
    //checks if initializing the current piece onto the board as it is will clash with already existing piece --> symbolizes game being over
    for (int j = 0; j < 4; j++) {
        for (int i = 3; i < 7; i++) {
            if (m_vector[j][i - 3] != ' ' && b[j][i] != ' ') {
                this->display(s, 3, 0);
                return true;
            }
        }
    }
    return false;
}

void BBPiece::foamy(Screen& s, vector<vector<char> >& b, int x, int y, int rightbound, int downbound) const {    //pass in variables that define bounds
    
    b[y][x] = '*';  //sets positio passed in on the screen and the 2d vector representing the board to *
    s.gotoXY(x, y);
    s.printChar('*');
    //right adjacent
    if ((x+1)<= rightbound && (x+1) <= 11) {    //checks that the right adjacent piece doesn't violate the rightmost bound or go outside of the well wall
        if (b[y][x+1] == ' ') {
            this->foamy(s, b, x+1, y, rightbound, downbound);   //uses recursion
        }
    }
    //downadjacent
    if ((y+1)<= downbound && (y+1) <= 18) { //checks that the down adjacent piece doesn't violate the downmost bound or go outside of the well wall
        if (b[y+1][x] == ' ') {
            this->foamy(s, b, x, y+1, rightbound, downbound);   //recursion
        }
    }
    //left adjacent
    if ((x-1)>= rightbound - 4 && (x-1) >= 1) { //checks that the left adjacent piece doesn't violate the leftmost bound or go outside of the well wall
        if (b[y][x-1] == ' ') {
            this->foamy(s, b, x-1, y, rightbound, downbound);   //recursion
        }
    }
    //downadjacent
    if ((y-1)>= downbound - 4 && (y-1) >= 0) {  //checks that the upper adjacent piece doesn't violate the uppermost bound or go outside of the well wall
        if (b[y-1][x] == ' ') {
            this->foamy(s, b, x, y-1, rightbound, downbound);   //recursion
        }
    }
    
    
}

void BBPiece::vapor(Screen& s, vector<vector<char> >& b, int x, int y) const {
        //deletes two rows above and below vapor bomb in the two colums defined in the vapor bomb if piece fragments are present
    for (int i = x + 1; i < x + 3; i++) {
        for (int j = y - 2; j < y + 3; j++) {
            if (j >= 0 && j < 18) {
                if (b[j][i] != ' ') {
                    b[j][i] = ' ';
                    s.gotoXY(i, j);
                    s.printChar(' ');
                }
            }
        }
    }
}

void BBPiece::cash(Screen& s, vector<vector<char> >& b, int x, int y) const {
    //turns # to $ - meaning piece is permantly on board (no longer falling)
    for (int i = x; i < 4 + x; i++) {
        for (int j = y; j < 4 + y; j++) {
            if (m_vector[j - y][i - x] == '#') {
                s.gotoXY(i, j);
                s.printChar('$');
                b[j][i] = '$';  //add to board vector to be able to check later
            }
        }
    }
}

