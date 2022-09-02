//
//  Side.h
//  Project 3
//
//  Created by Winona Kraus on 5/16/19.
//  Copyright © 2019 Winona Kraus. All rights reserved.
//

#ifndef Side_h
#define Side_h

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}


#endif /* Side_h */
