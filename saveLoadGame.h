// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: defines and function declarations related to saving to and loading from binary files
#ifndef SAVELOADGAME_H_INCLUDED
#define SAVELOADGAME_H_INCLUDED

#include "game.h"

#define MAX_SAVE_FILE_PATH_LENGTH 100
#define MAX_SAVED_GAME_FILE_LENGTH 15

#define SAVE_GAME_MSG "SAVE NEW GAME"
#define SAVE_INSRTUCTIONS_MSG "SELECT A FILE TO SAVE:"
#define LOAD_INSTRUCTIONS_MSG "SELECT A FILE TO LOAD:"

// saving to .bin file
void saveGame(Hangman *hangman, char* fileName, MainSDLVariables *mainSDL);

// loading from .bin file
void loadGame(Hangman *hangman, char* fileName);

// reading saved game files from a folder
char** readFiles(const char *path, short *counter);

#endif // SAVELOADGAME_H_INCLUDED