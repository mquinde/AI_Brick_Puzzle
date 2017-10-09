//
//  move.cpp
//  CS380_Sliding_Brick_Puzzle
//
//  Created by Mariana Quinde Garcia on 4/27/17.
//  Copyright © 2017 Mariana Quinde Garcia. All rights reserved.
//

#include "move.h"
Move::Move(){}
Move::Move(int p, int d):piece(p), direction(d)
{
}
void Move::Print()
{
    cout << "(" << piece << "," ;
    if(direction == 1){
        cout << "Up) \n";
    }
    else if(direction == 2){
        cout << "Down) \n";
    }
    else if(direction == 3){
        cout << "Left) \n";
    }
    else
    {
        cout << "Right) \n";
    }
}
