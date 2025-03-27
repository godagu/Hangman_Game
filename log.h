// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: defines and function declarations related to logging the elapsed time of the game run

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include "leaderboard.h"

#define LOG_STARTED_MSG "Program started %s"
#define LOG_ENDED_MSG "Program ended %s"
#define TIME_ELAPSED_MSG "Program ran for %g seconds\n"

// starts logging time and writes to .txt time
void startLog();

// ends logging, counts the time elapsed, writes to .txt file
void endLog();

#endif // LOG_H_INCLUDED;