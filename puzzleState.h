//
//  puzzleState.h
//  CS380_Sliding_Brick_Puzzle
//
//  Created by Mariana Quinde Garcia on 4/27/17.
//  Copyright © 2017 Mariana Quinde Garcia. All rights reserved.
//

#ifndef puzzleState_hpp
#define puzzleState_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
class PuzzleState {
    
public:
    PuzzleState(vector<vector<int>>);
    vector<vector<int>> state;
    void Print();
    vector<Move> GetMoves(int piece);
    vector<Move> GetMoves();
    void ApplyMove(Move move);
    vector<vector<int>> ApplyMoveCloning(vector<vector<int>> st, Move move);
    bool CompareStates(vector<vector<int>> stateA, vector<vector<int>>stateB);
    void Normalize();
    
};

#endif /* puzzleState_h */
