/* 
 * Chess user-level program for the ECE 391 OS
 */

#include "chessboard.h"
#include "ece391syscall.h"
#include "ece391support.h"

//----------------------------------------------------------------------------------------//
//
//                                Main function
//
//----------------------------------------------------------------------------------------//
int main(void)
{	
	unsigned short int data;
	unsigned short int compressedStartSquare, compressedEndSquare;
	unsigned char startSquare, endSquare;
	unsigned char keycode = '\0', prev_keycode;	// Holds the previous poll's keypress

	uint32_t read_ret;

	unsigned char curSquare = 0x04; // Current square being highlighted
	unsigned char nextSquare; // Square that will be highlighted upon an arrow press 
	unsigned char starting_square_selected = 0;
	unsigned char ending_square_selected = 0;
	unsigned char moveMade = 0; // Set to 1 right after a move has been successfully made, 0 else
	unsigned char gameEnded = 0; //Set to 1 if the game is over due to resignation, checkmate, stalemate, or draw agreement. 0 else
	unsigned char drawOffer = 0; //Set to 1 for white offering a draw, 2 for blcak offering a draw, and 0 else.
	unsigned char promotion = 0;
	int result;
	int curVariant = 0; // Set to 0 for normal chess, set to 1 for atomic chess, set to 2 for kriegspiel
	init_chessboard(0);

	// Insert system call to set keyboard mode such that read() does not block
	ece391_set_keyboard_mode(1);

	// Main game loop
	while(1) {		
		if(gameEnded) {
			
			//Reset the game board so that another game can be played
			starting_square_selected = 0;
			ending_square_selected = 0;
			gameEnded = 0;
			curSquare = 0x04;
			promotion = 0;
			do {
				ece391_fdputs(1, (uint8_t*)"Choose which chess variant you wish to play.\nSelect 0 for normal chess, 1 for atomic chess, and 2 for kriegspiel. \n");
				if(-1 == ece391_read(0, (uint8_t*)&curVariant, 1))
				{
					 ece391_fdputs (1, (uint8_t*)"Can't read keypress.\n");
		        	return -1;
				}
				curVariant -= (int)'0'; // convert to integer representation. Necessary as it will be input as a character
			} while((curVariant < 0) || (curVariant > 2));
			data = curVariant;
			init_chessboard(curVariant);
		}

		if((result = get_all_moves()) <= 0) {
			if(result == -1) { // Black has taken White's king
				data = 0x0005;
			}
			else if(result == -2) { // White has taken Black's king
				data = 0x0004;
			}
			else { // result == 0
				if(in_check(get_color())) {
					/* Checkmate */
					if(currentTurn) { // White has checkmated Black
						data = 0x0004;
					}
					else { // Black has checkmated White
						data = 0x0005;
					}
				}
				else {
					/* Stalemate */
					data = 0x0003;
				}
			}
			gameEnded = 1;
		}


	//-----------------------------------get keycode value------------------------------------------------//
	
		while(!moveMade && !gameEnded)
		{
			prev_keycode = keycode; // Store the previously pressed key

			// poll for a new keypress
			if(-1 == (read_ret = ece391_read(0, (uint8_t*)&keycode, 1)))
			{
		        ece391_fdputs (1, (uint8_t*)"Can't read keypress.\n");
		        return -1;
		    }

		    else if(read_ret == 0) // Nothing was read -> no key was pressed
		    {
		    	keycode = '\0';
		    }

			if((prev_keycode != '\0') && (keycode == '\0')) { /* A key was just released */

				if(promotion) { //Promotion key presses
					switch(prev_keycode) {
						case 'b': // Bishop ('b')
							data = 0x0002;
							promote(endSquare, BISHOP);
							break;

						case 'n': // Knight ('n')
							data = 0x0003;
							promote(endSquare, KNIGHT);
							break;

						case 'q': // Queen ('q')
							data = 0x0000;
							promote(endSquare, QUEEN);
							break;

						case 'r': // Rook ('r')
							data = 0x0001;
							promote(endSquare, ROOK);
							break;

						default: // Dummy value of data so as to avoid sending transmissions before a piece has been selected
							data = 0xffff;
							break;
					}

					//Only transmit if a piece has been selected
					if(data != 0xffff) {

						compressedEndSquare = ((7 -(endSquare >> 4)) << 3 & 0x38) | (endSquare & 0x07);
						data |= (compressedEndSquare << 3);

						//Set the color bit of the transmission (transmission[2]): (0 for black, 1 for white)
						if(!currentTurn) {
							data |= 0x0004;
						}

						// Handshake with hardware
						while(*to_sw_sig != 2);

						// Transmit the chosen piece
						IOWR(TRANSMISSION_BASE, 0, data & 0xffff); 
						*to_hw_sig = 2;

						if(currentTurn && (drawOffer == 1)) {
							drawOffer = 0;
						}
						else if(!currentTurn && (drawOffer == 2)) {
							drawOffer = 0;
						}

						currentTurn = ~currentTurn;
						moveMade = 1;
						promotion = 0;
					}	
				}	

				else { //Other non promotion related key presses
					switch(prev_keycode) {
						case 'l':	 // Current player offered a draw ('l')
							if(numMoves >= 100) {
								gameEnded = 1; // Game ends by 50-move rule
								data = 0x0003;
							}
							else {
								switch(drawOffer) {
									case 0: // No one has offered a draw
										if(currentTurn) { // Black just offered a draw
											drawOffer = 2;
											data = 0x0007;
										}
										else { // White just offered a draw
											drawOffer = 1;
											data = 0x0006;
										}

										//Handshake with hardware
										*to_hw_sig = 1;
										while(*to_sw_sig != 1);

										IOWR(TRANSMISSION_BASE, 0, data & 0xffff); 
										*to_hw_sig = 3;

										break;
									case 1: // White had offered a draw already
										if(currentTurn){ // Black agreed to a draw
											gameEnded = 1;
											data = 0x0003;
										}
										break;
									case 2: // Black had offered a draw already
										if(!currentTurn) { // White agreed to a draw
											gameEnded = 1;
											data = 0x0003;
										}
										break;
								}
							}							
							break;							

						case 'g': // Current player resigned ('g')
							if(currentTurn) { //Black resigned
								data = 0x0004;
							}
							else { // White resigned
								data = 0x0005;
							}
							gameEnded = 1;
							break;

						case 'z': // Current player is selecting a square for a move ('z')
							if(!starting_square_selected) {
								startSquare = curSquare;
								starting_square_selected = 1;
							}
							else {
								endSquare = curSquare;
								ending_square_selected = 1;
							}
							break;

						case 'd': // Right arrow key
							if(!((nextSquare = curSquare + 1) & BOUNDS)) {
								compressedStartSquare = ((7 - (curSquare >> 4)) << 3 & 0x38) | (curSquare & 0x07);
								compressedEndSquare = ((7 -(nextSquare >> 4)) << 3 & 0x38) | (nextSquare & 0x07);
								data = 0x0000 | (compressedStartSquare << 9) | (compressedEndSquare << 3);
								if(starting_square_selected && (curSquare == startSquare)) {
									data |= 0x0001;	// Keeps the start square selected
								}
							
								//Handshake with hardware
								*to_hw_sig = 1;
								while(*to_sw_sig != 1);

								//Send transmission to unhighlight the current square and highlight the next one
								IOWR(TRANSMISSION_BASE, 0, data & 0xffff); 
								*to_hw_sig = 3;
							
								curSquare = nextSquare;
							}
							break;

						case 'a': // Left arrow key
							if(!((nextSquare = curSquare -1) & BOUNDS)) {
								compressedStartSquare = ((7 - (curSquare >> 4)) << 3 & 0x38) | (curSquare & 0x07);
								compressedEndSquare = ((7 -(nextSquare >> 4)) << 3 & 0x38) | (nextSquare & 0x07);
								data = 0x0000 | (compressedStartSquare << 9) | (compressedEndSquare << 3);
								if(starting_square_selected && (curSquare == startSquare)) {
									data |= 0x0001;	// Keeps the start square selected
								}
						
								//Handshake with hardware
								*to_hw_sig = 1;
								while(*to_sw_sig != 1);

								//Send transmission to unhighlight the current square and highlight the next one
								IOWR(TRANSMISSION_BASE, 0, data & 0xffff); 
								*to_hw_sig = 3;
							
								curSquare = nextSquare;
							}
							break;

						case 's': // Down arrow key
							if(!((nextSquare = curSquare - 0x10) & BOUNDS)) {
								compressedStartSquare = ((7 - (curSquare >> 4)) << 3 & 0x38) | (curSquare & 0x07);
								compressedEndSquare = ((7 -(nextSquare >> 4)) << 3 & 0x38) | (nextSquare & 0x07);
								data = 0x0000 | (compressedStartSquare << 9) | (compressedEndSquare << 3);
								if(starting_square_selected && (curSquare == startSquare)) {
									data |= 0x0001;	// Keeps the start square selected
								}
						
								//Handshake with hardware
								*to_hw_sig = 1;
								while(*to_sw_sig != 1);

								//Send transmission to unhighlight the current square and highlight the next one
								IOWR(TRANSMISSION_BASE, 0, data & 0xffff); 
								*to_hw_sig = 3;
								
								curSquare = nextSquare;
							}
							break;

						case 'w': // Up arrow key
							if(!((nextSquare = curSquare + 0x10) & BOUNDS)) {
								compressedStartSquare = ((7 - (curSquare >> 4)) << 3 & 0x38) | (curSquare & 0x07);
								compressedEndSquare = ((7 -(nextSquare >> 4)) << 3 & 0x38) | (nextSquare & 0x07);
								data = 0x0000 | (compressedStartSquare << 9) | (compressedEndSquare << 3);
								if(starting_square_selected && (curSquare == startSquare)) {
									data |= 0x0001;	// Keeps the start square selected
								}
						
								//Handshake with hardware
								*to_hw_sig = 1;
								while(*to_sw_sig != 1);

								//Send transmission to unhighlight the current square and highlight the next one
								IOWR(TRANSMISSION_BASE, 0, data & 0xffff); 
								*to_hw_sig = 3;
							
								curSquare = nextSquare;
							}
							break;
						}
					}

			
			// If arrow key, let transmission vector D (16-bit string) be updated as follows:
			// D[15] = 0, and then D[14:9] will hold the address of the square we had previously highlighted
			// and D[8:3] will hold the address of the square to be highlighted
			// The encoding of D[2:0] is as follows:
			// 000 to highlight the ending square and deselect the starting square (when no square has been selected)
			// 001 to highlight both the ending square and the starting square (when the start square has been selected already)
			// 010 to send a "dummy transmission" to deselect a previously selected square (when selecting an end square)
			// 011 for unconditional draws such as stalemate and the 50-move rule and when both players agreed to a draw
			// 100 if black resigns (White wins)
			// 101 if white resigns (black wins)
			// 110 if white offers draw.
			// 111 if black offers draw.

			// Extra encodings for kriegspiel
			// 1100 if white is giving check
			// 1101 if black is giving check

			// Also need to update the currently selected square in software
			// Transmit to hardware (set to_hw_sig = 3, and wait for to_sw_sig = 1. Then set to_hw_sig = 1,
			// wait for to_sw_sig = 1, and return to beginning.  This is done to prevent cascading transmissions.)
		
		
				if(starting_square_selected && ending_square_selected) {
				// Determine whether the (starting_square, ending_square) combination constitutes a valid move.
				// If so, set D[15] = 1 (move transmission).  Set D[14:9] to starting square.
				// Set D[8:3] to ending square.  Set D[2:0] based on whether we have 
				// castling, en passant, or pawn promotion.  Wait for handshaking sequence as before.

					if(isValidMove(startSquare, endSquare)) {
						compressedStartSquare = ((7 - (startSquare >> 4)) << 3 & 0x38) | (startSquare & 0x07);
						compressedEndSquare = ((7 -(endSquare >> 4)) << 3 & 0x38) | (endSquare & 0x07);
						result = move(startSquare, endSquare);
						if((curVariant == 2)  && ((result == 12) || (result == 13))) {
							data = result;
						}
						else {
							data = 0x8000 | (compressedStartSquare << 9) | (compressedEndSquare << 3) | result;
						}
				
						// Handshake with hardware
						*to_hw_sig = 1;
						while(*to_sw_sig != 1);
						IOWR(TRANSMISSION_BASE, 0, data & 0xffff); //The transmission code will be something else 
						*to_hw_sig = 3;		

						if((data & 0x0007) == 0x0001) {
							promotion = 1;
						}	
						else {
							if(currentTurn && (drawOffer == 1)) {
								drawOffer = 0;
							}
							else if(!currentTurn && (drawOffer == 2)) {
								drawOffer = 0;
							}
							currentTurn = ~currentTurn;
							moveMade = 1;
						}
					}			
					else {
				// Else if the move is not valid, we'll want to send a signal to display some sort of font data
				// Informing the user they need to select another move.
				// Add this when we do font data.
				// For now, just return to the beginning of the loop without sending transmission (keep to_hw_sig where it is)
						ece391_fdputs(1, (uint8_t*)"Invalid move.\n");
					}
					starting_square_selected = 0;
					ending_square_selected = 0;
				}
				prev_keycode = 0;
			}

			if(*to_sw_sig == 3) { /* If the reset button is pressed, set the gameEnded flag */				
				starting_square_selected = 0;
				ending_square_selected = 0;
				moveMade = 1;
				gameEnded = 0;
				curSquare = 0x04;
				promotion = 0;
				do {
					ece391_fdputs(1, (uint8_t*)"Choose which chess variant you wish to play.\nSelect 0 for normal chess, 1 for atomic chess, and 2 for kriegspiel. \n");
					if(-1 == ece391_read(0, (uint8_t*)&curVariant, 1))
					{
						 ece391_fdputs (1, (uint8_t*)"Can't read keypress.\n");
			        	return -1;
					}
					curVariant -= (int)'0'; // convert to integer representation. Necessary as it will be input as a character
				} while((curVariant < 0) || (curVariant > 2));
				data = curVariant;
				IOWR(TRANSMISSION_BASE, 0, data & 0xffff);
				*to_hw_sig = 1;
				init_chessboard(curVariant);
			}		

		}//Stop polling for keyboard input to check if checkmate or stalemate has occurred
		moveMade = 0;
	}
	return 0;
}

