#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdlib.h>

#define BOUNDS 0x88 /* Magic number that when ANDed with a square, will be false if the square is on the board, and true if it is not. */
#define MAX_PIECE_MOVES 27	/* Maximum number of possible moves a piece can have during a given turn */
#define NUM_CHESS_PIECES 32	/* Maximum number of chess pieces on the board */


typedef enum piece_t {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING} Piece;
typedef enum color_t {BLACK, WHITE} Color;

/* Function pointer that generates all possible moves for a piece and populates the given array
 * with all the possible end squares and returns the number of such possible moves. */
typedef unsigned char (*piece_move)(unsigned char[], unsigned char);

typedef struct{
	unsigned char isTaken;
	Color color;
	Piece piece;
	piece_move generate_moves;	
	unsigned char location;
	unsigned char numMoves;
	unsigned char moves[MAX_PIECE_MOVES];
}chess_piece;

/* Initializes the chessboard to its start position */
void init_chessboard(int variant);

/* Generates possible moves for a pawn */
unsigned char generate_pawn_moves(unsigned char moves[], unsigned char startSquare);

/* Generates possible moves for a knight */
unsigned char generate_knight_moves(unsigned char moves[], unsigned char startSquare);

/* Generates possible moves for a bishop */
unsigned char generate_bishop_moves(unsigned char moves[], unsigned char startSquare);

/* Generates possible moves for a rook */
unsigned char generate_rook_moves(unsigned char moves[], unsigned char startSquare);

/* Generates possible moves for a queen */
unsigned char generate_queen_moves(unsigned char moves[], unsigned char startSquare);

/* Generates possible moves for a king */
unsigned char generate_king_moves(unsigned char moves[], unsigned char startSquare);

/* Generates all possible moves for the current player whose turn it is to move */
int get_all_moves();

/* Checks if a given move is valid */
int isValidMove(unsigned char startSquare, unsigned char endSquare);

/* Performs the given move. Returns 1 if promotion has occurred, 2 if queenside castling occurred, 
 * 4 if kingside castling occurred, 6 if en passant occurred, and 0 else. 
 * In the atomic chess variant, it can also return 7, which means a capture has occurred */
int move(unsigned char startSquare, unsigned char endSquare);

/* Promotes the pawn at the promotion square to the given piece. Invalid piece arguments should never occur. */
void promote(unsigned char promotionSquare, Piece newPiece);

/* Returns the color whose move it is */
Color get_color();

/* Returns 1 if the king is in check. Returns 0 otherwise */
int in_check(Color kingColor);

/* Returns 1 if the potential piece move will put the king in check. Returns 0 otherwise */
int puts_king_in_check(unsigned char startSquare, unsigned char endSquare, Color pieceColor);

/* So other files can access the current turn: 0 for white, non zero for black */
extern unsigned char currentTurn; 

/* So other files cana ccess the number of half moves since a pawn move or capture has been made */
extern int numMoves;

/* Functions for other chess variants */

/* Populates the argument array with all squares on the board that are adjacent to the given square.
 * Returns the number of adjacent squares (i.e. the number of valid squares returned in the adjacentSquares array) */
unsigned char getAdjacentSquares(unsigned char startSquare, unsigned char adjacentSquares[]);

#endif
