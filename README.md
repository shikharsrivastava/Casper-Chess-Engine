# Casper-Chess-Engine

Casper is a Chess Engine (Chess playing bot) written in C++. This was my first endeavour to learn about Artificial Intelligence.  
Casper is written for windows and uses the following algorithms and techniques-

## Move Generation

Move Generation forms the base of the engine. It enumerates all the possible moves from the current state of the board. It differentiates the moves on the basis of -
 - Killing Move
 - EnPeasent Move
 - Capturing Move
 - Castling Move
 - Pawn Start Move
 - Promotion Move
 - Score of the move (For future move ordering)

The move is encoded in a 32 bit integer where the segments of the bits are used tostore information of the move like -
 - Castling Flag
 - Pawn Start Flag
 - Capturing Flag
 - Promotion Flag
 - Initial and final position of piece
 
The move generator accuulates thes moves in a move list for searching

## Searching and Evaluation

This is the brain of the engine and it is where it actuallys searches the best move in vision. The algorithms used are-
 - NegaScout Search Algorithm (Principal Variation Search)
 - Quiescence Search (To check Horizon effect)
 - Static Evaluation Function
 - Killer Heuristics
 - History Heuristics
 - MvvLva Move ordering
 - Zorbist Hashing for state saving
 - Iterative Deepening for (Move ordering)
 
The search is Done by passing in the time and max depth information. The search takes place util it reaches the desired depth or the time finishes.
