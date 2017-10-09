//
//  move.h
//  CS380_Sliding_Brick_Puzzle
//
//  Created by Mariana Quinde Garcia on 4/27/17.
//  Copyright © 2017 Mariana Quinde Garcia. All rights reserved.
//

#ifndef move_hpp
#define move_hpp

#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;
class Move {
    
public:
    Move();
    Move(int piece, int direction);
    int piece;
    int direction;
    void Print();
};

#endif /* move_h */
