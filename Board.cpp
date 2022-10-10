//Bill Tran UID: 505604257
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:


      struct Ship               //To organize each ship type with ID,Length, and Symbol
      {
          int ship_id;
          int ship_length;
          char ship_symbol;
      };

    char boardarr[MAXROWS][MAXCOLS];      //Creating an array to save points in

    vector<Ship> ShipsVector;               //Make a vector to store different types of ships

    const Game& m_game;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    //Filling up the game board array with '.' chars

    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int k = 0; k < m_game.cols(); k++)
        {
            boardarr[i][k] = '.';
        }
    }
}

void BoardImpl::clear()
{   
//Resets the board to same as when first constructed.
    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int k = 0; k < m_game.cols(); k++)
        {
            boardarr[i][k] = '.';
        }
    }
}

void BoardImpl::block()
{
    // TODO:  Replace this with code to block half of the cells on the board

    //50% Chance to block cell
    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int k = 0; k < m_game.cols(); k++)
        {
            if (randInt(2) == 0)
            {
                boardarr[i][k] = '#';
            }

        }
    }

}

void BoardImpl::unblock()
{
    // TODO:  Replace this with code to unblock all blocked cells

    for (int i = 0; i < m_game.rows(); i++)
    {
        for (int k = 0; k < m_game.cols(); k++)
        {
            //If point is blocked set to unblocked char
            if(boardarr[i][k] == '#')       
                boardarr[i][k] = '.';
        }
    }

}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
//Validation Checks

    if (shipId >= m_game.nShips() || shipId < 0)    //shipId should be within number of ships and nonnegative
        return false; 

    if (m_game.isValid(topOrLeft) == 0)
        return false;

    for (int i = 0; i < m_game.rows(); i++)    //Check if there is still this type of ship on the board 
    {
        for (int k = 0; k < m_game.cols(); k++)
        {
            if (boardarr[i][k] == m_game.shipSymbol(shipId))
                return false ;
        }
    }

//For Vertical placement

    if (dir == VERTICAL)
    {
        int i = topOrLeft.r;
        while (i < (topOrLeft.r + m_game.shipLength(shipId)))         //While the ship fits where placed
        {
            if (i >= m_game.rows())                                   //Checks if the whole ship fits
                return false;

            if (boardarr[i][topOrLeft.c] != '.')        //Checks if spaces are blocked
                return false;

            i++;
        }

        int k = topOrLeft.r;
        while (k < (topOrLeft.r + m_game.shipLength(shipId)))       //Actually placing the ship
        {
            boardarr[k][topOrLeft.c] = m_game.shipSymbol(shipId);
            k++;
        }

        Ship ToSave;                                    //Saving shiptype info into shipvector
        ToSave.ship_id = shipId;
        ToSave.ship_symbol = m_game.shipSymbol(shipId);
        ToSave.ship_length = m_game.shipLength(shipId);
        ShipsVector.push_back(ToSave);
    }

//For Horizontal placement

    if (dir == HORIZONTAL)
    {
        int i = topOrLeft.c;
        while (i < (topOrLeft.c + m_game.shipLength(shipId)))         //While the ship fits where placed
        {
            if (i >= m_game.cols())                                   //Checks if the whole ship fits
                return false;

            if (boardarr[topOrLeft.r][i] != '.')        //Checks if spaces are blocked
                return false;

            i++;
        }

        int k = topOrLeft.c;
        while (k < (topOrLeft.c + m_game.shipLength(shipId)))       //Actually placing the ship
        {
            boardarr[topOrLeft.r][k] = m_game.shipSymbol(shipId);
            k++;
        }

        Ship ToSave;                                    //Saving shiptype info into shipvector
        ToSave.ship_id = shipId;
        ToSave.ship_symbol = m_game.shipSymbol(shipId);
        ToSave.ship_length = m_game.shipLength(shipId);
        ShipsVector.push_back(ToSave);
    }

    return true;

}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId >= m_game.nShips() || shipId < 0)    //shipId should be within number of ships and nonnegative
        return false;

    if (m_game.isValid(topOrLeft) == 0)
        return false;

//For Vertical unplacement

    if (dir == VERTICAL)
    {
        int i = topOrLeft.r;
        if ((i + m_game.shipLength(shipId)) > m_game.rows())
            return false;

        i = topOrLeft.r;
        for (; i < (topOrLeft.r + m_game.shipLength(shipId)); i++)         //While the ship fits where placed
        {
            if (boardarr[i][topOrLeft.c] != m_game.shipSymbol(shipId))                                   //Checks if the whole ship fits
                return false;
        }

        for (int k = 0; k < ShipsVector.size(); k++)    //erases this ship type from ShipsVector essentially
        {
            if (ShipsVector[k].ship_id == shipId)
            {
                ShipsVector[k].ship_symbol = '.';
                ShipsVector[k].ship_length = -1;
                ShipsVector[k].ship_id = -1;
            }
        }

        i = topOrLeft.r;
        for (; i < topOrLeft.r + m_game.shipLength(shipId); i++)
        {
            boardarr[i][topOrLeft.c] = '.';
        }
    }

//For Horizontal unplacement

    if (dir == HORIZONTAL)
    {
        int i = topOrLeft.c;
        if ((i + m_game.shipLength(shipId)) > m_game.rows())
            return false;

        i = topOrLeft.c;
        for (; i < (topOrLeft.c + m_game.shipLength(shipId)); i++)         //While the ship fits where placed
        {
            if (boardarr[topOrLeft.r][i] != m_game.shipSymbol(shipId))                                   //Checks if the whole ship fits
                return false;
        }

        for (int k = 0; k < ShipsVector.size(); k++)    //erases this ship type from ShipsVector essentially
        {
            if (ShipsVector[k].ship_id == shipId)
            {
                ShipsVector[k].ship_symbol = '.';
                ShipsVector[k].ship_length = -1;
                ShipsVector[k].ship_id = -1;
            }
        }

        i = topOrLeft.c;
        for (; i < topOrLeft.c + m_game.shipLength(shipId); i++)
        {
            boardarr[topOrLeft.r][i] = '.';
        }
    }
    return true; 
}

void BoardImpl::display(bool shotsOnly) const
{
    //diplaying top left corner space between row and column labels
    cout << "  ";   

    //diplaying all row numbers 
    for (int k = 0; k < m_game.cols(); k++)
    {
        cout << k;
    }

    cout << endl; 

    int i = 0;
    for (; i < m_game.rows(); i++)
    {
        //diplaying all column numbers before the line
        cout << i << " ";
        int k = 0;
        for (; k < m_game.cols(); k++)
        {
            if (shotsOnly == 0)
            {
                cout << boardarr[i][k];
            }

            else
            {
                //diplaying all '.'
                if (boardarr[i][k] != '.' && boardarr[i][k] != 'X' && boardarr[i][k] != 'o')
                {
                    cout << '.';
                }
                //diplaying all other characters
                else
                {
                    cout << boardarr[i][k];
                }
            }
        }

        cout << endl;

    }

}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if (m_game.isValid(p) == 0)
    {
        return false;
    }

    char TempPoint = boardarr[p.r][p.c];
    if (TempPoint == 'o' || TempPoint == 'X')
    {
        return false;
    }

    if (TempPoint != '.')
    {
        int TempShot = -1;
        int k = 0;
        for (; k < ShipsVector.size(); k++)
        {
            if (ShipsVector[k].ship_symbol == TempPoint)
            {
                TempShot = k;

                ShipsVector[k].ship_length = ShipsVector[k].ship_length - 1;
            }
        }

        boardarr[p.r][p.c] = 'X';
        shotHit = 1;
        if (ShipsVector[TempShot].ship_length == 0)
        {
            shipDestroyed = 1;
            shipId = ShipsVector[TempShot].ship_id;
        }

    }

    else
    {
        boardarr[p.r][p.c] = 'o';
        shotHit = 0;
        shipDestroyed = 1;
    }

    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    int k = 0;
    for (; k < ShipsVector.size(); k++)
    {
        if (ShipsVector[k].ship_length > 0)
        {
            return false;
        }
    }

    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
