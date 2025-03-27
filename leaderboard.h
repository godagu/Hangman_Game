// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: defines, fucntions' and struct declarations related to leaderboard functionality of the game

#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

#include "saveLoadGame.h"

#define MAX_HIGHSCORES_SHOWN 8
#define MAX_NAME_LENGTH 15

#define EMPTY_NAME "----"
#define ERROR_NUMBER -404

#define PLAYER_NAME_Y 30
#define INPUT_NAME_Y 50
#define SCORE_X 200

#define CONGRATS_ON_HIGHSCORE_MSG "CONGRATS! YOU'VE REACHED A HIGHSCORE."
#define WRITE_PLAYER_NAME_MSG "PLEASE ENTER YOUR NAME:"
#define TITLE_LEADEARBOARD_MSG "LEADERBOARD"

// struct for a player (for adding to leaderboard)
typedef struct Player{
    int nameLength;
    char* name;
    int highscore;
} Player;

// alocating memory, setting all elements to default names and scores
Player** initializeLeaderboard();

// saving to .bin file
void saveLeaderboard(Player **leaderboard);

// sorting the leaderboard in descending order
void sortLeaderboard(Player **leaderboard);

// loading from .bin file, return the high score benchmark
int loadLeaderboard(Player **leaderboard);

// add a player and their highscore to the leaderboard
void updateLeaderboard(char* name, int highscore, Player **leaderboard);

// get input for the player's name
char* getName(MainSDLVariables *mainSDL);

// show leaderboard
void renderLeaderboard(MainSDLVariables *mainSDL, Player **leaderboard);

// free leaderboard
void freeLeaderboard(Player **leaderboard);

#endif // LEADERBOARD_H_INCLUDED