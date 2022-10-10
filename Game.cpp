//Bill Tran UID: 505604257
#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int m_rows, m_cols;

    struct Ship {
        int ship_length;
        char ship_symbol;
        string ship_name;
    };

    vector<Ship> ShipsVector;

};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    Ship toAdd;
    toAdd.ship_length = length;
    toAdd.ship_symbol = symbol;
    toAdd.ship_name = name;
    ShipsVector.push_back(toAdd);
    return true;
}

int GameImpl::nShips() const
{
    return ShipsVector.size();
}

int GameImpl::shipLength(int shipId) const
{
    return ShipsVector[shipId].ship_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return ShipsVector[shipId].ship_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return ShipsVector[shipId].ship_name;  
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if (p1->placeShips(b1) == 0 || p2->placeShips(b2) == 0)
        return nullptr;
    bool HitShip;
    bool DestroyedShip;
    bool boolTemp;
    int shipID = 999;
    while ((b1.allShipsDestroyed()) == 0 || (!b2.allShipsDestroyed()) == 0)
    {
        HitShip = false;
        DestroyedShip = false;

        cout << p1->name() << "'s turn.  Board for " << p2->name() << ":" << endl;

        b2.display(p1->isHuman());

        Point P1AttackRec = p1->recommendAttack();

        boolTemp = b2.attack(P1AttackRec, HitShip, DestroyedShip, shipID);
        p1->recordAttackResult(P1AttackRec, boolTemp, HitShip, DestroyedShip, shipID);
        p2->recordAttackByOpponent(P1AttackRec);
        if (boolTemp == 0)
            cout << p1->name() << " wasted a shot at " << P1AttackRec.r << "," << P1AttackRec.c << ")." << endl;
        else if (HitShip)
        {
            if (DestroyedShip == true)
            {
                cout << p1->name() << " attacked" << " (" << P1AttackRec.r << "," << P1AttackRec.c << ") and destroyed the " << shipName(shipID) << ", resulting in:" << endl;
                b2.display(p1->isHuman());
            }
            else
            {
                cout << p1->name() << " attacked" << " (" << P1AttackRec.r << "," << P1AttackRec.c << ") and hit something, resulting in:" << endl;
                b2.display(p1->isHuman());
            }
        }
        else
        {
            cout << p1->name() << " attacked" << " (" << P1AttackRec.r << "," << P1AttackRec.c << ") and missed, resulting in:" << endl;
            b2.display(p1->isHuman());
        }

        if (b2.allShipsDestroyed() == 1)
        {
            cout << p1->name() << " wins!" << endl;
            if (p2->isHuman())
            {
                cout << "Here's where " << p1->name() << "'s ships were:" << endl;
                b1.display(false);
            }
            return p1;
        }

        if (shouldPause)
        {
            waitForEnter();
        }

        HitShip = false;
        DestroyedShip = false;
        boolTemp = false;

        cout << p2->name() << "'s turn.  Board for " << p1->name() << ":" << endl;

        b1.display(p2->isHuman());

        Point P2AttackRec = p2->recommendAttack();

        boolTemp = b1.attack(P2AttackRec, HitShip, DestroyedShip, shipID);
        p2->recordAttackResult(P2AttackRec, boolTemp, HitShip, DestroyedShip, shipID);
        p1->recordAttackByOpponent(P2AttackRec);


        if (boolTemp == 0)
            cout << p2->name() << " wasted a shot at " << P2AttackRec.r << "," << P2AttackRec.c << ")." << endl;
        else if (HitShip)
        {
            if (DestroyedShip == true)
            {
                cout << p2->name() << " attacked" << " (" << P2AttackRec.r << "," << P2AttackRec.c << ") and destroyed the " << shipName(shipID) << ", resulting in:" << endl;
                b1.display(p1->isHuman());
            }
            else
            {
                cout << p2->name() << " attacked" << " (" << P2AttackRec.r << "," << P2AttackRec.c << ") and hit something, resulting in:" << endl;
                b1.display(p1->isHuman());
            }
        }
        else
        {
            cout << p2->name() << " attacked" << " (" << P2AttackRec.r << "," << P2AttackRec.c << ") and missed, resulting in:" << endl;
            b1.display(p2->isHuman());
        }

        if (b1.allShipsDestroyed() == 1)
        {
            cout << p2->name() << " wins!" << endl;
            if (p2->isHuman())
            {
                cout << "Here's where " << p2->name() << "'s ships were:" << endl;
                b2.display(false);
            }
            return p2;
        }

        if (shouldPause)
        {
            waitForEnter();
        }
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

