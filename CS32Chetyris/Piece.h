#ifndef PIECE_INCLUDED
#define PIECE_INCLUDED

#include "UserInterface.h"
//
#include <algorithm>
#include <vector>
using namespace std;

enum PieceType {
    PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
    PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES
};

PieceType chooseRandomPieceType();


//class for bounding box and piece
class BBPiece {
public:
    //constructor --> intializes empty vector vector char
    BBPiece();
    //returns piecetype
    PieceType ptype() const;
    //assignment operator
    BBPiece& operator=(const BBPiece& copy);
    //prints out nextpiece onto screen
    void printNextPiece(Screen& s, int x, int y) const;
    //creates piece based on random piece type generated
    void initializepiece(PieceType rando);
    //display the vector vector char for appropriate pieces
    void display(Screen& s, int x, int y) const;
    //erase the piece from screen and vector
    void erasepiece(Screen& s, int x, int y) const;
    //check if valid to move piece down (aka no intersections)
    bool isValidDown(vector<vector<char> >& b, int x, int y) const;
    //check if valid to move piece right (aka no intersections)
    bool isValidRight(vector<vector<char> >& b, int x, int y) const;
    //check if valid to move piece left (aka no intersections)
    bool isValidLeft(vector<vector<char> >& b, int x, int y) const;
    //checks to see if piece is valid to rotate
    bool isValidRotate(Screen& s,vector<vector<char> > & b, int x, int y);
    //check if the game is over (if displaying the piece will result in conflict
    bool isGameOver(vector<vector<char> >& b, Screen& s) const;
    //explodes foam piece
    void foamy(Screen& s, vector<vector<char> >& b, int x, int y, int rightbound, int downbound) const;
    //explodes vapor bomb
    void vapor(Screen& s, vector<vector<char> >& b, int x, int y) const;
    //turns piece from '#' to '$' and adds to board vectorvectorchar
    void cash(Screen& s, vector<vector<char> >& b, int x, int y) const;
    

private:
    vector<vector<char> > m_vector;
    PieceType type;
    int orientation;

};


#endif // PIECE_INCLUDED
