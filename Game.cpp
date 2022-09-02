//
//  Game.cpp
//  Project 3
//
//  Created by Winona Kraus on 5/16/19.
//  Copyright © 2019 Winona Kraus. All rights reserved.
//

#include "Game.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north): m_Board(b), m_South(south), m_North(north), m_Turn(SOUTH)
{
    
}

void Game::display() const
{
    int length = m_Board.holes()+2;
    string grid[9][length];
    int r, c;
    grid[0][0] = m_North->name(); // sets first row North's name
    int x = 1;
    for (c = 2; c < length; c++)
    // sets the holes numbers in the grid with the pot set to 0 for second row
    {
        grid[1][c] = to_string(x);
        x++;
    }
    for (c = 2; c < length; c++)
    // the third row is dashes for each hole not including the pots
        grid[2][c] = "-";
    x = 1;
    for (c = 2; c < length; c++)
    {
        // sets the fourth row to the number of beans in each hole
        grid[3][c] = to_string(m_Board.beans(NORTH, x));
        x++;
    }
    grid[4][0] = to_string(m_Board.beans(NORTH, 0));
    grid[4][length-1] = to_string(m_Board.beans(SOUTH, 0));
     // sets the fifth row to the number of beans in the pot for each side
    x = 1;
    for (c = 2; c < length; c++)
    {
        // sets the sixth row to the number of beans in each hole
        grid[5][c] = to_string(m_Board.beans(SOUTH, x));
        x++;
    }
    for (c = 2; c < length; c++)
        // the seventh row is dashes for each hole not including the pots
        grid[6][c] = "-";
    x = 1;
    for (c = 2; c < length; c++)
        // sets the holes numbers in the grid with the pot set to 0 for eighth row
    {
        grid[7][c] = to_string(x);
        x++;
    }
    grid[8][0] = m_South->name(); // sets last row South's name
    cout << grid[0][0]; // prints first row
    cout << endl;
    for (r = 1; r < 4; r++)
    {
        for (c = 0; c < length; c++)
            cout << grid[r][c] << ' ';
        // prints 2th-4th row with spaces between each string
        cout << endl;
    }
    cout << grid[4][0]; // prints north's pot
    for (c = 2; c < length; c++) // spaces north and south's pot
        cout << "  ";
    cout << " " << grid[4][length-1]; // prints south's pot
    cout << endl;
    for (r = 5; r < 8; r++)
    {
        for (c = 0; c < length; c++) // prints 6th, 7th and 8th row
            cout << grid[r][c] << " ";
        cout << endl;
    }
    cout << grid[8][0]; // prints last row
    cout << endl << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    for (int n = 1; n <= m_Board.holes(); n++)
    {
            if (m_Board.beans(SOUTH, n) > 0) // makes sure at least hole has beans
            {
                for (int n = 1; n <= m_Board.holes(); n++)
                {
                    if (m_Board.beans(NORTH, n) > 0) // makes at least hole has beans
                    {
                        over = false;
                        // both players must be able to move for the game to not be over
                        break;
                    }
                    else if (n == m_Board.holes())
                        over = true; // otherwise, the game is over
                }
                break;
            }
            else if (n == m_Board.holes())
                over = true; // otherwise, the game is over
    }
    // compares the number of beans in pots
    if (over == false)
        hasWinner = false;
    else
    {
        if (m_Board.beans(NORTH, 0) == m_Board.beans(SOUTH, 0))
            // if they equal then there is no winner
            hasWinner = false;
        else if (m_Board.beans(NORTH, 0) > m_Board.beans(SOUTH, 0))
            // if the north side has more then they are the winner
        {
            hasWinner = true;
            winner = NORTH;
        }
        else
            // otherwise, the south side wins
        {
            hasWinner = true;
            winner = SOUTH;
        }
    }
}

bool Game::move()
{
    bool over;
    bool hasWinner;
    Side winner;
    status(over, hasWinner, winner);
    if (over == true) // checks to make sure the game isn't over
    {
        play(); // ends game
        return false;
    }
    Side endSide;
    int endHole;
    display();
    if (m_Turn == SOUTH) // if it's the south player's turn
    {
        m_Board.sow(SOUTH, m_South->chooseMove(m_Board, SOUTH), endSide, endHole);
        // selects a hole and moves beans
        if (endHole == 0 && endSide == SOUTH)
        {
            move(); // if the player ends in their pot then they take another turn
        }
        else if (endSide == SOUTH && m_Board.beans(SOUTH, endHole) == 1 && m_Board.beans(NORTH, endHole) > 0)
        {
            // if the player captures the other player's beans
            m_Board.moveToPot(NORTH, endHole, SOUTH);
            m_Board.moveToPot(SOUTH, endHole, SOUTH);
        }
        m_Turn = NORTH;
    }
    else // if it's the north player's turn
    {
        m_Board.sow(NORTH, m_North->chooseMove(m_Board, NORTH), endSide, endHole);
        // selects a hole and moves beans
        if (endHole == 0 && endSide == NORTH)
        {
            move(); // if the player ends in their pot then they take another turn
        }
        else if (endSide == NORTH && m_Board.beans(NORTH, endHole) == 1 && m_Board.beans(SOUTH, endHole) > 0)
        {
        // if the player captures the other player's beans
            m_Board.moveToPot(SOUTH, endHole, NORTH);
            m_Board.moveToPot(NORTH, endHole, NORTH);
        }
        m_Turn = SOUTH;
    }
    status(over, hasWinner, winner);
    if (over == true) // clears board
    {
        display();
        for (int n = 1; n <= m_Board.holes(); n++) // empties board
        {
            m_Board.moveToPot(SOUTH, n, SOUTH);
            m_Board.moveToPot(NORTH, n, NORTH);
        }
    }
    return true;
}

void Game::play()
{
    bool over;
    bool hasWinner;
    Side winner;
    status(over, hasWinner, winner);
    while (over == false) // while the game is not over
    {
        move();
        if (!m_North->isInteractive() && !m_South->isInteractive())
        {
            string x;
            cout << "Press enter to continue." << endl;
            // waits for the user to press enter to continue
            cin.ignore(10000, '\n');

        }
        status(over, hasWinner, winner);
    }
    status(over, hasWinner, winner);
    display(); // displays the final scores
    if (hasWinner == false)
        cout << "It was a tie!" << endl;
    else
    {
        if (winner == NORTH)
            cout << m_North->name() << " wins!" << endl;
        else
            cout << m_South->name() << " wins!" << endl;
    }
}
int Game::beans(Side s, int hole) const
{
    return m_Board.beans(s, hole);
}
