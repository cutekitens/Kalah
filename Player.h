//
//  Player.h
//  Project 3
//
//  Created by Winona Kraus on 5/16/19.
//  Copyright © 2019 Winona Kraus. All rights reserved.
//

#ifndef Player_h
#define Player_h
#include "string"
#include "Side.h"
#include "Board.h"

class Player
{
public:
    Player(std::string name);
    // Create a Player with the indicated name.
    std::string name() const;
    // Return the name of the player.
    virtual bool isInteractive() const;
    // Return false if the player is a computer player. Return true if the player is human.
    // Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const = 0;
    // Every concrete class derived from this class must implement this function so that
    // if the player were to be playing side s and had to make a move given board b, the
    // function returns the move the player would choose. If no move is possible, return
    // −1.
    virtual ~Player();
    // Since this class is designed as a base class, it should have a virtual destructor
private:
    std::string m_name;
};

#endif /* Player_h */

#ifndef HumanPlayer_h
#define HumanPlayer_h

class HumanPlayer: public Player
{
public:
    HumanPlayer(std::string name);
    bool isInteractive() const;
    int chooseMove(const Board& b, Side s) const;
};

#endif /* HumanPlayer_h */

#ifndef BadPlayer_h
#define BadPlayer_h

class BadPlayer: public Player
{
public:
    BadPlayer(std::string name);
    bool isInteractive() const;
    int chooseMove(const Board& b, Side s) const;
};

#endif /* BadPlayer_h */

#ifndef AlarmClock_h
#define AlarmClock_h

#include <chrono>
#include <future>
#include <atomic>

class AlarmClock
{
public:
    AlarmClock(int ms)
    {
        m_timedOut = false;
        m_isRunning = true;
        m_alarmClockFuture = std::async([=]() {
            for (int k = 0; k < ms  &&  m_isRunning; k++)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (m_isRunning)
                m_timedOut = true;
        });
    }
    
    ~AlarmClock()
    {
        m_isRunning = false;
        m_alarmClockFuture.get();
    }
    
    bool timedOut() const
    {
        return m_timedOut;
    }
    
    AlarmClock(const AlarmClock&) = delete;
    AlarmClock& operator=(const AlarmClock&) = delete;
private:
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_timedOut;
    std::future<void> m_alarmClockFuture;
};

#endif /* AlarmClock_h */

#ifndef SmartPlayer_h
#define SmartPlayer_h

class SmartPlayer: public Player
{
public:
    SmartPlayer(std::string name);
    bool isInteractive() const;
    int chooseMove(const Board& b, Side s) const;
private:
    int* legalMoves(const Board& b, Side s, int arr[], int size) const;
    int calculateValue(const Board& b, Side s) const;
    int chooseBestMove(AlarmClock& ac, const Board& b, Side s, int searchDepth) const;
    
};

#endif /* SmartPlayer_h */
