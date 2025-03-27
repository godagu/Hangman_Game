// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: definitions of functions declared in log.h

#include "log.h"

static time_t start;
void startLog(){
    FILE *ptr; 
    ptr = fopen(LOG_FILE_PATH, "a+");
    if(!ptr){
        ptr = fopen(LOG_FILE_PATH, "w+");
        if(!ptr){
            return;
        }
    }

    start = time(NULL);
    struct tm *starTime = localtime(&start);
    fprintf(ptr, LOG_STARTED_MSG, asctime(starTime));

    fclose(ptr);
}

void endLog(){
    FILE *ptr; 
    ptr = fopen(LOG_FILE_PATH, "a+");
    if(!ptr){
        ptr = fopen(LOG_FILE_PATH, "w+");
        if(!ptr){
            return;
        }
    }

    time_t end = time(NULL);
    struct tm *endTime = localtime(&end);
    fprintf(ptr, LOG_ENDED_MSG, asctime(endTime));

    double diff = difftime(end, start);
    fprintf(ptr, TIME_ELAPSED_MSG, diff);

    fclose(ptr);
}
