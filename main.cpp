//
//  main.cpp
//  CS380_Sliding_Brick_Puzzle
//
//  Created by Mariana Quinde Garcia on 4/25/17.
//  Copyright © 2017 Mariana Quinde Garcia. All rights reserved.
//

#include <iostream>
#include "puzzle.h"

int main(int argc, const char * argv[]) {
    //string file1 = "SBP-level1.txt";
    //string file2 = "SBP-level2.txt";
    string file1 = "/Users/mquinde/Documents/DrexelCourses/CS380/CS380_A2_Sliding_Brick_Puzzle/CS380_Sliding_Brick_Puzzle/SBP-level1.txt";
    string file2 = "/Users/mquinde/Documents/DrexelCourses/CS380/CS380_A2_Sliding_Brick_Puzzle/CS380_Sliding_Brick_Puzzle/SBP-level2.txt";
    
    
    // A* with Manhattan
    // File 1
    cout << "SBP-level1.txt manhattan" << endl;
    Puzzle puzzle(file1);
    vector<Node> nodes;
    vector<Node> visited;
    nodes.push_back(Node(puzzle.state));
    puzzle.AStar(nodes, visited, clock(), 0);
    puzzle.PrintResults();
    
    //File 2
    cout << endl << "SBP-level2.txt manhattan" << endl;
    puzzle = Puzzle(file2);
    nodes.clear();
    visited.clear();
    nodes.push_back(Node(puzzle.state));
    puzzle.AStar(nodes, visited, clock(), 0);
    puzzle.PrintResults();
    
    
    // A* with Custom Heuristic
    // File 1
    cout << endl << "SBP-level1.txt custom" << endl;
    puzzle = Puzzle(file1);
    nodes.clear();
    visited.clear();
    nodes.push_back(Node(puzzle.state));
    puzzle.AStar(nodes, visited, clock(), 1);
    puzzle.PrintResults();
    
    
    // file 2
    cout << endl << "SBP-level2.txt custom" << endl;
    puzzle = Puzzle(file2);
    nodes.clear();
    visited.clear();
    nodes.push_back(Node(puzzle.state));
    puzzle.AStar(nodes, visited, clock(), 1);
    puzzle.PrintResults();
    

    return 0;
}
