//Bill Tran UID: 505604257
#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
    : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
    // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
    bool /* shotHit */, bool /* shipDestroyed */,
    int /* shipId */)
{
    // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
    // AwfulPlayer completely ignores what the opponent does
}





//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const { return true; }
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();

    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);

private:
    char getDirection(int length);

};

HumanPlayer::HumanPlayer(string nm, const Game& g) //initializes only the player name and game
    : Player(nm, g) {}


//This is a helper function that prompts user to input direction 
//This has to be a separate function due to its use of recursion
char HumanPlayer::getDirection(int shipId)
{
    cout << "Enter h or v for direction of " << game().shipName(shipId) << " (length " << game().shipLength(shipId) << "): ";
    string DirInput = "1";

    getline(cin, DirInput);

    //If the input isn't valid, it runs the function again until it is valid.
    if (DirInput[0] != 'h' && DirInput[0] != 'v')
    {
        cout << "Direction must be h or v." << endl;
        return getDirection(shipId);
    }

    else
    {
        return DirInput[0];
    }
}

bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " must place 5 ships." << endl;

    b.display(0);

    //For each type of ship
    int k = 0;
    for (; k < game().nShips(); k++)
    {

        //Getting human to choose direction and setting direction.
        Direction direction;
        direction = VERTICAL;
        char DirectionCharInput = getDirection(k);

        if (DirectionCharInput == 'v')
            direction = VERTICAL;

        else if (DirectionCharInput == 'h')
            direction = HORIZONTAL;

        //Asking for position of placement
        int row = -1;
        int col = -1;
        cout << "Enter row and column of leftmost cell (e.g., 3 5):";
        bool IsPosValid = getLineWithTwoIntegers(row, col);
        //Setting topOrLeft point with inputted coordinates
        Point topOrleft;
        topOrleft.r = row;
        topOrleft.c = col;
        bool IsPlacedValid = b.placeShip(topOrleft, k, direction);

        //Checks to see if the inputted ints are valid and repeats input process if not.
        while (IsPosValid == 0 || IsPlacedValid == 0)
        {
            if (IsPosValid == 0)
            {
                cout << "You must enter two integers." << endl;
            }
            else
            {
                cout << "The ship can not be placed there." << endl;
            }
            cout << "Enter row and column of leftmost cell (e.g., 3 5):";
            IsPosValid = getLineWithTwoIntegers(row, col);
            topOrleft.r = row;
            topOrleft.c = col;
            IsPlacedValid = b.placeShip(topOrleft, k, direction);
        }

        if (k != game().nShips() - 1)
            b.display(false);

    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    Point p;
    int row = -1;
    int col = -1;

    cout << "Enter the row and column to attack (e.g., 3 5):";

    bool IsPosValid = getLineWithTwoIntegers(row, col);
    while (!IsPosValid)
    {
        cout << "You must enter two integers." << endl;

        cout << "Enter the row and column to attack (e.g., 3 5):";

        IsPosValid = getLineWithTwoIntegers(row, col);
    }
    p.r = row;
    p.c = col;

    return p;
}



void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */, bool /* shotHit */, bool /* shipDestroyed */, int /* shipId */)
{
    // The recordAttackResult and recordAttackByOpponent presumably need not do anything.
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
    // The recordAttackResult and recordAttackByOpponent presumably need not do anything.
}


//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.


class MediocrePlayer : public Player
{
public:
    MediocrePlayer(std::string nm, const Game& g);

    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);

private:
    Point CrossPoint;
    int ValidAttackPoints = 0;
    bool RecursiveHelper(int ID, Board& b);
    int ShipPlaceOrderArray[100];
    int attack_state = 1;
    int SavedShotsBoard[10][10]; //board that records all hits and misses
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g)
{
    int i = 0;
    for (; i < 10; i++)
    {
        int k = 0; 
        for (; k < 10; k++)
        {
            SavedShotsBoard[i][k] = 0;
        }
    }
}

bool MediocrePlayer::RecursiveHelper(int ID, Board& b) //The recursive function that must be used in placeShips
{
    if (ID == game().nShips())  //base case is when the ID is out of range for number of ships
        return true;
    int i = 0;
    for (; i < game().rows(); i++)  //for each column
    {
        int k = 0;
        for (; k < game().cols(); k++) //for each row
        {
            if (b.placeShip(Point(i, k), ShipPlaceOrderArray[ID], HORIZONTAL)) //Checks to see if it can place horizontally successfully
            {
                if (RecursiveHelper(ID + 1, b)) //recursively calls to keep iterating through all ships.
                    return 1;
                else
                    b.unplaceShip(Point(i, k), ShipPlaceOrderArray[ID], HORIZONTAL); //If any of the ships fail to place you have to unplace the rest of them.
            }
            if (b.placeShip(Point(i, k), ShipPlaceOrderArray[ID], VERTICAL)) ////Checks to see if it can place vertically successfully
            {
                if (RecursiveHelper(ID + 1, b)) //recursively calls to keep iterating through all ships.
                    return 1;
                else
                    b.unplaceShip(Point(i, k), ShipPlaceOrderArray[ID], VERTICAL); //If any of the ships fail to place you have to unplace the rest of them.
            }
        }
    }
    //All of them have to return true for this to work so false if any doesn't
    return false;
}

bool MediocrePlayer::placeShips(Board& b)
{

//Makes the ships in order of longest ship to smallest ship so it is easier to place ships.
    int ShipsOrderedCount = 0;
    int i = 10;
    for (; i > 0; i--)
    {
        int k = 0;
        for (; k < game().nShips(); k++)
        {
            if (game().shipLength(k) == i)
            {
                ShipPlaceOrderArray[ShipsOrderedCount] = k;
                ShipsOrderedCount++;
            }
        }
    }
//Has to try to place all ships 50 times per spec.
    for (int i = 0; i < 50; i++) 
    {
        b.block();  // needs to call block function first per spec
        if (RecursiveHelper(0, b)) // using the recursive algorithm stored in an auxiliary function per spec
        {
            b.unblock();
            return true; //if it works, unblock current blocks and return true
        }
        b.unblock(); //else unblock and try again
    }
    return false; //else return false, if unable to place ships after 50 tries
}

void MediocrePlayer::recordAttackByOpponent(Point p) 
{
    //For a MediocrePlayer, this function does nothing. What attacks an opponent has made do not affect a MediocrePlayer's attack decisions.
}

Point MediocrePlayer::recommendAttack()
{
    int row;
    int col;
    if (attack_state == 1) //In state 1, recommendAttack returns a randomly chosen position on the board that has not been chosen before.
    {
        while (true)   // keeps iterating until curent row col values have not been attacked before
        {
            row = randInt(game().rows());
            col = randInt(game().cols());
            if (SavedShotsBoard[row][col] == 0)
                break;
        }
    }
    else //In state 2, let (r,c) be the position that was hit that caused the transition from 1 to state 2.
    {
    //In state 2, recommendAttack returns a randomly chosen position drawn from a limited set:
        while (1) //Iterates until if statement is true.
        {
            if (randInt(2) == 0)//the set of valid positions no more than 4 steps away from(r, c) vertically, that have not been chosen before.
            {
                col = CrossPoint.c + randInt(9) - 4;
                row = CrossPoint.r;
            }
            else //the set of valid positions no more than 4 steps away from(r, c) horizontally, that have not been chosen before.
            {
                col = CrossPoint.c;
                row = CrossPoint.r + randInt(9) - 4;
            }
            if (col < 0 || row < 0 || col >= game().cols() || row >= game().rows())
            {
                continue;
            }
            if (!SavedShotsBoard[row][col])
            {
                ValidAttackPoints = ValidAttackPoints - 1;
                break;
            }
        }
    }
    SavedShotsBoard[row][col] = 1; //Save this shot on the board.

    return Point(row, col);
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId)
{
    if (shotHit) //The attack hits a ship
    {
        if (shipDestroyed) //The attack hits a ship, destroying its final undestroyed segment, so the ship is destroyed.
        {
            attack_state = 1; //In any event, the player switches to state 1.
            return;
        }

        if (attack_state == 1) //if shot didn't destroy anything, and currently in state 1
        {
            CrossPoint = p; //Update the CrossPoint
            ValidAttackPoints = 0;
            for (int n = 0; n < 9; n++) //Include each valid attack point in count.
            {
                if (game().isValid(Point(CrossPoint.r - 4 + n, CrossPoint.c)))
                {
                    if (!SavedShotsBoard[CrossPoint.r - 4 + n][CrossPoint.c])
                        ValidAttackPoints++;
                }
                if (game().isValid(Point(CrossPoint.r, CrossPoint.c - 4 + n)))
                {
                    if (!SavedShotsBoard[CrossPoint.r][CrossPoint.c - 4 + n])
                        ValidAttackPoints++;
                }
            }
            attack_state = 2;
        }

        if (ValidAttackPoints == 0) //If done attack valid points from CrossPoint
        {
            attack_state = 1; //resetting basically
        }
    }
}


//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class GoodPlayer : public Player
{
public:
    GoodPlayer(std::string nm, const Game& g);

    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);

private:
    Point Endpoint1;
    bool End1 = false;
    Point Endpoint2;
    bool End2 = false;
    int turn = 0;
    Direction ShipDir;
    bool Condition1;
    bool boatsize;
    bool RecursiveHelper(int ID, Board& b);
    int ShipPlaceOrderArray[100];
    int attack_state = 1;
    int SavedShotsBoard[10][10]; //board that records all hits and misses

};

GoodPlayer::GoodPlayer(string nm, const Game & g) : Player(nm, g)
{
    int i = 0;
    for (; i < 10; i++)
    {
        int k = 0;
        for (; k < 10; k++)
        {
            SavedShotsBoard[i][k] = 0;
        }
    }
}

bool GoodPlayer::RecursiveHelper(int ID, Board & b) //The recursive function that must be used in placeShips
{
    if (ID == game().nShips())  //base case is when the ID is out of range for number of ships
        return true;
    int i = 0;
    for (; i < game().rows(); i++)  //for each column
    {
        int k = 0;
        for (; k < game().cols(); k++) //for each row
        {
            if (b.placeShip(Point(i, k), ShipPlaceOrderArray[ID], HORIZONTAL)) //Checks to see if it can place horizontally successfully
            {
                if (RecursiveHelper(ID + 1, b)) //recursively calls to keep iterating through all ships.
                    return 1;
                else
                    b.unplaceShip(Point(i, k), ShipPlaceOrderArray[ID], HORIZONTAL); //If any of the ships fail to place you have to unplace the rest of them.
            }
            if (b.placeShip(Point(i, k), ShipPlaceOrderArray[ID], VERTICAL)) ////Checks to see if it can place vertically successfully
            {
                if (RecursiveHelper(ID + 1, b)) //recursively calls to keep iterating through all ships.
                    return 1;
                else
                    b.unplaceShip(Point(i, k), ShipPlaceOrderArray[ID], VERTICAL); //If any of the ships fail to place you have to unplace the rest of them.
            }
        }
    }
    //All of them have to return true for this to work so false if any doesn't
    return false;
}

bool GoodPlayer::placeShips(Board & b)
{

    //Makes the ships in order of longest ship to smallest ship so it is easier to place ships.
    int ShipsOrderedCount = 0;
    int i = 10;
    for (; i > 0; i--)
    {
        int k = 0;
        for (; k < game().nShips(); k++)
        {
            if (game().shipLength(k) == i)
            {
                ShipPlaceOrderArray[ShipsOrderedCount] = k;
                ShipsOrderedCount++;
            }
        }
    }
    //Has to try to place all ships 50 times per spec.
    for (int i = 0; i < 50; i++)
    {
        b.block();  // needs to call block function first per spec
        if (RecursiveHelper(0, b)) // using the recursive algorithm stored in an auxiliary function per spec
        {
            b.unblock();
            return true; //if it works, unblock current blocks and return true
        }
        b.unblock(); //else unblock and try again
    }
    return false; //else return false, if unable to place ships after 50 tries
}

Point GoodPlayer::recommendAttack()
{
    int r = game().rows() / 2;
    int c = game().cols() / 2; 

    if (attack_state == 1) 
    {
        turn = turn + 1;
        int time = 0;
        if (turn < 15) 
        {
            while (SavedShotsBoard[r][c] != 0)
            {
                time = time + 1;
                if (boatsize) 
                {
                    if (randInt(2)) 
                    {
                        c = randInt(game().cols() / 2) * 2 + 1;
                        r = randInt(game().rows() / 4 + game().rows() % 2) * 2;
                    }
                    else
                    {
                        c = randInt(game().cols() / 2 + game().cols() % 2) * 2;
                        r = randInt(game().rows() / 4) * 2 + 1;
                    }
                }
                else if (boatsize == 0) 
                {
                    c = randInt(game().cols());
                    r = randInt(game().rows() / 2);
                }
                if (time == 50)
                {
                    turn = 20;
                    break;
                }
            }
        }
        else if (turn>=15) 
        {
            while (SavedShotsBoard[r][c] != 0) 
            {
                time = time + 1;
                if (boatsize) 
                {
                    if (randInt(2)) 
                    {
                        c = randInt(game().cols() / 2 + game().cols() % 2) * 2;
                        r = randInt(game().rows() / 2) * 2 + 1;
                    }
                    else
                    {
                        c = randInt(game().cols() / 2) * 2 + 1;
                        r = randInt(game().rows() / 2 + game().rows() % 2) * 2;
                    }
                }
                else if (boatsize == 0)
                {
                    c = randInt(game().cols());
                    r = randInt(game().rows());
                }
                if (time == 50)
                {
                    c = randInt(game().cols());
                    r = randInt(game().rows());
                    break;
                }
            }
        }
        return Point(r, c);
    }
    else if (attack_state == 3) 
    {
        if (ShipDir == VERTICAL)
        {
            if (!End1 && game().isValid(Point(Endpoint1.r - 1, Endpoint1.c)))
                if(SavedShotsBoard[Endpoint1.r - 1][Endpoint1.c] == 0)
                    return Point(Endpoint1.r - 1, Endpoint1.c);
            else
                End1 = 1;
            if (!End2 && game().isValid(Point(Endpoint2.r + 1, Endpoint2.c)))
                if (SavedShotsBoard[Endpoint2.r + 1][Endpoint2.c] == 0)
                    return Point(Endpoint2.r + 1, Endpoint2.c);
            else
                End2 = 1;
        }
        else if (ShipDir == HORIZONTAL)
        {

            if (!End1 && game().isValid(Point(Endpoint1.r, Endpoint1.c - 1)))
                if(SavedShotsBoard[Endpoint1.r][Endpoint1.c - 1] == 0)
                    return Point(Endpoint1.r, Endpoint1.c - 1);
            else
                End1 = 1;
            if (!End2 && game().isValid(Point(Endpoint2.r, Endpoint2.c + 1)))
                if(SavedShotsBoard[Endpoint2.r][Endpoint2.c + 1] == 0)
                    return Point(Endpoint2.r, Endpoint2.c + 1);
            else
                End2 = 1;
        }
        attack_state = 2;
        Condition1 = 1;
    }

    if (attack_state == 2)
    {
        if (Condition1)
        {
            int i = 0;
            for (; i < game().rows(); i++)
            {
                int k = 0;
                for (; k < game().cols(); k++)
                {
                    if (SavedShotsBoard[i][k] == 2) 
                    {
                        Endpoint2 = Point(i, k);
                        Endpoint1 = Point(i, k); 
                    }
                }
            }
        }
        int i = 1;
        for (; i < 5; i++) 
        {
            if (game().isValid(Point(Endpoint1.r - i, Endpoint1.c)) && SavedShotsBoard[Endpoint1.r - i][Endpoint1.c] == 0) //looks down
                return Point(Endpoint1.r - i, Endpoint1.c);
            else if (game().isValid(Point(Endpoint1.r, Endpoint1.c + i)) && SavedShotsBoard[Endpoint2.r][Endpoint2.c + i] == 0) //right
                return Point(Endpoint2.r, Endpoint2.c + i);
            else if (game().isValid(Point(Endpoint1.r + i, Endpoint1.c)) && SavedShotsBoard[Endpoint2.r + i][Endpoint2.c] == 0) //up
                return Point(Endpoint2.r + i, Endpoint2.c);
            else if (game().isValid(Point(Endpoint1.r, Endpoint1.c - i)) && SavedShotsBoard[Endpoint1.r][Endpoint1.c - i] == 0) //left
                return Point(Endpoint1.r, Endpoint1.c - i);
        } 

        Condition1 = 1; 
        return Point(randInt(game().rows()), randInt(game().cols()));
    }
    return Point(r, c);
}


void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId)
{
    if (validShot == 0)
    {
        return;
    }
    switch (shotHit)
    {
    case (true) :
        {
            if (shipDestroyed)
            {
                if (p.r != Endpoint1.r)
                    ShipDir = VERTICAL; 
                else
                    ShipDir = HORIZONTAL;

                switch (ShipDir) 
                {
                case (HORIZONTAL) :
                    {
                        if (p.c < Endpoint1.c)
                        {
                            int i = 0; 
                            for (; i < game().shipLength(shipId); i++)
                                SavedShotsBoard[p.r][p.c + i] = 3; 
                        }
                        else
                        {
                            int i = 0;
                            for (; i < game().shipLength(shipId); i++)
                                SavedShotsBoard[p.r][p.c - i] = 3; 
                        }
                    }
                        break;
                case (VERTICAL) :
                    {
                        if (p.r < Endpoint1.r)
                        {
                            int i = 0;
                            for (; i < game().shipLength(shipId); i++)
                                SavedShotsBoard[p.r + i][p.c] = 3; 
                        }
                        else
                        {
                            int i = 0;
                            for (; i < game().shipLength(shipId); i++)
                                SavedShotsBoard[p.r - i][p.c] = 3;
                        }
                    }
                        break;
                }

                if (Condition1)
                {
                    int i = 0;
                    bool tempbool = true;
                    for (; i < game().rows(); i++)
                    {
                        int k = 0;
                        for (; k < game().cols(); k++)
                        {
                            if (SavedShotsBoard[i][k] == 2)
                                tempbool = false;
                        }
                    }
                    if (tempbool)
                    {
                        Condition1 = false;
                    }
                }

                End1 = 0;
                End2 = 0;

                switch (Condition1)
                {
                    case(true) :
                    {
                       attack_state = 2;
                    }
                       break;
                    default:
                    {
                        attack_state = 1;
                    }
                        break;
                }

            }
            else
            {
                SavedShotsBoard[p.r][p.c] = 2;

                switch (attack_state)
                {
                case(1):
                    {
                        Endpoint2 = p;
                        Endpoint1 = p;
                        attack_state = 2;
                    }
                        break;
                case(2):
                {
                    if (Endpoint1.r > p.r) //determine whether it is vertical or horizontal
                    {
                        ShipDir = VERTICAL;
                        Endpoint1.r = p.r;
                    }
                    else if (Endpoint1.r < p.r)
                    {
                        ShipDir = VERTICAL;
                        Endpoint2.r = p.r;
                    }
                    else if (Endpoint1.c > p.c)
                    {
                        ShipDir = HORIZONTAL;
                        Endpoint1.c = p.c;
                    }
                    else
                    {
                        ShipDir = HORIZONTAL;
                        Endpoint2.c = p.c;
                    }
                    attack_state = 3; 
                    return;
                }
                }
                if (attack_state == 3) 
                {
                    switch (ShipDir)
                    {
                    case HORIZONTAL:
                    {
                        if (Endpoint1.c == 0 || SavedShotsBoard[Endpoint1.r][Endpoint1.c - 1] == 1)
                            End1 = 1;
                        if (Endpoint2.c == game().cols() - 1 || SavedShotsBoard[Endpoint2.r][Endpoint2.c + 1] == 1)
                            End2 = 1;
                    }
                        break;
                    case VERTICAL:
                    {
                        if (Endpoint1.r == 0 || SavedShotsBoard[Endpoint1.r - 1][Endpoint1.c] == 1)
                            End1 = 1;
                        if (Endpoint2.r == game().rows() - 1 || SavedShotsBoard[Endpoint2.r + 1][Endpoint2.c] == 1)
                            End2 = 1;
                    }
                        break;
                    default:
                        break;
                    }

                    if (End1 == 0) //if first end has not been reached, update the first point
                    {
                        Endpoint1 = p;
                    }
                    else //if the first point has been found, update the second point
                    {
                        Endpoint2 = p;
                    }
                    if (End2 == 1 && End1 == 1) //if both ends have been found and no ship has been sunk...
                    {
                        Condition1 = 1; //weird is activated
                        attack_state = 2; //return to state 2
                        return;
                    }
                }
            }
        }
            break;
    default : 
    {
        SavedShotsBoard[p.r][p.c] = 1; 
    }
    break;
    
    
    }

}

void GoodPlayer::recordAttackByOpponent(Point p)
{

}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
