// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: main header file, consisting of the majority defines, functions' and structs' declarations

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdio.h>
#include "include/SDL2/SDL.h" // changed for mac
#include "include/SDL2/SDL_ttf.h" // changed for mac
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>

#define HEIGHT 480
#define WIDTH 640

#define FONT_NAME "Firestarter.ttf"
#define FONT_SIZE 40

#define DEFAULT_NAME "PLAYER"

#define MAX_WORD_LENGTH 14
#define MAX_GAMES_SAVED 8
#define MAX_IMAGE_FILE_PATH_LENGTH 20
#define MAX_GUESSED_LETTERS_IN_A_LINE 13
#define MAX_INSTRUCTIONS_LENGTH 23
#define INSTRUCTION_COUNT 4
#define MAX_WRONG_GUESS_COUNT 9
#define MAX_HIGHSCORE_CHAR_SIZE 5

#define DASHES_X 20
#define DASHES_Y 50
#define LETTER_X 20
#define LETTER_Y 40
#define HANGMAN_X 0
#define HANGMAN_Y 60
#define HANGMAN_W 400
#define HANGMAN_H 400
#define GAME_OVER_Y 50
#define ANSWER_Y 50
#define CONGRATS_X 250
#define CONGRATS_Y 100
#define GUESSED_LTTRS_X 245
#define GUESSED_LTTRS_Y 100
#define PLAY_AGAIN_Y 75
#define HIGHSCORE_X 320
#define HIGHSCORE_Y 10
#define TITLE_X 20
#define TITLE_Y 50
#define START_X 15
#define START_Y 20
#define PRESS_TAB_X 15
#define PRESS_TAB_Y 30
#define TITLE_IMG_X 0
#define TITLE_IMG_Y 50
#define TITLE_IMG_W 350
#define TITLE_IMG_H 350
#define POINTER_X 15
#define POINTER_Y 60
#define POINTER_W 5
#define POINTER_H 5
#define SAVED_GAMES_X 30
#define SAVED_GAMES_Y 50
#define SL_INSTRUCTIONS_X 30
#define SL_INSTRUCTIONS_Y 10
#define LINE_X 10
#define LINE_Y 15
#define WRITE_INSTR_X 15
#define WRITE_INSTR_Y 10
#define INPUT_X 15
#define INPUT_Y 45
#define TITLE_INSTR_X 30
#define TITLE_INSTR_Y 10
#define INSTR_LIST_X 30
#define INSTR_LIST_Y 50

#define POINTER_MOVE 50

#define TEXT_COLOR_R 255
#define TEXT_COLOR_G 255
#define TEXT_COLOR_B 255
#define TEXT_COLOR_A 255
#define BCKGRND_CLR_R 49
#define BCKGRND_CLR_G 52
#define BCKGRND_CLR_B 58
#define BCKGRND_CLR_A 255

// change all for mac!
#define DICTIONARY_FILE "words.bin"
#define IMAGE_FILE "hangman_bmp/%d.bmp"
#define LOAD_GAME_FILE "savedGames/%s"
#define SAVED_FILES_PATH "savedGames"
#define LOG_FILE_PATH "log.txt"
#define LEADERBOARD_FILE_PATH "highscore.bin"

#define ERROR_WORD "FAILED"
#define INITIALIZATION_ERROR_MSG "INITIALIZATION ERRROR"
#define SDL_INITIALIZATION_ERROR_MSG "Failed to initialize SDL: %s\n"

#define GAME_OVER_MSG "GAME OVER"
#define THE_ANSWER_MSG "THE ANSWER:"
#define CONGRATS_MSG "CONGRATS!"
#define PLAY_AGAIN_MSG "PRESS 'SPACE' TO PLAY AGAIN"
#define GUESSED_WORDS_IN_A_ROW_MSG "GUESSED WORDS IN A ROW: "
#define TITLE_MSG "HANGMAN"
#define START_MSG "PRESS 'SPACE' TO START"
#define INSTRUCTIONS_MSG "PRESS 'TAB' FOR INSTRUCTIONS"
#define LINE "______________________________________"
#define WRITE_FILE_NAME_MSG "WRITE THE NAME OF THE FILE:"
#define TITLE_INSTRUCTIONS_MSG "INSTRUCTIONS:"
#define S_INSTRUCTIONS_MSG "S - SAVE GAME"
#define L_INSTRUCTIONS_MSG "L - LOAD GAME"
#define H_INSTRUCTIONS_MSG "H - VIEW LEADERBOARD"
#define ESC_INSTRUCTIONS_MSG "ESC - EXIT TO MAIN MENU"
#define NO_GAMES_TO_LOAD_MSG "THERE ARE NO GAMES TO LOAD"

// struct for the most used SDL variables in this game
typedef struct MainSDLVariables{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surfaceHangman[MAX_WRONG_GUESS_COUNT];
    SDL_Texture *textureHangman[MAX_WRONG_GUESS_COUNT];
    SDL_Event event;
    SDL_Surface *surfaceDashes;
    SDL_Texture *textureDashes;
    TTF_Font *font;
} MainSDLVariables;

// struct for all hangman variables
typedef struct Hangman{
    char* word;
    int wordLength;
    int guessCountWrong;
    int guessCountRight;
    int guessCountTotal;
    int* guessedLetterIndexes;
    int guessedLetterIndexCount;
    char* guessedLetters;
    int correctGuessInARow;
    bool correctGuessAdded;
    bool started;
    bool save;
} Hangman;

// INITIALIZE FUNCTIONS BEFORE THE GAME
// initialize all hangman variables to NULL, 0, false ect
Hangman initializeHangman();

// initialize all SDL variables
MainSDLVariables initializeMainSDL();

// generating random word from a dictionary file
void generateRandomWord(Hangman *hangman);

// uploading the bmp files with hangman parts
void uploadHangmanImages(MainSDLVariables *mainSDL);


// FUNCTIONS RELATED TO USER INPUT
// getting input when playing
void getUserInput(Hangman *hangman, MainSDLVariables *mainSDL);

// check if guess is (in)correct and for repetition
void checkLetter(char letter, Hangman *hangman);

// uploading the bmp files with hangman parts;
char* getFileName(MainSDLVariables *mainSDL);


// RENDERING FUNCTIONS
// rendering correct letters guessed
void renderLetters(Hangman *hangman, MainSDLVariables *mainSDL);

// rendering dashes under the letters
void renderDashes(Hangman *hangman, MainSDLVariables *mainSDL);

// rendering hangman parts
void renderHangman(Hangman *hangman, MainSDLVariables *mainSDL);

// rendering game over, congrats, title, play again, instruction messages
void renderGameOver(Hangman *hangman, MainSDLVariables *mainSDL);
void renderCongrats(Hangman *hangman, MainSDLVariables *mainSDL);
void renderTitle(Hangman *hangman, MainSDLVariables *mainSDL);
void renderPlayAgain(int xPosition, int yPosition, MainSDLVariables *mainSDL);
void renderInstructions(MainSDLVariables *mainSDL);

// 'high score': correct guesses in a row (shown at the top right corner)
void renderCorrectGuessCount(Hangman *hangman, MainSDLVariables *mainSDL);

// rendering list of files to load from or to save to
char* renderSaveORLoad(Hangman *hangman, bool save, MainSDLVariables *mainSDL);

// render a list of all the letters guessed
void renderLettersGuessed(Hangman *hangman, MainSDLVariables *mainSDL);

// FUNCTION FOR RESETTING VARIABLES
// set some variables to default after lost game
void playAgain(Hangman *hangman);

#endif //GAME_H_INCLUDED