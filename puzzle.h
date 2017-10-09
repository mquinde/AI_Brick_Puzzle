//
//  Puzzle.hpp
//  CS380_Sliding_Brick_Puzzle
//
//  Created by Mariana Quinde Garcia on 4/25/17.
//  Copyright © 2017 Mariana Quinde Garcia. All rights reserved.
//

#ifndef puzzle_h
#define puzzle_h

#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <algorithm>
#include <tuple>
#include "move.h"
#include "node.h"

using namespace std;
class Puzzle {

public:
    
    vector < vector < int > > state;
    Puzzle(string filename);
    
    void Load(string filename);
    void Print(vector < vector < int > > st);
    void PrintResults();
    bool IsSolved(vector < vector < int > > st);
    bool CompareStates(vector < vector < int > > stateA, vector < vector < int > >stateB);
    
    vector < vector < int > > Clone(vector < vector < int > > st);
    vector<Move> GetMoves(int piece, vector < vector < int > > st);
    vector<Move> GetMoves(vector < vector < int > > st);
    vector < vector < int > > ApplyMoveCloning(Move move, vector < vector < int > > st);
    
    void ApplyMove(Move move, vector < vector < int > > &st);
    void Normalize(vector < vector < int > > &st);
    
    void RandomWalk(vector < vector < int > > &st, int n);
    void BFS(vector<Node> nodes, vector<Node> visited, clock_t start);
    bool DFS(Node node, clock_t start, int limit);
    void IDS(Node node, clock_t start, int limit);
    void AStar(vector<Node> nodes, vector<Node> visited, clock_t start, int heuristic);
    int Quinde(Node node);

    double resultTime;
    int resultLength;
    int resultNodes;
    
private:
    int width;
    int height;
    tuple<int,int> goal;
    vector<int> SplitString(string line);
    vector< vector < vector < int > > > goalStates;
    void SwitchPiece(int a, int b, vector < vector < int > > &st);
    void PrintSolution(Node node);
    vector < Node > DFSvisited;
    int Manhattan(Node node);
    static bool IsMoreThanOne (int i);
    void replaceSpaces(vector < vector <int> > cloneState, int enteredSpaces, int emptySpaces, vector < vector < vector < int > > > &resultingStates);
    void replacePieces(vector < vector <int> > cloneState, vector < vector <int> > originalState, int currentPiece, int maxPiece, vector < vector < vector < int > > > &resultingStates);
};

#endif /* Puzzle_h */
