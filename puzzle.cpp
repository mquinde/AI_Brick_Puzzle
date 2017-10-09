//
//  Puzzle.cpp
//  CS380_Sliding_Brick_Puzzle
//
//  Created by Mariana Quinde Garcia on 4/25/17.
//  Copyright © 2017 Mariana Quinde Garcia. All rights reserved.
//

#include "puzzle.h"

Puzzle::Puzzle(string filename)
{
    Load(filename);
}

void Puzzle::AStar(vector<Node> nodes, vector<Node> visited, clock_t start, int heuristic)
{
    // Nodes contains all unvisited child nodes at all levels
    // remove all nodes that are actually visited
    vector<int> indexToRemove;
    for(int i = 0; i < nodes.size(); i++){
        // Check not visited
        bool isVisited = false;
        for(int k = 0; k < visited.size(); k++){
            if(CompareStates(nodes[i].state, visited[k].state)){
                indexToRemove.push_back(i);
                break;
            };
        }
    }
    for(int i = indexToRemove.size()-1; i>=0; i--){
        nodes.erase(nodes.begin() + indexToRemove[i]);
    }

    Node node = nodes[0];
    // find unvisited child with lowest heuristic value
    for(int i = 0; i < nodes.size(); i++){
        if(nodes[i].hvalue < node.hvalue){
            node = nodes[i];
        }
    }
    
    // Visit child with lowest heuristic
    // check if solved
    vector < vector < int > > st =  node.state;
    visited.push_back(node);
    if(IsSolved(st)){
        double time = (clock() - start)/(double) CLOCKS_PER_SEC;
        PrintSolution(node);
        resultNodes = visited.size();
        resultTime  = time;
        resultLength = node.length;
        return;
    }
    // get all child moves
    int newLength = node.length + 1;
    vector<Move> possibleMoves = GetMoves(st);
    for(int j = 0; j < possibleMoves.size(); j++){
        Move move = possibleMoves[j];
        vector < vector < int > > newState = ApplyMoveCloning(move, st);
        Normalize(newState);
        int hvalue;
        if(heuristic == 0){
            hvalue = Manhattan(newState) + newLength;
        } else {
            int v = Quinde(newState);
            hvalue = v + newLength;
        }
        Node newNode = Node(newState, newLength, move, &node, hvalue);
        nodes.push_back(newNode);
    }
    // run again
    AStar(nodes, visited, start, heuristic);
}

void Puzzle::RandomWalk(vector<vector<int> > &st, int n)
{
    int x = 0;
    cout << endl;
    Print(st);
    while( x <= n && !IsSolved(st) ){
        
        // generate all moves
        vector<Move> moves = GetMoves(st);
        
        // select one at random;
        if(moves.size()==0){
            x = n;
            break;
        }
        
        int i = rand() % moves.size();
        
        // execute it
        moves[i].Print();
        cout << endl;
        ApplyMove(moves[i], st);
        
        // normalize the resulting game state
        Normalize(st);
        
        Print(st);
        cout << endl;
        x++;
    }
}

void Puzzle::BFS(vector<Node> nodes, vector<Node> visited, clock_t start)
{
    vector<Node> children;
    for(int i = 0; i < nodes.size(); i++){
        // Check not visited
        Node node = nodes[i];
        vector < vector < int > > st =  node.state;
        bool isVisited = false;
        for(int k = 0; k < visited.size(); k++){
            if(CompareStates(st, visited[k].state)){
                isVisited = true;
                break;
            };
        }
        if(!isVisited){
            // check if solved
            visited.push_back(node);
            if(IsSolved(st)){
                double time = (clock() - start)/(double) CLOCKS_PER_SEC;
                PrintSolution(node);
                resultNodes = visited.size();
                resultTime  = time;
                resultLength = node.length;
                return;
            }
            // get all child moves
            int newLength = node.length + 1;
            
            vector<Move> possibleMoves = GetMoves(st);
            for(int j = 0; j < possibleMoves.size(); j++){
                Move move = possibleMoves[j];
                vector < vector < int > > newState = ApplyMoveCloning(move, st);
                Normalize(newState);
                Node newNode = Node(newState, newLength, move, &node);
                children.push_back(newNode);
            }
        }
    }
    // run for all adjacent
    BFS(children, visited, start);
}

bool Puzzle::DFS(Node node, clock_t start, int limit = -1)
{
    if((limit == -1) || (node.length <= limit) ){
        vector < vector < int > > st = node.state;
        // check if visited
        bool isVisited = false;
        for(int k = 0; k < DFSvisited.size(); k++){
            if(CompareStates(st, DFSvisited[k].state)){
                isVisited = true;
                break;
            };
        }
        if(!isVisited)
        {
            DFSvisited.push_back(node);
            
            // check if solved
            if(IsSolved(st)){
                double time = (clock() - start)/(double) CLOCKS_PER_SEC;
                PrintSolution(node);
                resultNodes = DFSvisited.size();
                resultTime  = time;
                resultLength = node.length;
                return true;
            }
            // get all posible moves
            int newLength = node.length + 1;
            vector<Move> possibleMoves = GetMoves(st);
            for(int j = 0; j < possibleMoves.size(); j++){
                // run for children, and then for sibling
                Move move = possibleMoves[j];
                vector < vector < int > > newState = ApplyMoveCloning(move, st);
                Normalize(newState);
                Node newNode = Node(newState, newLength, move, &node);
                bool found = DFS(newNode, start, limit);
                if(found){
                    DFSvisited.clear();
                    return true;
                    
                }
            }
        }

        
    }
    return false;
}
void Puzzle::IDS(Node node, clock_t start, int limit){
    // set limit
    bool found = DFS(node, start, limit);
    if(!found){
        // increase limit
        limit++;
        DFSvisited.clear();
        IDS(node, start, limit);
    }
}

void Puzzle::Load(string const filename)
{
    string line;
    ifstream file (filename);
    if (file.is_open())
    {
        getline(file,line);
        vector<int> v = SplitString(line);
        if(v.size() < 2){
            cout << "File format is incorrect. \n";
        }
        state.clear();
        width = v[0];
        height = v[1];
        
        while(getline(file,line))
        {
            state.push_back(SplitString(line));
        }
        
        // get coordinates for the top leftmost piece of the goal
        for(int row = 0; row < height; row++) {
            for(int i = 0; i < width; i++){
                if(state[row][i] == -1){
                    goal = make_tuple(row, i);
                    row = height;
                    i = width;
                }
            }
        }
        file.close();
    } else {
        cout << "Could not find file " + filename + "\n";
    }
}

void Puzzle::Print(vector < vector < int > > st)
{
    cout << width << "," << height << ",\n";
    for(int row = 0; row < height; row++) {
        for(int i = 0; i < width; i++){
            cout << st[row][i] << ",";
            if(i==(width-1)){
                cout << "\n";
            }
        }
    }
}

bool Puzzle::IsSolved( vector < vector < int > > st)
{
    for(int row = 0; row < height; row++)
    {
        if(find(st[row].begin(), st[row].end(), -1) != st[row].end()) {
            return false;
        }
    }
    return true;
}

bool Puzzle::CompareStates(vector < vector < int > > stateA, vector<vector<int> > stateB)
{
    int h = stateA.size();
    if(h != stateB.size()){
        return false;
    }
    if(stateA.size()>0){
        int w = stateA[0].size();
        if(w != stateB[0].size()){
            return false;
        }
        for(int row = 0; row < h; row++) {
            for(int i = 0; i < w; i++){
                if(stateA[row][i] != stateB[row][i]){
                    return false;
                }
            }
        }
    }
    return true;
}

vector < vector < int > > Puzzle::Clone(vector < vector < int > > st)
{
    vector < vector < int > > cloned(st);
    return cloned;
}

vector<Move> Puzzle::GetMoves(int const piece, vector < vector < int > > st)
{
    bool canMoveUp = true;
    bool canMoveDown = true;
    bool canMoveLeft = true;
    bool canMoveRight = true;
    bool isMaster = (piece==2);
    for(int row = 0; row < height; row++) {
        for(int i = 0; i < width; i++){
            
            if(st[row][i]==(piece)){
                //check up
                if(canMoveUp){
                    if(row == 0){
                        canMoveUp = false;
                    }
                    else {
                        int cellUp = st[row-1][i];
                        if((!(cellUp==0||cellUp==piece)) && !(cellUp == -1 && isMaster)){
                            canMoveUp = false;
                        }
                    }
                }
                
                //check down
                if(canMoveDown){
                    if(row == height-1){
                        canMoveDown = false;
                    }
                    else {
                        int cellDown = st[row+1][i];
                        if((!(cellDown==0||cellDown==piece)) && !(cellDown == -1 && isMaster)){
                            canMoveDown = false;
                        }
                    }
                }
                
                //check left
                if(canMoveLeft){
                    if(i == 0){
                        canMoveLeft = false;
                    }
                    else {
                        int cellLeft = st[row][i-1];
                        if((!(cellLeft==0||cellLeft==piece)) && !(cellLeft == -1 && isMaster)){
                            canMoveLeft = false;
                        }
                    }
                }
                
                //check right
                if(canMoveRight){
                    if(i == width - 1){
                        canMoveRight = false;
                    }
                    else {
                        int cellRight = st[row][i+1];
                        if((!(cellRight==0||cellRight==piece)) && !(cellRight == -1 && isMaster)){
                            canMoveRight = false;
                        }
                    }
                }
            }
        }
    }
    vector<Move> moves;
    if(canMoveUp){
        moves.push_back(Move(piece,1));
    }
    if(canMoveDown){
        moves.push_back(Move(piece,2));
    }
    if(canMoveLeft){
        moves.push_back(Move(piece,3));
    }
    if(canMoveRight){
        moves.push_back(Move(piece,4));
    }
    return moves;
}

vector<Move> Puzzle::GetMoves(vector < vector < int > > st)
{
    vector<Move> moves;
    vector<int> pieces;
    for(int row = 0; row < height; row++)
    {
        pieces.insert(pieces.end(), st[row].begin(), st[row].end());
    }
    pieces.erase(unique(pieces.begin(), pieces.end()), pieces.end());
    
    for(int i = 0; i < pieces.size(); i++)
    {
        int piece = pieces[i];
        if(piece > 1){
            vector<Move> pieceMoves = GetMoves(piece, st);
            moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
        }
    }
    return moves;
}

void Puzzle::ApplyMove(Move const move, vector < vector < int > > &st)
{
    if(move.direction == 1 || move.direction == 3){
        for(int row = 0; row < height; row++) {
            for(int i = 0; i < width; i++){
                if(st[row][i]==(move.piece))
                {
                    if(move.direction == 1){
                        st[row-1][i] = move.piece;
                        st[row][i] = 0;
                    }
                    if(move.direction == 3){
                        st[row][i-1] = move.piece;
                        st[row][i] = 0;
                    }
                }
            }
        }
    }
    
    if(move.direction == 2){
        for(int row = height-1; row >= 0; row--) {
            for(int i = 0; i < width; i++){
                if(st[row][i]==(move.piece)){
                    st[row+1][i] = move.piece;
                    st[row][i] = 0;
                }
            }
        }
    }
    
    if(move.direction == 4){
        for(int row = 0; row < height; row++) {
            for(int i = width-1; i >= 0; i--){
                if(st[row][i]==(move.piece)){
                    st[row][i+1] = move.piece;
                    st[row][i] = 0;
                }
            }
        }
    }
    
}

vector < vector < int > > Puzzle::ApplyMoveCloning(Move const move, vector < vector < int > > st)
{
    vector < vector < int > > clone = Clone(st);
    ApplyMove(move, clone);
    return clone;
}

void Puzzle::Normalize(vector < vector < int > > &st)
{
    int newPiece = 3;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width ; j++) {
            int current = st[i][j];
            if (current == newPiece) {
                newPiece++;
            }
            else if (current > newPiece) {
                SwitchPiece(newPiece, current, st);
                newPiece++;
            }
        }
    }
}

vector<int> Puzzle::SplitString(string const line)
{
    vector<int> values;
    string value;
    stringstream stream;
    
    stream.str(line);
    while(getline(stream, value, ',')) {
        values.push_back(stoi(value));
    }
    
    return values;
}

void Puzzle::SwitchPiece(int a, int b, vector<vector<int> > &st)
{
    for(int row = 0; row < height; row++) {
        for(int i = 0; i < width; i++){
            if(st[row][i]==a){
                st[row][i] = b;
            }
            else if(st[row][i]== b){
                st[row][i] = a;
            }
        }
    }
    
}

int Puzzle::Manhattan(Node node)
{
    int hvalue;
    int h = node.state.size();
    int w = node.state[0].size();
    tuple<int,int> masterCoordinates;
    // if goal on top or left
    if(get<0>(goal)==0 || get<1>(goal)==0 ){
        // get coordinates for the top leftmost piece of the master
        for(int row = 0; row < h; row++) {
            for(int i = 0; i < w; i++){
                if(node.state[row][i] == 2){
                    masterCoordinates = make_tuple(row, i);
                    row = h;
                    i = w;
                }
            }
        }
    // if goal on right
    } else if(get<0>(goal)==(h-1)){
        // get coordinates for the top rightmost masterpiece
        for(int row = 0; row < h; row++) {
            for(int i = (w-1); i >=0; i--){
                if(node.state[row][i] == 2){
                    masterCoordinates = make_tuple(row, i);
                    row = h;
                    i = -1;
                }
            }
        }
    // if goal on bottom
    } else {
        // get coordinates for the bottom leftmost masterpiece
        for(int row = (h-1); row >= 0; row--) {
            for(int i = 0; i < w; i++){
                if(node.state[row][i] == 2){
                    masterCoordinates = make_tuple(row, i);
                    row = -1;
                    i = w;
                }
            }
        }
    }
    
    hvalue = (abs(get<0>(goal) - get<0>(masterCoordinates))) + (abs(get<1>(goal) - get<1>(masterCoordinates)));
    

    return hvalue;
}

static bool IsCommonPiece (int i) {
    return ( i > 1 || i == 0);
}

void Puzzle::replaceSpaces(vector < vector <int> > cloneState, int enteredSpaces, int emptySpaces, vector < vector < vector < int > > > &resultingStates) {
    if (enteredSpaces == emptySpaces) {
        resultingStates.push_back(cloneState);
    } else {
        int h = cloneState.size();
        int w = cloneState[0].size();
        for(int row = 1; row < h-1; row++) {
            for(int col = 1; col < w-1; col++){
                if(cloneState[row][col] == -3){
                    vector < vector < int > >  altered = Clone(cloneState);
                    altered[row][col] = 0;
                    replaceSpaces(altered, enteredSpaces+1, emptySpaces, resultingStates);
                }
            }
        }
    }
}

void Puzzle::replacePieces(vector < vector <int> > cloneState, vector < vector <int> > originalState, int currentPiece, int maxPiece, vector < vector < vector < int > > > &resultingStates) {
    if (currentPiece > maxPiece) {
        resultingStates.push_back(cloneState);
    } else {
        int h = cloneState.size();
        int w = cloneState[0].size();
        for(int row = 1; row < h-1; row++) {
            for(int col = 1; col < w-1; col++){
                if(cloneState[row][col] == -3){
                    bool fits = true;
                    vector < vector < int > >  altered = Clone(cloneState);
                    int indexRow = 0;
                    for(int rowP = 0; rowP < h; rowP++) {
                        int countPieces = count(originalState[rowP].begin(), originalState[rowP].end(), currentPiece);
                        if(countPieces > 0)
                        {
                            for(int colP = 0; colP < countPieces; colP++){
                                if(((row + indexRow)<h)&&(col+colP<(w))&&(cloneState[row + indexRow][col+colP] == -3)){
                                    altered[row + indexRow][col+colP] = currentPiece;
                                } else {
                                    fits = false;
                                    colP = countPieces;
                                    rowP = h;
                                }
                            }
                            indexRow++;
                        }
                    }
                    
                    if(fits){
                        replacePieces(altered, originalState, currentPiece+1, maxPiece, resultingStates);
                    }
                }
            }
        }
    }
}

int Puzzle::Quinde(Node node)
{
    int hvalue;
    int h = node.state.size();
    int w = node.state[0].size();
    // check if goal states exist
    if(goalStates.size() == 0){
        // if not generate
        // copy start state
        vector < vector < int > > cloneStart = Clone(state);
        // count number of goals
        int countGoals = 0;
        int countZeros = 0;
        for(int i = 0; i < cloneStart.size(); i++){
            // count goal cells
            countGoals += count(cloneStart[i].begin(), cloneStart[i].end(), -1);
            // count initial empty spaces
            countZeros += count(cloneStart[i].begin(), cloneStart[i].end(), 0);
        }
        // calculate final empty spaces
        int emptySpaces = countZeros + countGoals;
        
        // Place the goal in the right place
        // replace -1's with -2's to indicate new goal
        // if goal on top or left
        if(get<0>(goal)==0 || get<1>(goal)==0 ){
            // copy master piece starting by top leftmost piece
            int indexRow = 0;
            for(int row = 0; row < h; row++) {
                if(find(cloneStart[row].begin(), cloneStart[row].end(), 2) != cloneStart[row].end())
                {
                    int countMasters = count(cloneStart[row].begin(), cloneStart[row].end(), 2);
                    for(int col = 0; col < countMasters; col++){
                        cloneStart[get<0>(goal) + indexRow][get<1>(goal) + col] = -2;
                    }
                }
            }
            // if goal on right
        } else if(get<0>(goal)==(h-1)){
            // copy master piece starting by top rightmost piece
            int indexRow = 0;
            for(int row = 0; row < h; row++) {
                if(find(cloneStart[row].begin(), cloneStart[row].end(), 2) != cloneStart[row].end())
                {
                    int countMasters = count(cloneStart[row].begin(), cloneStart[row].end(), 2);
                    for(int col = 0; col < countMasters; col++){
                        cloneStart[get<0>(goal) + indexRow][get<1>(goal) - col] = -2;
                    }
                }
            }            // if goal on bottom
        } else {
            // get coordinates for the bottom leftmost masterpiece
            // copy master piece starting by bottom leftmost piece
            int indexRow = 0;
            for(int row = 0; row < h; row++) {
                if(find(cloneStart[row].begin(), cloneStart[row].end(), 2) != cloneStart[row].end())
                {
                    int countMasters = count(cloneStart[row].begin(), cloneStart[row].end(), 2);
                    for(int col = 0; col < countMasters; col++){
                        cloneStart[get<0>(goal) - indexRow][get<1>(goal) + col] = -2;
                    }
                }
            }
        }
        
        for(int i = 0; i < cloneStart.size(); i++){
            // clear all common pieces
            replace_if(cloneStart[i].begin(), cloneStart[i].end(), IsCommonPiece, -3);
            // change -2 for 2 for master
            replace(cloneStart[i].begin(), cloneStart[i].end(), -2, 2);
        }
        
        // get all possible states with empty spaces
        vector < vector < int > > cloneState = cloneStart;
        vector < vector < vector < int > > > resultingStates;
        vector < vector < vector < int > > > newStates;
        replaceSpaces(cloneStart, 0, emptySpaces, resultingStates);
        newStates.push_back(resultingStates[0]);
        
        
        // remove duplicates
        for(int i = 1; i < resultingStates.size(); i++){
            bool isDuplicate = false;
            for(int k = 0; k < newStates.size(); k++){
                if(CompareStates(resultingStates[i], newStates[k])){
                    isDuplicate = true;
                    break;
                };
            }
            if(!isDuplicate){
                newStates.push_back(resultingStates[i]);
            }
        }
        
        // fill up with normalized
        int highestPiece = state[0][0];
        for(int row = 0; row < h; row++) {
            for(int i = 0; i < w; i++){
                if(state[row][i] > highestPiece){
                    highestPiece = state[row][i];
                }
            }
        }
        
        vector < vector < vector < int > > > rStates;
        for(int k = 0; k < newStates.size(); k++){
            vector < vector < vector < int > > > finalStates;
            replacePieces(newStates[k], state, 3, highestPiece, finalStates);
            rStates.insert(rStates.end(), finalStates.begin(), finalStates.end());
        }
        
        goalStates = rStates;
    }
    
    // for each goal state, check how much the current node state differs
    // get lowest difference
    int lowestDiff = 0;
    for(int i = 0; i < goalStates.size(); i++){
        int diff = 0;
        // compare the inner spaces
        for(int row = 0; row < h; row++) {
            for(int col = 0; col < w; col++){
                if(node.state[row][col] != (goalStates[i])[row][col]){
                    diff++;
                }
            }
        }
        if(i == 0){
            lowestDiff = diff;
        } else {
            if(diff < lowestDiff){
                lowestDiff = diff;
            }
        }
    }
    // return lowest difference
    return lowestDiff;
}


void Puzzle::PrintSolution(Node n)
{
    vector<Move> moves;
    Node node = n;
    while(node.length > 0) {
        moves.push_back(node.move);
        node = *node.parent;
    }
    for(int i = moves.size()-1; i >= 0; i--){
        moves[i].Print();
    }
    Print(n.state);
    cout << "\n";
}

void Puzzle::PrintResults()
{
    cout << resultNodes << " " << resultTime << " " << resultLength << endl;
}


