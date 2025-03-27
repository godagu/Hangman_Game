// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: main function, initialize and quit functions' definitions

#include "game.h"
#include "saveLoadGame.h"
#include "leaderboard.h"
#include "log.h"

// creating window, renderer, opening font
bool initialize(MainSDLVariables *mainSDL);

// exit function
void quitSDL(MainSDLVariables *mainSDL, Hangman *hangman);

int main(int argc, char *argv[]){
    srand(time(NULL));
    startLog();
    atexit(endLog);

    Hangman hangman = initializeHangman();
    MainSDLVariables mainSDL = initializeMainSDL();
    Player** leaderboard = initializeLeaderboard();
    
    int highscoreMark = loadLeaderboard(leaderboard);

    if(!initialize(&mainSDL)){
        printf(INITIALIZATION_ERROR_MSG);
        return 0;
    }

    bool quit = false;
    bool displayInstructions = false;
    bool displayLeaderboard = false;

    generateRandomWord(&hangman);
    uploadHangmanImages(&mainSDL);

    while(!quit){
        while(SDL_PollEvent(&(mainSDL.event))){
            if(mainSDL.event.type == SDL_QUIT){
                quit = true;
                break;
            }
            else if(mainSDL.event.type == SDL_KEYDOWN){
               if(mainSDL.event.key.keysym.sym == SDLK_SPACE){

                    if(!hangman.started){
                        hangman.started = true;
                    }

                    if(hangman.guessCountRight == hangman.wordLength || hangman.guessCountWrong == MAX_WRONG_GUESS_COUNT){
                        playAgain(&hangman);
                        hangman.correctGuessAdded = false;
                        renderDashes(&hangman, &mainSDL);
                        renderLetters(&hangman, &mainSDL);
                        renderHangman(&hangman, &mainSDL);
                        renderCorrectGuessCount(&hangman, &mainSDL);
                    }

                }

                if(mainSDL.event.key.keysym.sym == SDLK_TAB){
                    displayInstructions = true;
                }

                if(hangman.guessCountRight != hangman.wordLength && hangman.guessCountWrong != MAX_WRONG_GUESS_COUNT && hangman.started){
                    getUserInput(&hangman, &mainSDL);
                }

                if(mainSDL.event.key.keysym.sym == SDLK_l && !hangman.started && !displayLeaderboard){
                    char *temp;
                    temp = renderSaveORLoad(&hangman, !hangman.save, &mainSDL);
                    
                    if(temp != NULL){
                        loadGame(&hangman, temp);
                        hangman.started = true;
                    }

                    displayInstructions = false;
                }

                if(mainSDL.event.key.keysym.sym == SDLK_s && !hangman.started && !displayLeaderboard){
                    char *temp;
                    temp = renderSaveORLoad(&hangman, hangman.save, &mainSDL);

                    if(temp){
                        saveGame(&hangman, temp, &mainSDL);
                        hangman.started = true;
                    }

                    displayInstructions = false;
                    free(temp);
                }

                if(mainSDL.event.key.keysym.sym == SDLK_ESCAPE){
                    hangman.started = false;
                    displayInstructions = false;
                    displayLeaderboard = false;
                }

                if(mainSDL.event.key.keysym.sym == SDLK_h && !hangman.started){
                    displayInstructions = false;
                    displayLeaderboard = true;
                }

            }
        }

        SDL_SetRenderDrawColor(mainSDL.renderer, BCKGRND_CLR_R, BCKGRND_CLR_G, BCKGRND_CLR_B, BCKGRND_CLR_A);

        if(!hangman.started){

            if(displayInstructions){
                renderInstructions(&mainSDL);
            }
            else if(displayLeaderboard){
                renderLeaderboard(&mainSDL, leaderboard);
            }
            else{
                renderTitle(&hangman, &mainSDL);
            }

        }
        else{
            if(hangman.guessCountWrong == MAX_WRONG_GUESS_COUNT){

                if(hangman.correctGuessInARow > highscoreMark){
                    char* name = getName(&mainSDL);

                    if(!name){
                        name = malloc(strlen(DEFAULT_NAME) + 1);
                        if(!name){
                            return 0;
                        }

                        strcpy(name, DEFAULT_NAME);
                    }

                    updateLeaderboard(name, hangman.correctGuessInARow, leaderboard);
                    free(name);
                    sortLeaderboard(leaderboard);
                    saveLeaderboard(leaderboard);
                    freeLeaderboard(leaderboard);
                    highscoreMark = loadLeaderboard(leaderboard);
                }
                renderGameOver(&hangman, &mainSDL);
                
            }
            else if(hangman.guessCountRight == hangman.wordLength){
                renderCongrats(&hangman, &mainSDL);
                renderDashes(&hangman, &mainSDL);
                renderLetters(&hangman, &mainSDL);
                renderHangman(&hangman, &mainSDL);
                renderCorrectGuessCount(&hangman, &mainSDL);
            }
            else{
                renderLettersGuessed(&hangman, &mainSDL);
                renderDashes(&hangman, &mainSDL);
                renderLetters(&hangman, &mainSDL);
                renderHangman(&hangman, &mainSDL);
                renderCorrectGuessCount(&hangman, &mainSDL);
            }
        }
        SDL_RenderPresent(mainSDL.renderer);
        SDL_RenderClear(mainSDL.renderer);
    }
    freeLeaderboard(leaderboard);
    free(leaderboard);
    quitSDL(&mainSDL, &hangman);
    return 0;
}

bool initialize(MainSDLVariables *mainSDL){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, SDL_INITIALIZATION_ERROR_MSG, SDL_GetError());
        return false;
    }

    mainSDL->window = SDL_CreateWindow(TITLE_MSG, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(!mainSDL->window){
        return false;
    }

    mainSDL->renderer = SDL_CreateRenderer(mainSDL->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!mainSDL->renderer){
        return false;
    }

    TTF_Init();
    mainSDL->font = TTF_OpenFont(FONT_NAME, FONT_SIZE);
    if(!mainSDL->font){
        return false;
    }

    return true;
}

void quitSDL(MainSDLVariables *mainSDL, Hangman *hangman){
    for(int i = 0; i < MAX_WRONG_GUESS_COUNT; ++i){
        SDL_DestroyTexture(mainSDL->textureHangman[i]);
        SDL_FreeSurface(mainSDL->surfaceHangman[i]);
    }

    SDL_DestroyTexture(mainSDL->textureDashes);
    SDL_FreeSurface(mainSDL->surfaceDashes);

    SDL_DestroyWindow(mainSDL->window);
    SDL_DestroyRenderer(mainSDL->renderer);

    free(hangman->word);
    free(hangman->guessedLetterIndexes);
    free(hangman->guessedLetters);
    hangman->guessCountWrong = 0;
    hangman->guessedLetterIndexes = NULL;
    hangman->guessCountTotal = 0;
    hangman->guessedLetters = NULL;
    hangman->guessedLetterIndexCount = 0;
    hangman->guessCountRight = 0;

    TTF_CloseFont(mainSDL->font);
}

