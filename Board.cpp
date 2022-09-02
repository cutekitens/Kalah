//
//  Board.cpp
//  Project 3
//
//  Created by Winona Kraus on 5/16/19.
//  Copyright © 2019 Winona Kraus. All rights reserved.
//

#include "Board.h"
#include <iostream>
using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole): m_NorthPot(0), m_SouthPot(0)
{
    if (nHoles <= 0)
        m_NumberOfHoles = 1;
    else
        m_NumberOfHoles = nHoles;
    if (nInitialBeansPerHole < 0)
        nInitialBeansPerHole = 0;
    m_NorthHoles = new int [m_NumberOfHoles];
    // creates a new array to hold the number of beans in each hole on the north side
    for (int n = 0; n < m_NumberOfHoles; n++)
    {
        m_NorthHoles[n] = nInitialBeansPerHole;
        // sets each hole to hold the initial number of beans
    }
    m_SouthHoles = new int [m_NumberOfHoles];
    // creates a new array to hold the number of beans in each hole on the south side
    for (int n = 0; n < m_NumberOfHoles; n++)
    {
        m_SouthHoles[n] = nInitialBeansPerHole;
        // sets each hole to hold the initial number of beans
    }
}

Board::Board(const Board& b)
{
    m_NumberOfHoles = b.holes();
    m_NorthHoles = new int [m_NumberOfHoles];
    // creates a new array to hold the number of beans in each hole on the north side
    for (int n = 0; n < m_NumberOfHoles; n++)
    {
        m_NorthHoles[n] = b.beans(NORTH, n+1);
        // copies over beans from other board
    }
    m_SouthHoles = new int [m_NumberOfHoles];
    // creates a new array to hold the number of beans in each hole on the south side
    for (int n = 0; n < m_NumberOfHoles; n++)
    {
        m_SouthHoles[n] = b.beans(SOUTH, n+1); //
        // copies over beans from other board
    }
    m_NorthPot = b.beans(NORTH, 0);
    m_SouthPot = b.beans(SOUTH, 0);
}

Board& Board::operator=(const Board& other)
{
    m_NumberOfHoles = other.m_NumberOfHoles;
    m_NorthHoles = new int [m_NumberOfHoles];
    // creates a new array to hold the number of beans in each hole on the north side
    for (int n = 0; n < m_NumberOfHoles; n++)
    {
        m_NorthHoles[n] = other.m_NorthHoles[n];
        // sets the arrays to have the same elements
    }
    m_SouthHoles = new int [m_NumberOfHoles];
    // creates a new array to hold the number of beans in each hole on the south side
    for (int n = 0; n < m_NumberOfHoles; n++)
    {
        m_SouthHoles[n] = other.m_NorthHoles[n]; //
        // sets the arrays to have the same elements
    }
    m_NorthPot = other.m_NorthPot;
    m_SouthPot = other.m_SouthPot;
    return *this;
}

Board::~Board()
{
    delete[] m_NorthHoles;
    delete[] m_SouthHoles;
}

int Board::holes() const
{
    return m_NumberOfHoles;
}

int Board::beans(Side s, int hole) const
{
    if (hole == 0) // checks if it is the pot
    {
        if (s == SOUTH)
            return m_SouthPot;
        else
            return m_NorthPot;
    }
    if (hole > 0 && hole <= m_NumberOfHoles)
    {
        if (s == NORTH)
            return m_NorthHoles[hole-1]; // the array starts at zero so need to use hole-1
        else
            return m_SouthHoles[hole-1]; // the array starts at zero so need to use hole-1
    }
    return -1;
}

int Board::beansInPlay(Side s) const
{
    int beans = 0;
    if (s == NORTH)
    {
        for (int n = 0; n < m_NumberOfHoles; n++)
            beans += m_NorthHoles[n];
            // looks at each hole and adds the number of beans in each to beans
    }
    else
    {
        for (int n = 0; n < m_NumberOfHoles; n++)
            beans += m_SouthHoles[n];
            // looks at each hole and adds the number of beans in each to beans
    }
    return beans; // does not include the number of beans in the pots
}
int Board::totalBeans() const
{
    int beans = 0;
    for (int n = 0; n < m_NumberOfHoles; n++)
        beans += m_NorthHoles[n];
        // looks at each hole and adds the number of beans in each to beans
    for (int n = 0; n < m_NumberOfHoles; n++)
        beans += m_SouthHoles[n];
        // looks at each hole and adds the number of beans in each to beans
    beans += m_SouthPot; // adds the number of beans in this pot to beans
    beans += m_NorthPot; // adds the number of beans in this pot to beans
    return beans;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if (hole-1 < 0 || hole-1 >= m_NumberOfHoles) // makes sure hole is valid
        return false;
    if (s == NORTH)
    {
        if (m_NorthHoles[hole-1] == 0) // if hole is empty then will return false
            return false;
    }
    else
    {
        if (m_SouthHoles[hole-1] == 0) // if hole is empty then will return false
            return false;
    }
    Side original = s; // holds the value of the player's side
    int numberOfBeans; // holds the number of beans in the choosen hole
    if (s == NORTH)
    {
        numberOfBeans = m_NorthHoles[hole-1];
        m_NorthHoles[hole-1] = 0; // empties the hole
    }
    else
    {
        numberOfBeans = m_SouthHoles[hole-1];
        m_SouthHoles[hole-1] = 0; // empties the hole
    }
    for (int n = 0; n < numberOfBeans; n++)
    {
        if (s == SOUTH)
            hole++; // moves to the next hole
        else
            hole--; // moves to the next hole
        if (hole == m_NumberOfHoles+1 && s == SOUTH) // The hole is the pot in this case, so it is set to 0
            hole = 0;
        if (hole == POT)
        // considers the case if the hole being looked at is the player or the opponent's pot
        {
            if (s == original) // checks if it is the player's pot
            {
                
                if (original == NORTH)
                {
                    m_NorthPot++; // adds the bean to the pot
                    s = SOUTH; // changes side for next bean
                    hole = 0; // next bean will go in 1
                }
                else
                {
                    m_SouthPot++; // adds the bean to the pot
                    s = NORTH; // changes side for next bean
                    hole = m_NumberOfHoles+1; // next bean will go in m_NumberOfHoles
                }
            }
            else
            {
                if (original == SOUTH)
                    hole = 1;
                else
                    hole = m_NumberOfHoles;
                s = original;
            // if it's the opponent's pot then the hole and side will be set to the appropriate values
            }
        }
        if (hole != POT && hole != m_NumberOfHoles+1)
        {
            if (s == NORTH)
            {
                m_NorthHoles[hole-1]++; // adds a bean to this hole
            }
            else
                m_SouthHoles[hole-1]++; // adds a bean to this hole

        }
    }
    if (hole == m_NumberOfHoles+1 && s == NORTH) // the beans ended at South's pot
    {
        s = SOUTH;
        hole = 0;
    }
    else if (hole == 0 && s == SOUTH) // the beans ended at North's pot
        s = NORTH;
    endSide = s;
    endHole = hole;
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if (hole-1 < 0 || hole-1 >= m_NumberOfHoles)
        return false;
    if (s == NORTH) // if the hole belongs to the north side
    {
        if (potOwner == NORTH) // who gets the beans put in their pot
            m_NorthPot += m_NorthHoles[hole-1];
        else
            m_SouthPot += m_NorthHoles[hole-1];
        m_NorthHoles[hole-1] = 0; // empties hole
    }
    else // if the hole belongs to the south side
    {
        if (potOwner == NORTH) // who gets the beans put in their pot
            m_NorthPot += m_SouthHoles[hole-1];
        else
            m_SouthPot += m_SouthHoles[hole-1];
        m_SouthHoles[hole-1] = 0; // empties hole
    }
    return true;
}
bool Board::setBeans(Side s, int hole, int beans)
{
    if (hole-1 < -1 || hole-1 >= m_NumberOfHoles || beans < 0)
        // checks if the hole is invalid or beans is negative
        return false;
    if (s == SOUTH)
    {
        if (hole != 0) // not the pot
            m_SouthHoles[hole-1] = beans; // sets the hole to number of beans given
        else // it's the pot
            m_SouthPot = beans;
    }
    else
    {
        if (hole != 0) // not the pot
            m_NorthHoles[hole-1] = beans; // sets the hole to number of beans given
        else // it's the pot
            m_NorthPot = beans;
        
    }
    return true;
}
