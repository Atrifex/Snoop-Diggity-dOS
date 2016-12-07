#include "chessboard.h"


chess_piece* chessboard[8][8];
chess_piece pieces[NUM_CHESS_PIECES];
unsigned char currentTurn; /* 0 for white, non zero for black */
unsigned char white_king_has_moved, black_king_has_moved, white_qr_has_moved, white_kr_has_moved, black_qr_has_moved, black_kr_has_moved;
unsigned char en_passant_possible; /* Set to 1 when a pawn moves 2 squares */
chess_piece* en_passant_victim;	/* Pointer to the pawn that just moved 2 squares, which potentially could get taken en passant */
int numMoves; /* Keeps track of the number of half moves since a pawn move or a capture (100 needed to be able to claim a draw) */

int curVariant = 0; /* 0 for normal chess, 1 for atomic chess, and 2 for kriegspiel */


/* Initializes the chessboard to its start position */
void init_chessboard(int variant) {
	int r, c; /* Loop variables */

	switch(variant) {
		case 1: // atomic chess
			curVariant = 1;
			break;
		case 2: //kriegspiel
			curVariant = 2;
			break;
		default: // normal chess
			curVariant = 0;
			break;
	}

	/* Initialize piece structs */

	/* White's queen rook */
	pieces[0].isTaken = 0;
	pieces[0].color = WHITE;
	pieces[0].piece = ROOK;
	pieces[0].generate_moves = generate_rook_moves;
	pieces[0].location = 0x00;
	chessboard[0][0] = &pieces[0];

	/* White's queen knight */
	pieces[1].isTaken = 0;
	pieces[1].color = WHITE;
	pieces[1].piece = KNIGHT;
	pieces[1].generate_moves = generate_knight_moves;
	pieces[1].location = 0x01;
	chessboard[0][1] = &pieces[1];

	/* White's queen bishop */
	pieces[2].isTaken = 0;
	pieces[2].color = WHITE;
	pieces[2].piece = BISHOP;
	pieces[2].generate_moves = generate_bishop_moves;
	pieces[2].location = 0x02;
	chessboard[0][2] = &pieces[2];

	/* White's queen */
	pieces[3].isTaken = 0;
	pieces[3].color = WHITE;
	pieces[3].piece = QUEEN;
	pieces[3].generate_moves = generate_queen_moves;
	pieces[3].location = 0x03;
	chessboard[0][3] = &pieces[3];

	/* White's king */
	pieces[4].isTaken = 0;
	pieces[4].color = WHITE;
	pieces[4].piece = KING;
	pieces[4].generate_moves = generate_king_moves;
	pieces[4].location = 0x04;
	chessboard[0][4] = &pieces[4];

	/* White's king bishop */
	pieces[5].isTaken = 0;
	pieces[5].color = WHITE;
	pieces[5].piece = BISHOP;
	pieces[5].generate_moves = generate_bishop_moves;
	pieces[5].location = 0x05;
	chessboard[0][5] = &pieces[5];
	
	/* White's king knight */
	pieces[6].isTaken = 0;
	pieces[6].color = WHITE;
	pieces[6].piece = KNIGHT;
	pieces[6].generate_moves = generate_knight_moves;
	pieces[6].location = 0x06;
	chessboard[0][6] = &pieces[6];

	/* White's king rook */
	pieces[7].isTaken = 0;
	pieces[7].color = WHITE;
	pieces[7].piece = ROOK;
	pieces[7].generate_moves = generate_rook_moves;
	pieces[7].location = 0x07;
	chessboard[0][7] = &pieces[7];

	/* White's a-pawn */
	pieces[8].isTaken = 0;
	pieces[8].color = WHITE;
	pieces[8].piece = PAWN;
	pieces[8].generate_moves = generate_pawn_moves;
	pieces[8].location = 0x10;
	chessboard[1][0] = &pieces[8];

	/* White's b-pawn */
	pieces[9].isTaken = 0;
	pieces[9].color = WHITE;
	pieces[9].piece = PAWN;
	pieces[9].generate_moves = generate_pawn_moves;
	pieces[9].location = 0x11;
	chessboard[1][1] = &pieces[9];

	/* White's c-pawn */
	pieces[10].isTaken = 0;
	pieces[10].color = WHITE;
	pieces[10].piece = PAWN;
	pieces[10].generate_moves = generate_pawn_moves;
	pieces[10].location = 0x12;
	chessboard[1][2] = &pieces[10];

	/* White's d-pawn */
	pieces[11].isTaken = 0;
	pieces[11].color = WHITE;
	pieces[11].piece = PAWN;
	pieces[11].generate_moves = generate_pawn_moves;
	pieces[11].location = 0x13;
	chessboard[1][3] = &pieces[11];

	/* White's e-pawn */
	pieces[12].isTaken = 0;
	pieces[12].color = WHITE;
	pieces[12].piece = PAWN;
	pieces[12].generate_moves = generate_pawn_moves;
	pieces[12].location = 0x14;
	chessboard[1][4] = &pieces[12];

	/* White's f-pawn */
	pieces[13].isTaken = 0;
	pieces[13].color = WHITE;
	pieces[13].piece = PAWN;
	pieces[13].generate_moves = generate_pawn_moves;
	pieces[13].location = 0x15;
	chessboard[1][5] = &pieces[13];

	/* White's g-pawn */
	pieces[14].isTaken = 0;
	pieces[14].color = WHITE;
	pieces[14].piece = PAWN;
	pieces[14].generate_moves = generate_pawn_moves;
	pieces[14].location = 0x16;
	chessboard[1][6] = &pieces[14];

	/* White's h-pawn */
	pieces[15].isTaken = 0;
	pieces[15].color = WHITE;
	pieces[15].piece = PAWN;
	pieces[15].generate_moves = generate_pawn_moves;
	pieces[15].location = 0x17;
	chessboard[1][7] = &pieces[15];


	/* Black's queen rook */
	pieces[16].isTaken = 0;
	pieces[16].color = BLACK;
	pieces[16].piece = ROOK;
	pieces[16].generate_moves = generate_rook_moves;
	pieces[16].location = 0x70;
	chessboard[7][0] = &pieces[16];

	/* Black's queen knight */
	pieces[17].isTaken = 0;
	pieces[17].color = BLACK;
	pieces[17].piece = KNIGHT;
	pieces[17].generate_moves = generate_knight_moves;
	pieces[17].location = 0x71;
	chessboard[7][1] = &pieces[17];

	/* Black's queen bishop */
	pieces[18].isTaken = 0;
	pieces[18].color = BLACK;
	pieces[18].piece = BISHOP;
	pieces[18].generate_moves = generate_bishop_moves;
	pieces[18].location = 0x72;
	chessboard[7][2] = &pieces[18];

	/* Black's queen */
	pieces[19].isTaken = 0;
	pieces[19].color = BLACK;
	pieces[19].piece = QUEEN;
	pieces[19].generate_moves = generate_queen_moves;
	pieces[19].location = 0x73;
	chessboard[7][3] = &pieces[19];

	/* Black's king */
	pieces[20].isTaken = 0;
	pieces[20].color = BLACK;
	pieces[20].piece = KING;
	pieces[20].generate_moves = generate_king_moves;
	pieces[20].location = 0x74;
	chessboard[7][4] = &pieces[20];

	/* Black's king bishop */
	pieces[21].isTaken = 0;
	pieces[21].color = BLACK;
	pieces[21].piece = BISHOP;
	pieces[21].generate_moves = generate_bishop_moves;
	pieces[21].location = 0x75;
	chessboard[7][5] = &pieces[21];
	
	/* Black's king knight */
	pieces[22].isTaken = 0;
	pieces[22].color = BLACK;
	pieces[22].piece = KNIGHT;
	pieces[22].generate_moves = generate_knight_moves;
	pieces[22].location = 0x76;
	chessboard[7][6] = &pieces[22];

	/* Black's king rook */
	pieces[23].isTaken = 0;
	pieces[23].color = BLACK;
	pieces[23].piece = ROOK;
	pieces[23].generate_moves = generate_rook_moves;
	pieces[23].location = 0x77;
	chessboard[7][7] = &pieces[23];

	/* Black's a-pawn */
	pieces[24].isTaken = 0;
	pieces[24].color = BLACK;
	pieces[24].piece = PAWN;
	pieces[24].generate_moves = generate_pawn_moves;
	pieces[24].location = 0x60;
	chessboard[6][0] = &pieces[24];

	/* Black's b-pawn */
	pieces[25].isTaken = 0;
	pieces[25].color = BLACK;
	pieces[25].piece = PAWN;
	pieces[25].generate_moves = generate_pawn_moves;
	pieces[25].location = 0x61;
	chessboard[6][1] = &pieces[25];

	/* Black's c-pawn */
	pieces[26].isTaken = 0;
	pieces[26].color = BLACK;
	pieces[26].piece = PAWN;
	pieces[26].generate_moves = generate_pawn_moves;
	pieces[26].location = 0x62;
	chessboard[6][2] = &pieces[26];

	/* Black's d-pawn */
	pieces[27].isTaken = 0;
	pieces[27].color = BLACK;
	pieces[27].piece = PAWN;
	pieces[27].generate_moves = generate_pawn_moves;
	pieces[27].location = 0x63;
	chessboard[6][3] = &pieces[27];

	/* Black's e-pawn */
	pieces[28].isTaken = 0;
	pieces[28].color = BLACK;
	pieces[28].piece = PAWN;
	pieces[28].generate_moves = generate_pawn_moves;
	pieces[28].location = 0x64;
	chessboard[6][4] = &pieces[28];

	/* Black's f-pawn */
	pieces[29].isTaken = 0;
	pieces[29].color = BLACK;
	pieces[29].piece = PAWN;
	pieces[29].generate_moves = generate_pawn_moves;
	pieces[29].location = 0x65;
	chessboard[6][5] = &pieces[29];

	/* Black's g-pawn */
	pieces[30].isTaken = 0;
	pieces[30].color = BLACK;
	pieces[30].piece = PAWN;
	pieces[30].generate_moves = generate_pawn_moves;
	pieces[30].location = 0x66;
	chessboard[6][6] = &pieces[30];

	/* Black's h-pawn */
	pieces[31].isTaken = 0;
	pieces[31].color = BLACK;
	pieces[31].piece = PAWN;
	pieces[31].generate_moves = generate_pawn_moves;
	pieces[31].location = 0x67;
	chessboard[6][7] = &pieces[31];

	for(r = 2; r < 6; r++) {
		for(c = 0; c < 8; c++) {
			chessboard[r][c] = NULL;
		}
	}

	currentTurn = 0;
	white_king_has_moved = 0;
	black_king_has_moved = 0;
	white_qr_has_moved = 0;
	white_kr_has_moved = 0;
	black_qr_has_moved = 0;
	black_kr_has_moved = 0;

	en_passant_possible = 0;
	en_passant_victim = NULL;
	numMoves = 0;
}


/* Generates possible moves for a pawn */
unsigned char generate_pawn_moves(unsigned char moves[], unsigned char startSquare) {
	unsigned char rank, file;
	unsigned char curSquare;
	unsigned char curIndex = 0;
	Color pieceColor = get_color();
	
	rank = startSquare >> 4;
	if(pieceColor == WHITE) {
		if(rank == 1) {	
			for(curSquare = (startSquare + 0x10); curSquare <= (startSquare + 0x20); curSquare = curSquare + 0x10) {
				rank = curSquare >> 4;
				file = curSquare & 0x0f;
				if(!(chessboard[rank][file] == NULL) || (puts_king_in_check(startSquare, curSquare, pieceColor))) {
					break;
				}
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
		else {
			curSquare = startSquare + 0x10;
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if((chessboard[rank][file] == NULL) && (!puts_king_in_check(startSquare, curSquare, pieceColor))) {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}

		/* Check for pawn captures */
		if(!((curSquare = startSquare + 0x0f) & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(!(chessboard[rank][file] == NULL) && (chessboard[rank][file]->color != pieceColor)) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}
			}
		}
		if(!((curSquare = startSquare + 0x11) & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(!(chessboard[rank][file] == NULL) && (chessboard[rank][file]->color != pieceColor)) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}
			}
		}

	}
	else { //pieceColor == BLACK
		if(rank == 6) {
			for(curSquare = (startSquare - 0x10); curSquare >= (startSquare - 0x20); curSquare = curSquare - 0x10) {
				rank = curSquare >> 4;
				file = curSquare & 0x0f;
				if(!(chessboard[rank][file] == NULL) || (puts_king_in_check(startSquare, curSquare, pieceColor))) {
					break;
				}
				moves[curIndex] = curSquare;
				curIndex++;
			}

		}
		else {
			curSquare = startSquare - 0x10;
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if((chessboard[rank][file] == NULL) && (!puts_king_in_check(startSquare, curSquare, pieceColor))) {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}

		/* Check for pawn captures */
		if(!((curSquare = startSquare - 0x0f) & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(!(chessboard[rank][file] == NULL) && (chessboard[rank][file]->color != pieceColor)) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}
			}
		}
		if(!((curSquare = startSquare - 0x11) & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(!(chessboard[rank][file] == NULL) && (chessboard[rank][file]->color != pieceColor)) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}
			}
		}

	}

	/* Check for en passant */
	if(en_passant_possible) {		
		if((startSquare == (en_passant_victim->location + 1)) || (startSquare == (en_passant_victim->location - 1))) {
			if(pieceColor == WHITE) {
				if(!puts_king_in_check(startSquare, en_passant_victim->location + 0x10, pieceColor)) {
					moves[curIndex] = en_passant_victim->location + 0x10;
					curIndex++;
				}
			}
			else { /* pieceColor == BLACK */
				if(!puts_king_in_check(startSquare, en_passant_victim->location - 0x10, pieceColor)) {
					moves[curIndex] = en_passant_victim->location - 0x10;
					curIndex++;
				}
			}
		}		
	}

	return curIndex;
}

/* Generates possible moves for a knight */
unsigned char generate_knight_moves(unsigned char moves[], unsigned char startSquare) {
	unsigned curIndex = 0;
	unsigned char curSquare;
	unsigned char rank, file;
	Color pieceColor = get_color();
	

	if(!((curSquare = startSquare - 0x12) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				return 0;
			}
			else {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	if(!((curSquare = startSquare - 0x21) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				return 0;
			}
			else {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	if(!((curSquare = startSquare - 0x1f) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				return 0;
			}
			else {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	if(!((curSquare = startSquare - 0x0e) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				return 0;
			}
			else {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	if(!((curSquare = startSquare + 0x12) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				return 0;
			}
			else {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	if(!((curSquare = startSquare + 0x21) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				return 0;
			}
			else {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	if(!((curSquare = startSquare + 0x1f) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				return 0;
			}
			else {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	if(!((curSquare = startSquare + 0x0e) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				return 0;
			}
			else {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	return curIndex;
}

/* Generates possible moves for a bishop */
unsigned char generate_bishop_moves(unsigned char moves[], unsigned char startSquare) {
	unsigned curIndex = 0;
	unsigned char curSquare;
	unsigned char rank, file;

	Color pieceColor = get_color();

	/* Check the southwest diagonal */
	for(curSquare = (startSquare-0x11); !(curSquare & BOUNDS); curSquare = curSquare - 0x11) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] == NULL) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				break;
			}
			moves[curIndex] = curSquare;
			curIndex++;
		}
		else {
			if(chessboard[rank][file]->color != pieceColor) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}							
			}
			break;
		}
	}

	/* Check the northeast diagonal */
	for(curSquare = (startSquare+0x11); !(curSquare & BOUNDS); curSquare = curSquare + 0x11) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] == NULL) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				break;
			}
			moves[curIndex] = curSquare;
			curIndex++;
		}
		else {
			if(chessboard[rank][file]->color != pieceColor) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}							
			}
			break;
		}
	}

	/* Check the southeast diagonal */
	for(curSquare = (startSquare-0x0f); !(curSquare & BOUNDS); curSquare = curSquare - 0x0f) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] == NULL) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				break;
			}
			moves[curIndex] = curSquare;
			curIndex++;
		}
		else {
			if(chessboard[rank][file]->color != pieceColor) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}							
			}
			break;
		}
	}

	/* Check the northwest diagonal */
	for(curSquare = (startSquare+0x0f); !(curSquare & BOUNDS); curSquare = curSquare + 0x0f) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] == NULL) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				break;
			}
			moves[curIndex] = curSquare;
			curIndex++;
		}
		else {
			if(chessboard[rank][file]->color != pieceColor) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}							
			}
			break;
		}
	}
	
	return curIndex;
}

/* Generates possible moves for a rook */
unsigned char generate_rook_moves(unsigned char moves[], unsigned char startSquare) {
	unsigned curIndex = 0;
	unsigned char curSquare;
	unsigned char rank, file;

	Color pieceColor = get_color();

	/* Check west along the rank */
	for(curSquare = (startSquare-1); !(curSquare & BOUNDS); curSquare--) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] == NULL) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				break;
			}
			moves[curIndex] = curSquare;
			curIndex++;
		}
		else {
			if(chessboard[rank][file]->color != pieceColor) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}							
			}
			break;
		}
	}

	/* Check east along the rank */
	for(curSquare = (startSquare+1); !(curSquare & BOUNDS); curSquare++) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] == NULL) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				break;
			}
			moves[curIndex] = curSquare;
			curIndex++;
		}
		else {
			if(chessboard[rank][file]->color != pieceColor) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}							
			}
			break;
		}
	}

	/* Check south along the file */
	for(curSquare = (startSquare-0x10); !(curSquare & BOUNDS); curSquare = curSquare - 0x10) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] == NULL) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				break;
			}
			moves[curIndex] = curSquare;
			curIndex++;
		}
		else {
			if(chessboard[rank][file]->color != pieceColor) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}							
			}
			break;
		}
	}

	/* Check north along the file */
	for(curSquare = (startSquare+0x10); !(curSquare & BOUNDS); curSquare = curSquare + 0x10) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] == NULL) {
			if(puts_king_in_check(startSquare, curSquare, pieceColor)) {
				break;
			}
			moves[curIndex] = curSquare;
			curIndex++;
		}
		else {
			if(chessboard[rank][file]->color != pieceColor) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}							
			}
			break;
		}
	}

	return curIndex;
}

/* Generates possible moves for a queen */
unsigned char generate_queen_moves(unsigned char moves[], unsigned char startSquare) {
	unsigned char numBishopMoves = generate_bishop_moves(moves, startSquare);
	return (numBishopMoves + generate_rook_moves(&(moves[numBishopMoves]), startSquare));
}

/* Generates possible moves for a king */
unsigned char generate_king_moves(unsigned char moves[], unsigned char startSquare) {
	unsigned char rank, file;
	unsigned char curSquare;
	unsigned char curIndex = 0;
	Color pieceColor = get_color();

	/* Check for castling */
	if(pieceColor == WHITE) {
		if(!white_king_has_moved && !in_check(WHITE)) {

			/* Queenside castling */
			if(!white_qr_has_moved) {
				if((chessboard[0][3] == NULL) && (chessboard[0][2] == NULL) && (chessboard[0][1] == NULL)) {
					if(!puts_king_in_check(0x04, 0x03, WHITE) && !puts_king_in_check(0x04, 0x02, WHITE)) {
						moves[curIndex] = 0x02;
						curIndex++;
					}
				}
			}

			/* Kingside castling */
			if(!white_kr_has_moved) {
				if((chessboard[0][5] == NULL) && (chessboard[0][6] == NULL)) {
					if(!puts_king_in_check(0x04, 0x05, WHITE) && !puts_king_in_check(0x04, 0x06, WHITE)) {
						moves[curIndex] = 0x06;
						curIndex++;
					}
				}
			}
		}
	}
	else { /* pieceColor == BLACK */
		if(!black_king_has_moved && !in_check(BLACK)) {

			/* Queenside castling */
			if(!black_qr_has_moved) {
				if((chessboard[7][3] == NULL) && (chessboard[7][2] == NULL) && (chessboard[7][1] == NULL)) {
					if(!puts_king_in_check(0x74, 0x73, BLACK) && !puts_king_in_check(0x74, 0x72, BLACK)) {
						moves[curIndex] = 0x72;
						curIndex++;
					}
				}
			}

			/* Kingside castling */
			if(!black_kr_has_moved) {
				if((chessboard[7][5] == NULL) && (chessboard[7][6] == NULL)) {
					if(!puts_king_in_check(0x74, 0x75, BLACK) && !puts_king_in_check(0x74, 0x76, BLACK)) {
						moves[curIndex] = 0x76;
						curIndex++;
					}
				}
			}
		}
	}

	/* Checks the three squares south of the king */
	for(curSquare = (startSquare-0x11); curSquare <= (startSquare-0x0f); curSquare++) {
		if(!(curSquare & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;			
			if(((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) &&
					!((curVariant == 1) && (chessboard[rank][file] != NULL))) {
						if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
								moves[curIndex] = curSquare;
								curIndex++;
				}
			}			
		}
	}

	/* Checks the three squares north of the king */
	for(curSquare = (startSquare+0x0f); curSquare <= (startSquare+0x11); curSquare++) {
		if(!(curSquare & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) &&
					!((curVariant == 1) && (chessboard[rank][file] != NULL))) {
				if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
					moves[curIndex] = curSquare;
					curIndex++;
				}
			}			
		}
	}

	/* Checks the square west of the king */
	if(!((curSquare = startSquare-1) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) &&
					!((curVariant == 1) && (chessboard[rank][file] != NULL))) {
			if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}

	/* Checks the square east of the king */
	if(!((curSquare = startSquare+1) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(((chessboard[rank][file] == NULL) || (chessboard[rank][file]->color != pieceColor)) &&
					!((curVariant == 1) && (chessboard[rank][file] != NULL))) {
			if(!puts_king_in_check(startSquare, curSquare, pieceColor)) {
				moves[curIndex] = curSquare;
				curIndex++;
			}
		}
	}
	return curIndex;
}


/* Generates all possible moves for the current player whose turn it is to move */
int get_all_moves() {
	int i;
	Color player_to_move = get_color();
	int availableMoves = 0;

	if(curVariant == 1) { // In atomic chess, the game ends if the opponent's king is destroyed
		if(player_to_move == WHITE) {
			if(pieces[4].isTaken) { // White's king has been taken
				return -1;
			}
		}
		else {
			if(pieces[20].isTaken) { // Black's king has been taken
				return -2;
			}
		}
	}

	if(player_to_move == WHITE) {
		for(i = 0; i < 16; i++) {
			if(!pieces[i].isTaken) {
				pieces[i].numMoves = pieces[i].generate_moves(pieces[i].moves, pieces[i].location);
				if(pieces[i].numMoves) {
					availableMoves = 1;
				}				
			}
		}
	}
	else { /* player_to_move == BLACK */
		for(i = 16; i < 32; i++) {
			if(!pieces[i].isTaken) {
				pieces[i].numMoves = pieces[i].generate_moves(pieces[i].moves, pieces[i].location);
				if(pieces[i].numMoves) {
					availableMoves = 1;
				}
			}
		}
	}
	return availableMoves;
}


/* Checks if a given move is valid */
int isValidMove(unsigned char startSquare, unsigned char endSquare) {
	unsigned char rank = startSquare >> 4;
	unsigned char file = startSquare & 0x0f;
	if(chessboard[rank][file] == NULL) {
		return 0;
	}
	if(chessboard[rank][file]->color != get_color()) { // Makes the person to move can't move their opponent's pieces
		return 0;
	}

	// Atomic Chess: Make sure nothing can be captured adjacent to the current color's king
	if((curVariant == 1) && (chessboard[endSquare >> 4][endSquare & 0x0f] != NULL)) {
		unsigned char adjacentSquares[8];
		unsigned char kingLocation;
		if(get_color() == WHITE) {
			kingLocation = pieces[4].location;
		}
		else {
			kingLocation = pieces[20].location;
		}

		unsigned char numAdjacentSquares = getAdjacentSquares(kingLocation, adjacentSquares);
		unsigned char n;
		for(n = 0; n < numAdjacentSquares; n++) {
			if(adjacentSquares[n] == endSquare) {
				return 0;
			}
		}

	}

	int i;
	for(i = 0; i < chessboard[rank][file]->numMoves; i++) {
		if(endSquare == chessboard[rank][file]->moves[i]) {
			return 1;
		}
	}
	return 0;
}


/* Performs the given move. Returns 1 if promotion has occurred, 2 if queenside castling occurred, 
 * 4 if kingside castling occurred, 6 if en passant occurred, and 0 else. */
int move(unsigned char startSquare, unsigned char endSquare) {
	unsigned char startRank, startFile, endRank, endFile;
	startRank = startSquare >> 4;
	startFile = startSquare & 0x0f;
	endRank = endSquare >> 4;
	endFile = endSquare & 0x0f;
	int returnValue = 0;
	Color pieceColor = get_color();
	Piece moving_piece = chessboard[startRank][startFile]->piece;

	if((chessboard[endRank][endFile] != NULL) || (moving_piece == PAWN)) {
		numMoves = 0; /* Fifty-move rule count resets since a pawn just moved or a piece was captured */
	}
	else {
		numMoves++;
	}

	if(moving_piece == KING) {
		if(pieceColor == WHITE) {
			white_king_has_moved = 1;
		}
		else {
			black_king_has_moved = 1;
		}

		if(startSquare == 0x04) {
			/* If castling occurs, we also need to move the rook */
			if(endSquare == 0x02) {
				chessboard[0][3] = chessboard[0][0];
				chessboard[0][3]->location = 0x03;
				chessboard[0][0] = NULL;
				white_qr_has_moved = 1;
				returnValue = 2;
			}
			else if(endSquare == 0x06) {
				chessboard[0][5] = chessboard[0][7];
				chessboard[0][5]->location = 0x05;
				chessboard[0][7] = NULL;
				white_kr_has_moved = 1;
				returnValue = 4;
			}
		}
		else if(startSquare == 0x74) {
			/* If castling occurs, we also need to move the rook */
			if(endSquare == 0x72) {
				chessboard[7][3] = chessboard[7][0];
				chessboard[7][3]->location = 0x73;
				chessboard[7][0] = NULL;
				black_qr_has_moved = 1;
				returnValue = 2;
			}
			else if(endSquare == 0x76) {
				chessboard[7][5] = chessboard[7][7];
				chessboard[7][5]->location = 0x75;
				chessboard[7][7] = NULL;
				black_qr_has_moved = 1;
				returnValue = 4;
			}
		}
	}
	else if(moving_piece == PAWN) {
		if((endRank == 7) || (endRank == 0)) {
			returnValue = 1;
		}
		else if(en_passant_possible) {
			if(pieceColor == WHITE) {
				if((chessboard[startRank][startFile]->piece == PAWN) && (endSquare == (en_passant_victim->location + 0x10))) {
					en_passant_victim->isTaken = 1;
					chessboard[endRank - 1][endFile] = NULL;
					returnValue = 6;
				}
			}
			else { /* pieceColor == BLACK */
				if((chessboard[startRank][startFile]->piece == PAWN) && (endSquare == (en_passant_victim->location - 0x10))) {
					en_passant_victim->isTaken = 1;
					chessboard[endRank + 1][endFile] = NULL;
					returnValue = 6;
				}
			}
		}

		/* White pawn moved two squares or black pawn moved two squares */
		if((endSquare - startSquare == 0x20) || (startSquare - endSquare == 0x20)) {
			en_passant_possible = 1;
			en_passant_victim = chessboard[startRank][startFile];
		}		
		else {
			en_passant_possible = 0;
			en_passant_victim = NULL;
		}
	}


	/* Set any necessary flags for rooks */
	if((startSquare == 0x00) || (endSquare == 0x00)) {
		white_qr_has_moved = 1;
	}
	if((startSquare == 0x07) || (endSquare == 0x07)) {
		white_kr_has_moved = 1;
	}
	if((startSquare == 0x70) || (endSquare == 0x70)) {
		black_qr_has_moved = 1;
	}
	if((startSquare == 0x77) || (endSquare == 0x77)) {
		black_kr_has_moved = 1;
	}


	/* Update the status of the chessboard */
	if(chessboard[endRank][endFile] != NULL) {
		chessboard[endRank][endFile]->isTaken = 1;
		if(curVariant == 1) {
			chessboard[startRank][startFile]->isTaken = 1;
			chessboard[startRank][startFile] = NULL;
			chessboard[endRank][endFile] = NULL;

			unsigned char adjacentSquares[8];
			unsigned char numAdjacentSquares = getAdjacentSquares(endSquare, adjacentSquares);
			unsigned char n;
			unsigned char curRank, curFile;
			for(n = 0; n < numAdjacentSquares; n++) {
				curRank = adjacentSquares[n] >> 4;
				curFile = adjacentSquares[n] & 0x0f;
				if((chessboard[curRank][curFile] != NULL) && (chessboard[curRank][curFile]->piece != PAWN)) {
					chessboard[curRank][curFile]->isTaken = 1;
					chessboard[curRank][curFile] = NULL;
				}
			}			
			return 7;
		}
	}
	chessboard[endRank][endFile] = chessboard[startRank][startFile];
	chessboard[endRank][endFile]->location = endSquare;
	chessboard[startRank][startFile] = NULL;
	if(curVariant == 2) {
		if(pieceColor == WHITE) {
			if(in_check(BLACK)) {
				return 12;
			}
		}
		else {
			if(in_check(WHITE)) {
				return 13;
			}
		}
	}
	return returnValue;
}


/* Promotes the pawn at the promotion square to the given piece. Invalid piece arguments should never occur. */
void promote(unsigned char promotionSquare, Piece newPiece) {
	unsigned char rank = promotionSquare >> 4;
	unsigned char file = promotionSquare & 0x0f;
	chessboard[rank][file]->piece = newPiece;
	switch(newPiece) {
		case KNIGHT:
			chessboard[rank][file]->generate_moves = generate_knight_moves;
			break;
		case BISHOP:
			chessboard[rank][file]->generate_moves = generate_bishop_moves;
			break;
		case ROOK:
			chessboard[rank][file]->generate_moves = generate_rook_moves;
			break;
		default:
			chessboard[rank][file]->generate_moves = generate_queen_moves;
	}
}

/* Helper functions */


/* Returns the color whose move it is */
Color get_color() {
	if(currentTurn) {
		return BLACK;
	}
	return WHITE;
}

/* Returns 1 if the king is in check. Returns 0 otherwise */
int in_check(Color kingColor) {
	if(curVariant == 1) { //In atomic chess, the king can be put in check
		return 0;
	}
	unsigned char kingLocation;
	unsigned char curSquare;
	unsigned char rank, file;
	if(kingColor == WHITE) {
		kingLocation = pieces[4].location;
	}
	else { /* kingColor == BLACK */
		kingLocation = pieces[20].location;
	}

	/* Check knight moves around the king */
	if(!((curSquare = kingLocation - 0x12) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL){
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KNIGHT)) {
				return 1;
			}
		}
	}
	if(!((curSquare = kingLocation - 0x21) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL){
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KNIGHT)) {
				return 1;
			}
		}
	}
	if(!((curSquare = kingLocation - 0x1f) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL){
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KNIGHT)) {
				return 1;
			}
		}
	}
	if(!((curSquare = kingLocation - 0x0e) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL){
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KNIGHT)) {
				return 1;
			}
		}
	}
	if(!((curSquare = kingLocation + 0x12) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL){
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KNIGHT)) {
				return 1;
			}
		}
	}
	if(!((curSquare = kingLocation + 0x21) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL){
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KNIGHT)) {
				return 1;
			}
		}
	}
	if(!((curSquare = kingLocation + 0x1f) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL){
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KNIGHT)) {
				return 1;
			}
		}
	}
	if(!((curSquare = kingLocation + 0x0e) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL){
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KNIGHT)) {
				return 1;
			}
		}
	}

	/* Check the southwest diagonal */
	for(curSquare = (kingLocation-0x11); !(curSquare & BOUNDS); curSquare = curSquare - 0x11) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if(chessboard[rank][file]->color != kingColor) {
				if((chessboard[rank][file]->piece == BISHOP) || (chessboard[rank][file]->piece == QUEEN)) {
					return 1;
				}
			}
			break;
		}		
	}
	/* Check the northeast diagonal */
	for(curSquare = (kingLocation+0x11); !(curSquare & BOUNDS); curSquare = curSquare + 0x11) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if(chessboard[rank][file]->color != kingColor) {
				if((chessboard[rank][file]->piece == BISHOP) || (chessboard[rank][file]->piece == QUEEN)) {
					return 1;
				}
			}
			break;
		}		
	}
	/* Check the southeast diagonal */
	for(curSquare = (kingLocation-0x0f); !(curSquare & BOUNDS); curSquare = curSquare - 0x0f) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if(chessboard[rank][file]->color != kingColor) {
				if((chessboard[rank][file]->piece == BISHOP) || (chessboard[rank][file]->piece == QUEEN)) {
					return 1;
				}
			}
			break;
		}		
	}
	/* Check the northwest diagonal */
	for(curSquare = (kingLocation+0x0f); !(curSquare & BOUNDS); curSquare = curSquare + 0x0f) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if(chessboard[rank][file]->color != kingColor) {
				if((chessboard[rank][file]->piece == BISHOP) || (chessboard[rank][file]->piece == QUEEN)) {
					return 1;
				}
			}
			break;
		}		
	}


	/* Check west along the rank */
	for(curSquare = (kingLocation-1); !(curSquare & BOUNDS); curSquare--) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if(chessboard[rank][file]->color != kingColor) {
				if((chessboard[rank][file]->piece == ROOK) || (chessboard[rank][file]->piece == QUEEN)) {
					return 1;
				}
			}
			break;
		}
	}
	/* Check east along the rank */
	for(curSquare = (kingLocation+1); !(curSquare & BOUNDS); curSquare++) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if(chessboard[rank][file]->color != kingColor) {
				if((chessboard[rank][file]->piece == ROOK) || (chessboard[rank][file]->piece == QUEEN)) {
					return 1;
				}
			}
			break;
		}
	}
	/* Check south along the file */
	for(curSquare = (kingLocation-0x10); !(curSquare & BOUNDS); curSquare = curSquare - 0x10) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if(chessboard[rank][file]->color != kingColor) {
				if((chessboard[rank][file]->piece == ROOK) || (chessboard[rank][file]->piece == QUEEN)) {
					return 1;
				}
			}
			break;
		}
	}
	/* Check north along the rank */
	for(curSquare = (kingLocation+0x10); !(curSquare & BOUNDS); curSquare = curSquare + 0x10) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if(chessboard[rank][file]->color != kingColor) {
				if((chessboard[rank][file]->piece == ROOK) || (chessboard[rank][file]->piece == QUEEN)) {
					return 1;
				}
			}
			break;
		}
	}

	/* Checks the three squares south of the king */
	for(curSquare = (kingLocation-0x11); curSquare <= (kingLocation-0x0f); curSquare++) {
		if(!(curSquare & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(chessboard[rank][file] != NULL) {
				if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KING)) {
					return 1;
				}
			}			
		}
	}

	/* Checks the three squares north of the king */
	for(curSquare = (kingLocation+0x0f); curSquare <= (kingLocation+0x11); curSquare++) {
		if(!(curSquare & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(chessboard[rank][file] != NULL) {
				if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KING)) {
					return 1;
				}
			}			
		}
	}

	/* Check the square to the west of the king */
	if(!((curSquare = kingLocation - 1) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KING)) {
				return 1;
			}
		}
	}
	/* Check the square to the west of the king */
	if(!((curSquare = kingLocation + 1) & BOUNDS)) {
		rank = curSquare >> 4;
		file = curSquare & 0x0f;
		if(chessboard[rank][file] != NULL) {
			if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == KING)) {
				return 1;
			}
		}
	}
	
	/* Check pawn moves around the king */
	if(kingColor == WHITE) {
		if(!((curSquare = kingLocation + 0x0f) & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(chessboard[rank][file] != NULL) {
				if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == PAWN)) {
					return 1;
				}
			}
		}
		if(!((curSquare = kingLocation + 0x11) & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(chessboard[rank][file] != NULL) {
				if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == PAWN)) {
					return 1;
				}
			}
		}
	}
	else { /* kingColor == black */
		if(!((curSquare = kingLocation - 0x0f) & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(chessboard[rank][file] != NULL) {
				if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == PAWN)) {
					return 1;
				}
			}
		}
		if(!((curSquare = kingLocation - 0x11) & BOUNDS)) {
			rank = curSquare >> 4;
			file = curSquare & 0x0f;
			if(chessboard[rank][file] != NULL) {
				if((chessboard[rank][file]->color != kingColor) && (chessboard[rank][file]->piece == PAWN)) {
					return 1;
				}
			}
		}
	}

	return 0;
}

/* Returns 1 if the potential piece move will put the king in check. Returns 0 otherwise */
int puts_king_in_check(unsigned char startSquare, unsigned char endSquare, Color pieceColor) {
	if(curVariant == 1) { //In atomic chess, the king can be put in check
		return 0;
	}
	int result = 0;
	unsigned char startRank, startFile, endRank, endFile;
	startRank = startSquare >> 4;
	startFile = startSquare & 0x0f;
	endRank = endSquare >> 4;
	endFile = endSquare & 0x0f;

	/* Check for en passant */
	if(en_passant_possible) {		
		if((startSquare == (en_passant_victim->location + 1)) || (startSquare == (en_passant_victim->location - 1))) {
			chessboard[en_passant_victim->location >> 4][en_passant_victim->location & 0x0f] = NULL;
		}		
	}

	chess_piece* start = chessboard[startRank][startFile];
	chess_piece* end = chessboard[endRank][endFile];

	chessboard[startRank][startFile] = NULL;
	chessboard[endRank][endFile] = start;
	start->location = endSquare;

	if(in_check(pieceColor)) {
		result = 1;
	}

	start->location = startSquare;
	chessboard[endRank][endFile] = end;
	chessboard[startRank][startFile] = start;

	/* Account for the en passant victim pawn being removed if en passant is the tested move */
	if(en_passant_possible) {		
		if((startSquare == (en_passant_victim->location + 1)) || (startSquare == (en_passant_victim->location - 1))) {
			chessboard[en_passant_victim->location >> 4][en_passant_victim->location & 0x0f] = en_passant_victim;
		}		
	}
	
	return result;
}

/* Populates the argument array with all squares on the board that are adjacent to the given square.
 * Returns the number of adjacent squares (i.e. the number of valid squares returned in the adjacentSquares array) */
unsigned char getAdjacentSquares(unsigned char startSquare, unsigned char adjacentSquares[]) {
	unsigned char numSquares = 0;
	unsigned char adjacentSquare;

	/* Checks the three squares south of the given square */
	for(adjacentSquare = (startSquare-0x11); adjacentSquare <= (startSquare-0x0f); adjacentSquare++) {
		if(!(adjacentSquare & BOUNDS)) {
			adjacentSquares[numSquares] = adjacentSquare;
			numSquares++;			
		}
	}

	/* Checks the three squares north of the king */
	for(adjacentSquare = (startSquare+0x0f); adjacentSquare <= (startSquare+0x11); adjacentSquare++) {
		if(!(adjacentSquare & BOUNDS)) {
			adjacentSquares[numSquares] = adjacentSquare;
			numSquares++;			
		}
	}

	/* Checks the square west of the king */
	if(!((adjacentSquare = startSquare-1) & BOUNDS)) {
		adjacentSquares[numSquares] = adjacentSquare;
		numSquares++;
	}

	/* Checks the square east of the king */
	if(!((adjacentSquare = startSquare+1) & BOUNDS)) {
		adjacentSquares[numSquares] = adjacentSquare;
		numSquares++;
	}

	return numSquares;
}
