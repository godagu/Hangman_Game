// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: definitions of functions declared in saveLoadGame.h
#include "saveLoadGame.h"

void saveGame(Hangman *hangman, char* fileName, MainSDLVariables *mainSDL){
    if(!fileName){
        return;
    }

    char* tempName = malloc(MAX_SAVE_FILE_PATH_LENGTH);
    if(!tempName){
        return;
    }

    char* pathFile = malloc(MAX_SAVE_FILE_PATH_LENGTH);
    if(!pathFile){
        return;
    }

    if(strcmp(fileName, SAVE_GAME_MSG) == 0){
        tempName = getFileName(mainSDL);
        if(!tempName){
            return;
        }

        sprintf(pathFile, LOAD_GAME_FILE, tempName);
        free(tempName);
        // free(fileName);
    }
    else{
        sprintf(pathFile, LOAD_GAME_FILE, fileName);
        // free(fileName);
    }

    FILE *ptr = NULL;
    ptr = fopen(pathFile, "wb");
    if(!ptr){
        free(pathFile);
        return;
    }

    free(pathFile);

    fwrite(hangman->word, sizeof(char), MAX_WORD_LENGTH + 1, ptr);
    fwrite(&(hangman->wordLength), sizeof(int), 1, ptr);
    fwrite(&(hangman->guessCountWrong), sizeof(int), 1, ptr);
    fwrite(&(hangman->guessCountRight), sizeof(int), 1, ptr);
    fwrite(&(hangman->guessCountTotal), sizeof(int), 1, ptr);
    fwrite(hangman->guessedLetterIndexes, sizeof(int), MAX_WORD_LENGTH, ptr);
    fwrite(&(hangman->guessedLetterIndexCount), sizeof(int), 1, ptr);
    fwrite(hangman->guessedLetters, sizeof(char), MAX_WORD_LENGTH + 1, ptr);
    fwrite(&(hangman->correctGuessInARow), sizeof(int), 1, ptr);
    fwrite(&(hangman->correctGuessAdded), sizeof(bool), 1, ptr);
    fwrite(&(hangman->started), sizeof(bool), 1, ptr);

    fclose(ptr);
}

void loadGame(Hangman *hangman, char* fileName){
    if(!fileName){
        return;
    }

    char* pathFile = malloc(MAX_SAVE_FILE_PATH_LENGTH);
    if(!pathFile){
        return;
    }

    sprintf(pathFile, LOAD_GAME_FILE, fileName);
    FILE *ptr = NULL;
    ptr = fopen(pathFile, "rb");
    if(!ptr){
        return;
    }

    if(!hangman->guessedLetterIndexes){
        hangman->guessedLetterIndexes = calloc(MAX_WORD_LENGTH + 1, sizeof(int));
        if(!hangman->guessedLetterIndexes){
            return;
        }
    }

    if(!hangman->guessedLetters){
        hangman->guessedLetters = calloc(sizeof(char), (MAX_WORD_LENGTH + MAX_WRONG_GUESS_COUNT) + 1);
        if(!hangman->guessedLetters){
            return;
        }
    }

    if(!hangman->word){
        hangman->word = calloc(MAX_WORD_LENGTH, sizeof(char));
        if(!hangman->word){
            return;
        }
    }

    fread(hangman->word, sizeof(char), MAX_WORD_LENGTH + 1, ptr);
    fread(&(hangman->wordLength), sizeof(int), 1, ptr);
    fread(&(hangman->guessCountWrong), sizeof(int), 1, ptr);
    fread(&(hangman->guessCountRight), sizeof(int), 1, ptr);
    fread(&(hangman->guessCountTotal), sizeof(int), 1, ptr);
    fread(hangman->guessedLetterIndexes, sizeof(int), MAX_WORD_LENGTH, ptr);
    fread(&(hangman->guessedLetterIndexCount), sizeof(int), 1, ptr);
    fread(hangman->guessedLetters, sizeof(char), MAX_WORD_LENGTH + 1, ptr);
    fread(&(hangman->correctGuessInARow), sizeof(int), 1, ptr);
    fread(&(hangman->correctGuessAdded), sizeof(bool), 1, ptr);
    fread(&(hangman->started), sizeof(bool), 1, ptr);

    fclose(ptr);
}

char** readFiles(const char *path, short *counter){
    char** savedGameFiles = NULL;
    *counter = 0;

    struct dirent *dp;
    DIR *dir = opendir(path);

    if(!dir){
        return NULL;
    }

    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, ".DS_Store") != 0){
            savedGameFiles = realloc(savedGameFiles, sizeof(char*) * ((*counter) + 1));
            if(!savedGameFiles){
                return NULL;
            }

            savedGameFiles[*counter] = malloc(strlen(dp->d_name) + 1);
            if(!savedGameFiles[*counter]){
                return NULL;
            }

            strcpy(savedGameFiles[*counter], dp->d_name);
            ++(*counter);
        }
    }

    closedir(dir);
    return savedGameFiles;
}