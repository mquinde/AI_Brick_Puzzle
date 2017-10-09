//
//  node.hpp
//  CS380_Sliding_Brick_Puzzle
//
//  Created by Mariana Quinde Garcia on 4/28/17.
//  Copyright © 2017 Mariana Quinde Garcia. All rights reserved.
//

#ifndef node_hpp
#define node_hpp

#include <stdio.h>
#include <vector>
#include "move.h"
using namespace std;
class Node {
    
public:
    Node(vector < vector < int > > st);
    Node(vector < vector < int > > st, int l, Move m, Node *par);
    Node(vector < vector < int > > st, int l, Move m, Node *par, int hv);
    vector < vector < int > > state;
    Move move;
    int length;
    int hvalue;
    Node *parent;
};
#endif /* node_h */
