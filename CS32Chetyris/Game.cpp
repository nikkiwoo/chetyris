#include "Game.h"
#include "Well.h"
#include "UserInterface.h"
#include <string>
//
#include "Piece.h"
#include <stdlib.h>

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

const int WELL_X = 0;
const int WELL_Y = 0;

const int PROMPT_Y = 20;
const int PROMPT_X = 0;

const int SCORE_X = 16;
const int SCORE_Y = 8;

const int ROWS_LEFT_X = 16;
const int ROWS_LEFT_Y = 9;

const int LEVEL_X = 16;
const int LEVEL_Y = 10;

const int IBB_X = 4;
const int IBB_Y = 0;

const int NEXT_PIECE_TITLE_X = 16;
const int NEXT_PIECE_TITLE_Y = 3;

const int NEXT_PIECE_X = 16;
const int NEXT_PIECE_Y = 4;

Game::Game()
 : m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1), m_score(0), m_rowsleft(5*m_level)
{
}

void Game::play()
{
    vector<vector<char> > board (19, vector<char> (12, ' '));   //initializes a 2d vector of chars set to ' ' to be used as the board tracker for the game
    m_well.display(m_screen, board, WELL_X, WELL_Y);    //displays the well
    displayStatus();  //  score, rows left, level
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // [in UserInterface.h]

    for(;;)
    {
        if ( ! playOneLevel(board))
            break;
        //reinitialize score, rows left, etc
        displayPrompt("Good job!  Press the Enter key to start next level!");
        waitForEnter();
        m_level++;
        m_rowsleft = 5*m_level;
        clearScreen();  //clears screen for new level
        displayStatus();    //updates level, score, etc
    }
    displayPrompt("Game Over!  Press the Enter key to exit!");  //reached if playOneLevel returns false (aka player fails to complete level)
    waitForEnter();
}

void Game::displayPrompt(std::string s)     
{
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
    m_screen.refresh();
}

void Game::displayStatus()
{
    //  score, rows left, level
    //  the current score (0), the number of remaining rows (5), and the current level (1).
    m_screen.gotoXY(SCORE_X, SCORE_Y);
    m_screen.printStringClearLine("Score:     ");
    m_screen.justoutput(m_score);
    m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
    m_screen.printStringClearLine("Rows left: ");
    m_screen.justoutput(m_rowsleft);
    m_screen.gotoXY(LEVEL_X, LEVEL_Y);
    m_screen.printStringClearLine("Level:     ");
    m_screen.justoutput(m_level);
    
}


bool Game::playOneLevel(vector<vector<char> > b)
{
    bool q = true;  //boolean to check if the player pressed 'q'
    BBPiece initialpiece;   //initializes two pieces
    BBPiece nextpiece;
    while (m_rowsleft != 0) { //while((level is not complete aka score doesn't meet requirements), score required = m_level*5 rows
        //initialize filledindex = 0, countdestroyed = 0
        int filledindex = 0;    //index of filled row
        int countdestroyed = 0; //number of rows destroyed
        //create initial piece with random (if piece is not default NUM), initialize, call next piece function, initialize next piece
        if (initialpiece.ptype() == NUM_PIECE_TYPES) {
            initialpiece.initializepiece(chooseRandomPieceType());
            
        }
        nextpiece.initializepiece(chooseRandomPieceType());
        
        if(!initialpiece.isGameOver(b, m_screen)) {  //while is not game over (aka piece is able to be displayed)
            m_screen.gotoXY(NEXT_PIECE_TITLE_X, NEXT_PIECE_TITLE_Y);    //displays next piece on screen
            m_screen.printStringClearLine("Next piece:");
            nextpiece.printNextPiece(m_screen, NEXT_PIECE_X, NEXT_PIECE_Y);
            this->callPiece(b, initialpiece, IBB_X, IBB_Y, max(1000-(100*(m_level-1)), 100), q);    //calls piece (allows player interaction)
            if (q == false) {
                return false;   //checks to see if player pressed quit --> game over
            }
            //check if rows are completed and tally score, update rows left and rows destroyed
            while (rowsfilled(b, filledindex)) {
                deleterows(b, filledindex);
                countdestroyed++;
                //m_rowscompleted++;
                if (m_rowsleft > 0) {
                m_rowsleft--;   // check if multiple rows deleted at once!
                }
                m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);  //refreshes rows left
                m_screen.printStringClearLine("Rows left: ");
                m_screen.justoutput(m_rowsleft);
                m_screen.refresh();
            }
            this->refreshScore(countdestroyed); //refreshes score based on how many rows were deleted at once
            initialpiece = nextpiece; //assignment operator for pieces
        } else {
            break;  //breaks out of while loop checking for rows left to be 0 if no more pieces can be validly called (well is filled up)
        }
    }
    if (m_rowsleft == 0) {  //returns true if level successfully completed (no rows left to be completed), false otherwise
        return true;
    }
    return false;
    
}

bool Game::callPiece(vector<vector< char> >& b, BBPiece& p, int x, int y, int tt, bool& q) {
    
    if (p.isValidDown(b, x, y)) {   //checks if valid for piece to move down on screen
        p.display(m_screen, x, y);
        m_screen.refresh(); //refreshes so the piece displays
        Timer t;
        while (t.elapsed() < tt) {  //checks that time elapsed is less than the allocated time from the spec
            //accept keyboard commands
            char ch;
            if (getCharIfAny(ch))
            {
                // There was a character typed; it's now in ch
                switch (ch)
                {
                    case ' ':
                        p.erasepiece(m_screen, x, y);
                        while (p.isValidDown(b, x, y)) {
                            y++;    //shifts pieces down as far as it is valid
                        }
                        p.display(m_screen, x, y - 1);   //displays piece at bottome
                        p.cash(m_screen, b, x, y - 1);  //turns '#' into '$'
                        if (p.ptype() == PIECE_FOAM) {
                            //accounts for foam piece
                            p.foamy(m_screen, b, x + 1, y, x + 3, y + 2);
                        }
                        if (p.ptype() == PIECE_VAPOR) {
                            //accounts for vapor bomb
                            p.vapor(m_screen, b, x, y - 1);
                        }
                        m_screen.refresh();
                        return true;
                        break;
                    case ARROW_LEFT:
                        if (p.ptype() == PIECE_CRAZY) { //accounts for switched commands of crazy piece
                            if (p.isValidRight(b, x, y)) {
                                p.erasepiece(m_screen, x, y);
                                callPiece( b, p, x+1, y, tt - t.elapsed(), q);
                                return true;
                            }
                        } else if (p.isValidLeft(b, x, y)) {    //checks if it is valid for the piece to go left, if so, use recursive call on the function to the left
                            p.erasepiece(m_screen, x, y);
                            callPiece( b, p, x-1, y, tt - t.elapsed(), q);
                            return true;
                        }
                        break;
                    case ARROW_RIGHT:
                        if (p.ptype() == PIECE_CRAZY) { //accounts for crazy piece
                            if (p.isValidLeft(b, x, y)) {
                                p.erasepiece(m_screen, x, y);
                                callPiece( b, p, x-1, y, tt - t.elapsed(), q);
                                return true;
                            }
                        } else if (p.isValidRight(b, x, y)) {   // " "
                            p.erasepiece(m_screen, x, y);
                            callPiece( b, p, x+1, y, tt - t.elapsed(), q);
                            return true;
                        }
                        break;
                    case ARROW_UP:
                        if (p.isValidRotate(m_screen, b, x, y)) {   //checks if valid for the piece to rotate and rotates piece if so
                            callPiece(b, p, x, y, tt - t.elapsed(), q); //recursively calls piece
                            return true;
                        }
                        break;
                    case ARROW_DOWN:
                        p.erasepiece(m_screen, x, y);
                        callPiece(b, p, x, y + 1, max(1000-(100*(m_level-1)), 100), q); //recursively calls piece to move down
                        return true;
                    case 'q':
                    case 'Q':
                        q = false;  //sets q equal to false in order to cause playOneLevel to return false
                        return false;
                        break;
                }
            }

            
        }   //timer expired
        
        p.erasepiece(m_screen, x, y);
        //erase piece from screen according to vector, erase from vector
        m_screen.refresh();
        callPiece(b, p, x, y+1, max(1000-(100*(m_level-1)), 100), q);   //recursively call the piece only slot down again
        
    } else {    //if piece is no longer able to move down on screen
        //put piece at bottom of board and change '#' to '$'
        p.cash(m_screen, b, x, y - 1);
        if (p.ptype() == PIECE_FOAM) {  //special effects for foam and vapor piece when they are placed on the board
            p.foamy(m_screen, b, x + 1, y, x + 3, y + 2);
        }
        if (p.ptype() == PIECE_VAPOR) {
            //vapor bomb
            p.vapor(m_screen, b, x, y - 1);
        }
        m_screen.refresh();
        return true;
    }
    return true;
}

bool Game::rowsfilled(vector<vector<char> >& b, int& filledindex) const {
    for (int i = 0; i < 18; i++) {  //checks if a row is filled
        bool fill = true;
        for (int j = 0; j < 12; j++) {
            if (b[i][j] == ' ') {
                fill = false;
            }
        }
        if (fill) {
            filledindex = i;    // assigns index of filled row to filledindex
            return true;
        }
    }
    return false;   //returns false if no filled rows
}


void Game::deleterows(vector<vector<char> >& b, int& index) {
    //deletes filled row
    for (int j = 1; j < 11; j++) {
        b[index][j] = ' ';
        m_screen.gotoXY(j, index);
        m_screen.printChar(' ');
    }
    //shifts all subsequent rows down
    for (int y = index; y > 0; y--) {
        for (int x = 1; x < 11; x++) {
            char temp = b[y - 1][x];
            b[y][x] = temp;
            m_screen.gotoXY(x, y);
            m_screen.printChar(temp);
        }
    }
    //initialize top row to ' '
    for (int x = 1; x < 11; x++) {
        b[0][x] = ' ';
        m_screen.gotoXY(x, 0);
        m_screen.printChar(' ');
}

}

void Game::refreshScore(int rowdel) {
    //refreshes score based on how many rows are cleared at once
    if (rowdel == 0) {
        return;
    }
    if (rowdel <= 2) {
        m_score += rowdel*100;
    }
    if (rowdel == 3) {
        m_score += 400;
    }
    if (rowdel == 4) {
        m_score += 800;
    }
    if (rowdel == 5) {
        m_score+= 1600;
    }
    m_screen.gotoXY(SCORE_X + 11, SCORE_Y);
    m_screen.justoutput(m_score);
}

void Game::clearScreen() {
    //clears screen to prepare for new level
    for (int i = 1; i < 11; i++) {
        for (int j = 0; j < 18; j++) {
            m_screen.gotoXY(i, j);
            m_screen.printChar(' ');
        }
    }
    
}
