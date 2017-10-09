# AI Brick Puzzle
Artificial Intelligence class sliding brick puzzle game

Created an sliding brick puzzle, similar to this one https://www.youtube.com/watch?v=vx25e_9Z3ok
My code implements a Slide Brick Puzzle along with algorithms to solve it. An AStar function to solve the puzzle by the A* Algorithm is now included. The AStar function receives a Heuristic parameter, which determines the heuristic used to solve it (manhattan distance or the custom heuristic). 

Custom Heuristic Description:
My heuristic consists in computing all the possible goal states for the puzzle (meaning all the states where the master has reached the goal and the other puzzle pieces are correctly assembled to a fitting position). That computation is only done the first time the heuristic is used. 
The actual heuristic value will be calculated by checking the difference between a state and all the possible goal states. The difference will be the number of tiles that do not match. The lowest difference towards a goal is taken as a heuristic value.
The evaluation function for each node will therefore be the length of the path of the node + the h(n), being the lowest difference towards a goal state.
The heuristic is admissible since it uses the f(n) of any node will differ by the length of it’s path, so the cost of a move is taken into account, and the lowest difference with a state goal, so the cost of reaching a state(future moves) is also accounted for. 

Results:
The results printed include the moves, the final state, the nodes visited, the time taken and the length of the solution path for the SBP level 1 and level 2 in the manhattan and custom heuristic. 
My custom heuristic takes longer because initially, it has to compute all the possible solutions. However, once that is accounted for, it takes the same or less time as the Manhattan Heuristic and it visits less nodes. In order to reduce this time, it would be better to keep the possible solutions stored and load them along with the file.

To run:

Run “make”
Additionally, run executable hw2: “./hw2”
The results for the second file take nearly a minute to compute, so the make and ./hw2 command take nearly a minute to finish.