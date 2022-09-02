//
//  Player.cpp
//  Project 3
//
//  Created by Winona Kraus on 5/16/19.
//  Copyright © 2019 Winona Kraus. All rights reserved.
//

#include "Player.h"
#include <iostream>
#include <random>
using namespace std;

Player::Player(string name): m_name(name)
{
    
}

Player::~Player()
{
    
}

string Player::name() const
{
    return m_name;
}

bool Player::isInteractive() const
{
    return false;
}

HumanPlayer::HumanPlayer(string name): Player(name)
{
    
}

bool HumanPlayer::isInteractive() const
{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    for (int n = 1; n <= b.holes(); n++) // checks if the player can make any moves
    {
        if (b.beans(s, n) != 0) // if at least one hole has beans then the for loop will end
            break;
        else if (n == b.holes()) // otherwise, it will return -1 if it gets to the end
            return -1;
    }
    int hole = 0;
    while (hole <= 0 || hole > b.holes() || b.beans(s, hole) == 0)
    // Prompts the user until the number given is valid and not empty
    {
        cout << "Choose a hole: " << endl;
        cin >> hole;
    }
    return hole;
}

BadPlayer::BadPlayer(string name): Player(name)
{
    
}

bool BadPlayer::isInteractive() const
{
    return false;
}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    for (int n = 1; n <= b.holes(); n++) // checks if the player can make any moves
    {
        if (b.beans(s, n) != 0) // if at least one hole has beans then the for loop will end
            break;
        else if (n == b.holes()) // otherwise, it will return -1 if it gets to the end
            return -1;
    }
    int hole = 0;
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(1, b.holes());
    while (b.beans(s, hole) == 0 || hole == 0) // runs until the hole is not empty and not equal to 0
        hole = distro(generator); // selects a random number between 1 and the number of holes
    return hole;
}

SmartPlayer::SmartPlayer(string name): Player(name)
{
    
}

bool SmartPlayer::isInteractive() const
{
    return false;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    AlarmClock ac(4900);  // will time out after 4.9 seconds
    return (chooseBestMove(ac, b, s, 0));
}

int SmartPlayer::calculateValue(const Board& b, Side s) const
{
    if (b.beans(opponent(s), 0) > b.totalBeans()/2) // if the opponent has the majority of the beans
        return -999; // -999 signifies a loss
    else if (b.beans(s, 0) > b.totalBeans()/2) // if the player has the majority of the beans
        return 999; // 999 signifies a win
    return (b.beans(s, 0) - b.beans(opponent(s), 0)); // returns the difference between the number
    // of beans in the pots of the two players
}

int* SmartPlayer::legalMoves(const Board& b, Side s, int arr[], int size) const
{
    
    int i = 0;
    for (int n = 1; n <= size; n++) // loops through holes
    {
        if (b.beans(s, n) > 0) // if there are beans in the hole...
        {
            arr[i] = n; //...then that hole number is added to the array
            i++;
        }
    }
    if (i != size) // if the array is not full...
    {
        while (i != size)
        {
            arr[i] = 0; // ...all the other elements are set to 0
            i++;
        }
    }
    return arr; // this should never be empty
}

int SmartPlayer::chooseBestMove(AlarmClock& ac, const Board& b, Side s, int searchDepth) const
{
    int* moves;
    int arr[b.holes()];
    int size = b.holes(), endHole;
    int bestHole, value = 0, value2;
    Side endSide;
    for (int n = 1; n <= b.holes(); n++) // checks if the player can make any moves
    {
        if (b.beans(s, n) != 0) // if at least one hole has beans then the for loop will end
            break;
        else if (n == b.holes()) // otherwise, it will return -1 if it gets to the end
        {
            value = calculateValue(b, s); // calculates the value at that position
            bestHole = -1;
            return value;
        }
    }
    moves = legalMoves(b, s, arr, size); // sets the pointer to point at the
    // first element of the list of legal moves found by the function
    bestHole = moves[0];
    if (searchDepth == 3) // It will think 3 moves ahead
    {
        value = calculateValue(b, opponent(s));
        // since s is the opponent we need to look at the opponent of s to get the correct value
        bestHole = -1;
        return value;
    }
    for (int n = 0; moves[n] != 0 && n != size && !ac.timedOut(); n++)
    {
        Board cb(b);
        cb.sow(s, moves[n], endSide, endHole);
        if (endSide == s && cb.beans(s, endHole) == 1 && cb.beans(opponent(s), endHole) > 0)
        {
            // if the player captures the other player's beans
            cb.moveToPot(opponent(s), endHole, s);
            cb.moveToPot(s, endHole, s);
        }
        while (endSide == s && endHole == 0) // the player goes again
        {
            int* moves2;
            moves2 = legalMoves(cb, s, arr, size);
            int tempvalue = -1000, tempelement = 0;
            Board cbb(cb); // new copy of the board
            for (int n = 0; moves2[n] != 0 && n != size; n++)
            {
                cbb.sow(s, moves2[n], endSide, endHole);
                if (calculateValue(cbb, s) > tempvalue)
                {
                    tempvalue = calculateValue(cbb, s);
                    tempelement = n;
                }
            }
            cb.sow(s, moves2[tempelement], endSide, endHole);
            if (endSide == s && cb.beans(s, endHole) == 1 && cb.beans(opponent(s), endHole) > 0)
            {
                // if the player captures the other player's beans
                cb.moveToPot(opponent(s), endHole, s);
                cb.moveToPot(s, endHole, s);
            }
        }
            value2 = chooseBestMove(ac, cb, opponent(s), searchDepth+1);
            cb = b;
            if (value2 > value && (searchDepth == 0 || searchDepth == 2))
            // if we are considering South's move, we pick the maximum value
            {
                bestHole = moves[n];
                value = value2;
            }
            else if (-value2 < value && searchDepth == 1)
            // if we are considering North's move, we pick the minimum value
            {
                bestHole = moves[n];
                value = -value2;
            }
    }
    if (searchDepth == 0) // The function will have gone through all possible moves \
        // and needs to return the bestHole rather than the value
        return bestHole;
    return value;
}

