//
//  node.cpp
//  CS380_Sliding_Brick_Puzzle
//
//  Created by Mariana Quinde Garcia on 4/28/17.
//  Copyright © 2017 Mariana Quinde Garcia. All rights reserved.
//

#include "node.h"
Node::Node(vector < vector < int > > st):state(st)
{
    length = 0;
}
Node::Node(vector < vector < int > > st, int l, Move m, Node *par):state(st), length(l), move(m), parent(par)
{
}
Node::Node(vector < vector < int > > st, int l, Move m, Node *par, int hv):state(st), length(l), move(m), parent(par), hvalue(hv)
{
}
