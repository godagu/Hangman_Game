// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: definitions of the functions declared in game.h

#include "game.h"
#include "saveLoadGame.h"
#include "leaderboard.h"

Hangman initializeHangman(){
    Hangman hangman;
    hangman.word = NULL;
    hangman.wordLength = 0;
    hangman.guessCountWrong = 0;
    hangman.guessCountTotal = 0;
    hangman.guessedLetterIndexes = NULL;
    hangman. guessedLetters = NULL;
    hangman.guessedLetterIndexCount = 0;
    hangman.guessCountRight = 0;
    hangman.correctGuessInARow = 0;
    hangman.correctGuessAdded = false;
    hangman.started = false;
    hangman.save = true;
    return hangman;
}

MainSDLVariables initializeMainSDL(){
    MainSDLVariables mainSDLVariables;
    mainSDLVariables.window = NULL;
    mainSDLVariables.renderer = NULL;
    mainSDLVariables.surfaceDashes = NULL;
    mainSDLVariables.textureDashes = NULL;
    mainSDLVariables.font = NULL;
    return mainSDLVariables;
}

void generateRandomWord(Hangman *hangman){
    hangman->word = malloc(sizeof(char) * MAX_WORD_LENGTH + 1);
    if(!hangman->word){
        return;
    }

    char ch;
    FILE *ftp;
    int i = 0;
    ftp = fopen(DICTIONARY_FILE, "rb");
    if(!ftp){
        strcpy(hangman->word, ERROR_WORD);
        hangman->wordLength = strlen(ERROR_WORD);
        return;
    }

    fseek(ftp, 0, SEEK_END);
    long fileSize = ftell(ftp);
    fseek(ftp, 0, SEEK_SET);

    long randomIndex = rand() % fileSize;
    fseek(ftp, randomIndex, SEEK_SET);

    while(fgetc(ftp) != '\n'){
        fseek(ftp, --randomIndex, SEEK_SET);
    }

    ch = fgetc(ftp);
    while(ch != '\n'){
        hangman->word[i] = ch;
        ++i;
        ch = fgetc(ftp);
    }

    hangman->word[i] = '\0';

    fclose(ftp);

    hangman->wordLength = strlen(hangman->word);

    hangman->guessedLetterIndexes = calloc(MAX_WORD_LENGTH, sizeof(int) + 1);
    if(!hangman->guessedLetterIndexes){
        return;
    }

    hangman->guessedLetters = calloc(sizeof(char), (MAX_WORD_LENGTH + MAX_WRONG_GUESS_COUNT + 1));
    if(!hangman->guessedLetters){
        return;
    }
}

void uploadHangmanImages(MainSDLVariables *mainSDL){
    for(int i = 1; i < MAX_WRONG_GUESS_COUNT + 1; ++i){ 
        char* fileName = malloc(MAX_IMAGE_FILE_PATH_LENGTH);
        if(!fileName){
            return;
        }
        sprintf(fileName, IMAGE_FILE, i);
        mainSDL->surfaceHangman[i - 1] = SDL_LoadBMP(fileName);
        if(!(mainSDL->surfaceHangman[i - 1])){
            return;
        }
        mainSDL->textureHangman[i - 1] = SDL_CreateTextureFromSurface(mainSDL->renderer, mainSDL->surfaceHangman[i - 1]);
        if(!(mainSDL->textureHangman[i - 1])){
            return;
        }
        free(fileName);
    }
}

void getUserInput(Hangman *hangman, MainSDLVariables *mainSDL){
    if((mainSDL->event.key.keysym.sym > 64 && mainSDL->event.key.keysym.sym < 91) || (mainSDL->event.key.keysym.sym > 96 && mainSDL->event.key.keysym.sym < 123)){
        checkLetter(mainSDL->event.key.keysym.sym, hangman);
    }
}

void checkLetter(char letter, Hangman *hangman){
    bool correctGuess = false;

    for(int k = 0; k < hangman->guessCountTotal; ++k){
        if(letter == hangman->guessedLetters[k]){
            return;
        }
    }

    for(int i = 0; i < hangman->wordLength; ++i){
        if(hangman->word[i] == letter){
            hangman->guessedLetterIndexes[hangman->guessCountRight] = i;
            correctGuess = true;
            ++(hangman->guessCountRight);
        }
    }

    hangman->guessedLetters[hangman->guessedLetterIndexCount] = letter;
    hangman->guessedLetters[hangman->guessedLetterIndexCount + 1] = '\0';
    ++(hangman->guessedLetterIndexCount);
    ++(hangman->guessCountTotal);

    if(!correctGuess){
        ++(hangman->guessCountWrong);
    }

    return;
}

char* getFileName(MainSDLVariables *mainSDL){
    SDL_RenderClear(mainSDL->renderer);
    SDL_RenderPresent(mainSDL->renderer);
    char* inputFileName = calloc(MAX_SAVED_GAME_FILE_LENGTH + 5, sizeof(char)); // .bin\0
    if(!inputFileName){
        return NULL;
    }

    int counter = 0;
    SDL_Event eventGetInput;

    while(1){
        while(SDL_PollEvent(&eventGetInput)){
            if(eventGetInput.type == SDL_KEYDOWN){
                if(eventGetInput.key.keysym.sym == SDLK_RETURN){
                    inputFileName[counter++] = '.';
                    inputFileName[counter++] = 'b';
                    inputFileName[counter++] = 'i';
                    inputFileName[counter++] = 'n';
                    inputFileName[counter] = '\0';
                    return inputFileName;
                }
                else if(((eventGetInput.key.keysym.sym > 96 && eventGetInput.key.keysym.sym < 123) || (eventGetInput.key.keysym.sym > 47 && eventGetInput.key.keysym.sym < 58)) && counter < MAX_SAVED_GAME_FILE_LENGTH){
                    inputFileName[counter] = (char)eventGetInput.key.keysym.sym;
                    ++counter;
                }

                if(eventGetInput.key.keysym.sym == SDLK_ESCAPE){
                    return NULL;
                }

                if(eventGetInput.key.keysym.sym == SDLK_BACKSPACE){
                    if (counter > 0){
                        inputFileName[counter - 1] = '\0';
                        --counter;
                    }
                }

            }
        }

        SDL_Surface *surfaceText = TTF_RenderText_Solid(mainSDL->font, WRITE_FILE_NAME_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureText = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceText);

        SDL_RenderCopy(mainSDL->renderer, textureText, NULL, &(SDL_Rect){
            .x = WRITE_INSTR_X,
            .y = WRITE_INSTR_Y,
            .w = (surfaceText->w) * 0.6,
            .h = (surfaceText->h) * 0.6
        });

        SDL_DestroyTexture(textureText);
        SDL_FreeSurface(surfaceText);

        if(counter > 0){
            SDL_Surface *surfaceInput = TTF_RenderText_Solid(mainSDL->font, inputFileName, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
            SDL_Texture *textureInput = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceInput);

            SDL_RenderCopy(mainSDL->renderer, textureInput, NULL, &(SDL_Rect){
                .x = INPUT_X,
                .y = INPUT_Y,
                .w = (surfaceInput->w) * 0.8,
                .h = (surfaceInput->h) * 0.8
            });

            SDL_DestroyTexture(textureInput);
            SDL_FreeSurface(surfaceInput);
        }
        SDL_RenderPresent(mainSDL->renderer);
        SDL_RenderClear(mainSDL->renderer);
    }
    free(inputFileName);
}

void renderDashes(Hangman *hangman, MainSDLVariables *mainSDL){
    char *dashes = malloc(2 * hangman->wordLength + 1);
    if(!dashes){
        return;
    }

    for(int i = 0; i < 2 * hangman->wordLength; ++i){
        if (i % 2 == 0){
            dashes[i] = '_';
        }
        else if (i % 2 != 0){
            dashes[i] = ' ';
        }
        dashes[2 * hangman->wordLength] = '\0';
    }

    mainSDL->surfaceDashes = TTF_RenderText_Solid(mainSDL->font, dashes, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    mainSDL->textureDashes = SDL_CreateTextureFromSurface(mainSDL->renderer, mainSDL->surfaceDashes);
    SDL_RenderCopy(mainSDL->renderer, mainSDL->textureDashes, NULL, &(SDL_Rect){
        .x = DASHES_X,
        .y = DASHES_Y,
        .w = (mainSDL->surfaceDashes)->w,
        .h = (mainSDL->surfaceDashes)->h
    });

    free(dashes);
}

void renderLetters(Hangman *hangman, MainSDLVariables *mainSDL){
    char *temp = calloc(2, sizeof(char));
    if(!temp){
        return;
    }

    for(int i = 0; i < hangman->guessCountRight; ++i){
        temp[0] =  hangman->word[hangman->guessedLetterIndexes[i]];

        SDL_Surface *surfaceLetter = TTF_RenderText_Solid(mainSDL->font, temp, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_B, TEXT_COLOR_A});
        SDL_Texture *textureLetter = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceLetter);

        SDL_RenderCopy(mainSDL->renderer, textureLetter, NULL, &(SDL_Rect){
            .x = LETTER_X + (hangman->guessedLetterIndexes[i]) * (((mainSDL->surfaceDashes)->w) / (hangman->wordLength)),
            .y = LETTER_Y,
            .w = ((mainSDL->surfaceDashes)->w) / (hangman->wordLength),
            .h = (mainSDL->surfaceDashes)->h,
        });

        SDL_DestroyTexture(textureLetter);
        SDL_FreeSurface(surfaceLetter);
    }
    free(temp);
}



void renderHangman(Hangman *hangman, MainSDLVariables *mainSDL){
    if(hangman->guessCountWrong > 0 && hangman->guessCountWrong < MAX_WRONG_GUESS_COUNT + 1){
        SDL_RenderCopy(mainSDL->renderer, mainSDL->textureHangman[hangman->guessCountWrong - 1], NULL, &(SDL_Rect){
            .x = HANGMAN_X,
            .y = HANGMAN_Y,
            .w = HANGMAN_W,
            .h = HANGMAN_H
        });
    }
}

void renderGameOver(Hangman *hangman, MainSDLVariables *mainSDL){
    SDL_Surface *surfaceGameOver = TTF_RenderText_Solid(mainSDL->font, GAME_OVER_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_B, TEXT_COLOR_A});
    SDL_Texture *textureGameOver = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceGameOver);
    SDL_RenderCopy(mainSDL->renderer, textureGameOver, NULL, &(SDL_Rect){
        .x = WIDTH / 2 - (surfaceGameOver->w),
        .y = HEIGHT / 2 - (surfaceGameOver->h) - GAME_OVER_Y,
        .w = (surfaceGameOver->w) * 2,
        .h = (surfaceGameOver->h) * 2
    });

    SDL_Surface *surfaceAnswer = TTF_RenderText_Solid(mainSDL->font, THE_ANSWER_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_B, TEXT_COLOR_A});
    SDL_Texture *textureAnswer = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceAnswer);
    SDL_RenderCopy(mainSDL->renderer, textureAnswer, NULL, &(SDL_Rect){
        .x = WIDTH / 2 - (surfaceGameOver->w / 2),
        .y = HEIGHT / 2 - (surfaceGameOver->h) + ANSWER_Y,
        .w = (surfaceAnswer->w) / 1.2,
        .h = (surfaceAnswer->h) / 1.2
    });

    SDL_Surface *surfaceAnswer2 = TTF_RenderText_Solid(mainSDL->font, hangman->word, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_B, TEXT_COLOR_A});
    SDL_Texture *textureAnswer2 = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceAnswer2);
    SDL_RenderCopy(mainSDL->renderer, textureAnswer2, NULL, &(SDL_Rect){
        .x = WIDTH / 2 - (surfaceAnswer2->w / 1.2 / 2),
        .y = HEIGHT / 2 - (surfaceGameOver->h) + ANSWER_Y * 2,
        .w = (surfaceAnswer2->w) / 1.2,
        .h = (surfaceAnswer2->h) / 1.2
    });

    int x = WIDTH / 2 - (surfaceGameOver->w) + 60;
    int y = HEIGHT / 2 - (surfaceGameOver->h) + ANSWER_Y * 2;
    
    hangman->correctGuessInARow = 0;

    renderPlayAgain(x, y, mainSDL);

    SDL_DestroyTexture(textureGameOver);
    SDL_DestroyTexture(textureAnswer);
    SDL_DestroyTexture(textureAnswer2);
    SDL_FreeSurface(surfaceGameOver);
    SDL_FreeSurface(surfaceAnswer);
    SDL_FreeSurface(surfaceAnswer2);
}

void renderCongrats(Hangman *hangman, MainSDLVariables *mainSDL){
    SDL_Surface *surfaceCongrats = TTF_RenderText_Solid(mainSDL->font, CONGRATS_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    SDL_Texture *textureCongrats = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceCongrats);
    SDL_RenderCopy(mainSDL->renderer, textureCongrats, NULL, &(SDL_Rect){
        .x = CONGRATS_X,
        .y = HEIGHT / 2 - CONGRATS_Y,
        .w = (surfaceCongrats->w) * 1.5,
        .h = (surfaceCongrats->h) * 1.5
    });

    int x = CONGRATS_X;
    int y = HEIGHT / 2 - CONGRATS_Y;

    if(!hangman->correctGuessAdded){
        ++(hangman->correctGuessInARow);
        hangman->correctGuessAdded = true;
    }

    renderPlayAgain(x, y, mainSDL);

    SDL_DestroyTexture(textureCongrats);
    SDL_FreeSurface(surfaceCongrats);
}

void renderLettersGuessed(Hangman *hangman, MainSDLVariables *mainSDL){
    if(strlen(hangman->guessedLetters) < MAX_WORD_LENGTH){
        SDL_Surface *surfaceLettersGuessed = TTF_RenderText_Solid(mainSDL->font, hangman->guessedLetters, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        if(!surfaceLettersGuessed){
            return;
        }

        SDL_Texture *textureLettersGuessed = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceLettersGuessed);
        if(!textureLettersGuessed){
            return;
        }

        SDL_RenderCopy(mainSDL->renderer, textureLettersGuessed, NULL, &(SDL_Rect){
            .x = GUESSED_LTTRS_X,
            .y = HEIGHT / 2 - GUESSED_LTTRS_Y,
            .w = (surfaceLettersGuessed->w),
            .h = (surfaceLettersGuessed->h)
        });
        SDL_DestroyTexture(textureLettersGuessed);
        SDL_FreeSurface(surfaceLettersGuessed);
    }
    else if(strlen(hangman->guessedLetters) > MAX_GUESSED_LETTERS_IN_A_LINE){
        char* temp = calloc(MAX_WORD_LENGTH, sizeof(char));
        if(!temp){
            return;
        }

        char* temp2 = calloc(hangman->guessedLetterIndexCount - MAX_WORD_LENGTH + 2, sizeof(char));
        if(!temp2){
            return;
        }

        strncpy(temp, hangman->guessedLetters, MAX_GUESSED_LETTERS_IN_A_LINE);
        strncpy(temp2, hangman->guessedLetters + MAX_GUESSED_LETTERS_IN_A_LINE, hangman->guessedLetterIndexCount - MAX_WORD_LENGTH + 1);

        SDL_Surface *surfaceLettersGuessed2 = TTF_RenderText_Solid(mainSDL->font, temp, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureLettersGuessed2 = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceLettersGuessed2);

        SDL_RenderCopy(mainSDL->renderer, textureLettersGuessed2, NULL, &(SDL_Rect){
            .x = GUESSED_LTTRS_X,
            .y = HEIGHT / 2 - GUESSED_LTTRS_Y,
            .w = (surfaceLettersGuessed2->w),
            .h = (surfaceLettersGuessed2->h)
        });

        SDL_Surface *surfaceLettersGuessed3 = TTF_RenderText_Solid(mainSDL->font, temp2, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureLettersGuessed3 = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceLettersGuessed3);

        SDL_RenderCopy(mainSDL->renderer, textureLettersGuessed3, NULL, &(SDL_Rect){
            .x = GUESSED_LTTRS_X,
            .y = HEIGHT / 2 - GUESSED_LTTRS_Y / 2,
            .w = (surfaceLettersGuessed3->w),
            .h = (surfaceLettersGuessed3->h)
        });

        SDL_DestroyTexture(textureLettersGuessed2);
        SDL_DestroyTexture(textureLettersGuessed3);
        SDL_FreeSurface(surfaceLettersGuessed2);
        SDL_FreeSurface(surfaceLettersGuessed3);

        free(temp);
        free(temp2);
    }
}

void renderPlayAgain(int xPosition, int yPosition, MainSDLVariables *mainSDL){
    SDL_Surface *surfacePlayAgain = TTF_RenderText_Solid(mainSDL->font, PLAY_AGAIN_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    SDL_Texture *texturePlayAgain = SDL_CreateTextureFromSurface(mainSDL->renderer, surfacePlayAgain);

    SDL_RenderCopy(mainSDL->renderer, texturePlayAgain, NULL, &(SDL_Rect){
        .x = xPosition,
        .y = yPosition + PLAY_AGAIN_Y,
        .w = (surfacePlayAgain->w) * 0.6,
        .h = (surfacePlayAgain->h) * 0.6
    });

    SDL_DestroyTexture(texturePlayAgain);
    SDL_FreeSurface(surfacePlayAgain);
}

void renderCorrectGuessCount(Hangman *hangman, MainSDLVariables *mainSDL){
    char *temp = calloc(sizeof(char), MAX_HIGHSCORE_CHAR_SIZE); 
    if(!temp){
        return;
    }
    sprintf(temp, "%d", hangman->correctGuessInARow);

    char *temp2 = calloc(strlen(GUESSED_WORDS_IN_A_ROW_MSG) + strlen(temp) + 1, sizeof(char));
    if(!temp2){
        return;
    }
    strcpy(temp2, GUESSED_WORDS_IN_A_ROW_MSG);
    strcat(temp2, temp);

    SDL_Surface *surfaceCount = TTF_RenderText_Solid(mainSDL->font, temp2, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    SDL_Texture *textureCount = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceCount);

    SDL_RenderCopy(mainSDL->renderer, textureCount, NULL, &(SDL_Rect){
        .x = HIGHSCORE_X,
        .y = HIGHSCORE_Y,
        .w = (surfaceCount->w) * 0.5,
        .h = (surfaceCount->h) * 0.5
    });

    free(temp);
    free(temp2);

    SDL_DestroyTexture(textureCount);
    SDL_FreeSurface(surfaceCount);
}

void renderTitle(Hangman *hangman, MainSDLVariables *mainSDL){
    SDL_Surface *surfaceTitle = TTF_RenderText_Solid(mainSDL->font, TITLE_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    SDL_Texture *textureTitle = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceTitle);

    SDL_RenderCopy(mainSDL->renderer, textureTitle, NULL, &(SDL_Rect){
        .x = WIDTH / 3 - TITLE_X,
        .y = HEIGHT / 2 - (surfaceTitle->h) - TITLE_Y,
        .w = (surfaceTitle->w) * 2,
        .h = (surfaceTitle->h) * 2
    });

    SDL_Surface *surfaceStart = TTF_RenderText_Solid(mainSDL->font, START_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    SDL_Texture *textureStart = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceStart);

    SDL_RenderCopy(mainSDL->renderer, textureStart, NULL, &(SDL_Rect){
        .x = WIDTH / 3 + ((surfaceStart->w) / START_X), 
        .y = HEIGHT / 2 - (surfaceStart->h * 0.6 / 2) + START_Y,
        .w = (surfaceStart->w) * 0.6,
        .h = (surfaceStart->h) * 0.6
    });

    SDL_Surface *surfaceTab = TTF_RenderText_Solid(mainSDL->font, INSTRUCTIONS_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    SDL_Texture *textureTab = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceTab);

    SDL_RenderCopy(mainSDL->renderer, textureTab, NULL, &(SDL_Rect){
        .x = PRESS_TAB_X,
        .y = HEIGHT - PRESS_TAB_Y,
        .w = (surfaceTab->w) * 0.5,
        .h = (surfaceTab->h) * 0.5
    });

    SDL_RenderCopy(mainSDL->renderer, mainSDL->textureHangman[MAX_WRONG_GUESS_COUNT - 1], NULL, &(SDL_Rect){
        .x = TITLE_IMG_X,
        .y = TITLE_IMG_Y,
        .w = TITLE_IMG_W,
        .h = TITLE_IMG_H,
    });

    SDL_DestroyTexture(textureTab);
    SDL_FreeSurface(surfaceTab);

    SDL_DestroyTexture(textureStart);
    SDL_FreeSurface(surfaceStart);

    SDL_DestroyTexture(textureTitle);
    SDL_FreeSurface(surfaceTitle);
}


char* renderSaveORLoad(Hangman *hangman, bool save, MainSDLVariables *mainSDL){
    SDL_Event eventSaveGameMenu;
    char** savedFiles = NULL;
    char* instructions = malloc(MAX_INSTRUCTIONS_LENGTH * sizeof(char));
    if(!instructions){
        return NULL;
    }
    short counter = 0;
    short index = 0;
    savedFiles = readFiles(SAVED_FILES_PATH, &counter);

    bool noGamesToLoad = false;

    if(!savedFiles && !save){
        counter = 1;
        savedFiles = calloc(sizeof(char*), 1);
        if(!savedFiles){
            return NULL;
        }
        savedFiles[0] = calloc(sizeof(char), strlen(NO_GAMES_TO_LOAD_MSG) + 1);
        if(!savedFiles[0]){
            return NULL;
        }
        strcpy(savedFiles[0], NO_GAMES_TO_LOAD_MSG);
        noGamesToLoad = true;
    }

    SDL_Rect pointer = {
        .x = POINTER_X,
        .y = POINTER_Y,
        .w = POINTER_W,
        .h = POINTER_H
    };

   if(save && counter < MAX_GAMES_SAVED){
        savedFiles = realloc(savedFiles, (counter + 1) * sizeof(char*));
        if(!savedFiles){
            return NULL;
        }
        savedFiles[counter] = malloc(sizeof(char) * MAX_SAVED_GAME_FILE_LENGTH);
        if(!savedFiles[counter]){
            return NULL;
        }
        strcpy(savedFiles[counter], SAVE_GAME_MSG);
        ++counter;
    }

    if(!save){
        strcpy(instructions, LOAD_INSTRUCTIONS_MSG);
    }
    else{
        strcpy(instructions, SAVE_INSRTUCTIONS_MSG);
    }

    while(1){
        int i = 0;
        for(i = 0; i < counter; ++i){
            SDL_Surface *surfaceSavedGames = TTF_RenderText_Solid(mainSDL->font, savedFiles[i], (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
            SDL_Texture *textureSavedGames = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceSavedGames);

            SDL_RenderCopy(mainSDL->renderer, textureSavedGames, NULL, &(SDL_Rect){
                .x = SAVED_GAMES_X,
                .y = SAVED_GAMES_Y + (SAVED_GAMES_Y * i),
                .w = (surfaceSavedGames->w) * 0.8,
                .h = (surfaceSavedGames->h) * 0.8
            });

            SDL_DestroyTexture(textureSavedGames);
            SDL_FreeSurface(surfaceSavedGames);
        }

        SDL_Surface *surfaceInstruction = TTF_RenderText_Solid(mainSDL->font, instructions, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureInstruction = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceInstruction);

        SDL_RenderCopy(mainSDL->renderer, textureInstruction, NULL, &(SDL_Rect){
            .x = SL_INSTRUCTIONS_X,
            .y = SL_INSTRUCTIONS_Y,
            .w = (surfaceInstruction->w) * 0.7,
            .h = (surfaceInstruction->h) * 0.7
        });

        SDL_DestroyTexture(textureInstruction);
        SDL_FreeSurface(surfaceInstruction);

        SDL_Surface *surfaceLine = TTF_RenderText_Solid(mainSDL->font, LINE, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureLine = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceLine);

        SDL_RenderCopy(mainSDL->renderer, textureLine, NULL, &(SDL_Rect){
            .x = LINE_X,
            .y = LINE_Y,
            .w = (surfaceLine->w) * 0.8,
            .h = (surfaceLine->h) * 0.8
        });

        SDL_DestroyTexture(textureLine);
        SDL_FreeSurface(surfaceLine);

        SDL_SetRenderDrawColor(mainSDL->renderer, TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A);
        SDL_RenderFillRect(mainSDL->renderer, &pointer);
        SDL_SetRenderDrawColor(mainSDL->renderer, BCKGRND_CLR_R, BCKGRND_CLR_G, BCKGRND_CLR_B, BCKGRND_CLR_A);

        SDL_RenderPresent(mainSDL->renderer);
        SDL_RenderClear(mainSDL->renderer);

        while(SDL_PollEvent(&eventSaveGameMenu)){
            if(eventSaveGameMenu.type == SDL_KEYDOWN){
                if(eventSaveGameMenu.key.keysym.sym == SDLK_ESCAPE){
                    return NULL;
                }

                if(eventSaveGameMenu.key.keysym.sym == SDLK_DOWN && index < (counter - 1)){
                    ++index;
                    pointer.y += POINTER_MOVE;
                }

                if(eventSaveGameMenu.key.keysym.sym == SDLK_UP && index > 0){
                    --index;
                    pointer.y -= POINTER_MOVE;
                }

                if(eventSaveGameMenu.key.keysym.sym == SDLK_BACKSPACE && counter > 0){
                    char* pathFile = malloc(MAX_SAVE_FILE_PATH_LENGTH);
                    if(!pathFile){
                        return NULL;
                    }
                    sprintf(pathFile, LOAD_GAME_FILE, savedFiles[index]);
                    remove(pathFile);

                    for(int i = 0; i < counter; ++i){
                        free(savedFiles[i]);
                    }
                    free(savedFiles);

                    savedFiles = readFiles(SAVED_FILES_PATH, &counter);
                    if(save && counter < MAX_GAMES_SAVED){
                        savedFiles = realloc(savedFiles, (counter + 1) * sizeof(char*));
                        if(!savedFiles){
                            return NULL;
                        }

                        savedFiles[counter] = malloc(sizeof(char) * MAX_SAVED_GAME_FILE_LENGTH);
                        if(!savedFiles[counter]){
                            return NULL;
                        }

                        strcpy(savedFiles[counter], SAVE_GAME_MSG);
                        ++counter;
                    }
                }
                if(eventSaveGameMenu.key.keysym.sym == SDLK_RETURN && !noGamesToLoad){
                    char *temp = malloc(strlen(savedFiles[index] + 1));
                    if(!temp){
                        return NULL;
                    }

                    strcpy(temp, savedFiles[index]);

                    for(int i = 0; i < counter; ++i){
                        free(savedFiles[i]);
                    }
                    free(savedFiles);
                    free(instructions);

                    return temp;
                }
            }
        }
    }

    for(int i = 0; i < counter; ++i){
        free(savedFiles[i]);
    }
    
    free(savedFiles);
    free(instructions);
}

void renderInstructions(MainSDLVariables *mainSDL){
    char *instructions[] = {S_INSTRUCTIONS_MSG, L_INSTRUCTIONS_MSG, H_INSTRUCTIONS_MSG, ESC_INSTRUCTIONS_MSG};

    SDL_Surface *surfaceTitle = TTF_RenderText_Solid(mainSDL->font, TITLE_INSTRUCTIONS_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    SDL_Texture *textureTitle = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceTitle);

    SDL_RenderCopy(mainSDL->renderer, textureTitle, NULL, &(SDL_Rect){
        .x = TITLE_INSTR_X,
        .y = TITLE_INSTR_Y,
        .w = (surfaceTitle->w) * 0.7,
        .h = (surfaceTitle->h) * 0.7
    });

    SDL_DestroyTexture(textureTitle);
    SDL_FreeSurface(surfaceTitle);

    SDL_Surface *surfaceLine = TTF_RenderText_Solid(mainSDL->font, LINE, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
    SDL_Texture *textureLine = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceLine);

    SDL_RenderCopy(mainSDL->renderer, textureLine, NULL, &(SDL_Rect){
        .x = LINE_X,
        .y = LINE_Y,
        .w = (surfaceLine->w) * 0.8,
        .h = (surfaceLine->h) * 0.8
    });

    SDL_DestroyTexture(textureLine);
    SDL_FreeSurface(surfaceLine);

    for(int i = 0; i < INSTRUCTION_COUNT; ++i){
        SDL_Surface *surfaceInstructions = TTF_RenderText_Solid(mainSDL->font, instructions[i], (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureInstructions = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceInstructions);

        SDL_RenderCopy(mainSDL->renderer, textureInstructions, NULL, &(SDL_Rect){
            .x = INSTR_LIST_X,
            .y = INSTR_LIST_Y + (i * INSTR_LIST_X),
            .w = (surfaceInstructions->w) * 0.8,
            .h = (surfaceInstructions->h) * 0.8,
        });

        SDL_DestroyTexture(textureInstructions);
        SDL_FreeSurface(surfaceInstructions);
    }
}

void playAgain(Hangman *hangman){
    free(hangman->word);
    free(hangman->guessedLetterIndexes);
    free(hangman->guessedLetters);
    hangman->guessCountWrong = 0;
    hangman->guessedLetterIndexes = NULL;
    hangman->guessCountTotal = 0;
    hangman->guessedLetters = NULL;
    hangman->guessedLetterIndexCount = 0;
    hangman->guessCountRight = 0;
    generateRandomWord(hangman);
}

