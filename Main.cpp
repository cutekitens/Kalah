//
//  main.cpp
//  Project 3
//
//  Created by Winona Kraus on 5/16/19.
//  Copyright © 2019 Winona Kraus. All rights reserved.
//

#include "Game.h"
#include <iostream>
using namespace std;

int main()
{
    HumanPlayer hp("Nico");
    SmartPlayer bp("Winona");
    Board b(6, 4);
    Game g(b, &hp, &bp);
    g.play();
}
