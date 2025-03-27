// HANGMAN game written using SDL2 and SDL_ttf libraries
// Author: Goda Gutparakyte
// Last modified: 2024-01-25
// Version: 2.0
// Compiled and tested using 13.0.0 version gcc compiler and C99 standard

// SUMMARY: definitions of functions declared in leaderboard.h

#include "leaderboard.h"

Player** initializeLeaderboard(){
    Player** leaderboard = malloc(sizeof(Player*) * MAX_HIGHSCORES_SHOWN);
    if(!leaderboard){
        return NULL;
    }

    for(int i = 0; i < MAX_HIGHSCORES_SHOWN; ++i){
        leaderboard[i] = malloc(sizeof(Player));
        if(!leaderboard[i]){
            return NULL;
        }

        leaderboard[i]->highscore = 0;
        leaderboard[i]->name = malloc((strlen(EMPTY_NAME) + 1) * sizeof(char)); 
        if(!leaderboard[i]->name){
            return NULL;
        }

        strcpy(leaderboard[i]->name, EMPTY_NAME); 
        leaderboard[i]->nameLength = strlen(leaderboard[i]->name);
    }
    return leaderboard;
}

void saveLeaderboard(Player **leaderboard){
    FILE *ftp;
    ftp = fopen(LEADERBOARD_FILE_PATH, "wb"); 
    if(!ftp){
        return;
    }

    for(int i = 0; i < MAX_HIGHSCORES_SHOWN; ++i){ 
        fwrite(&(leaderboard[i]->nameLength), sizeof(int), 1, ftp);
        fwrite(leaderboard[i]->name, sizeof(char), leaderboard[i]->nameLength, ftp);
        fwrite(&(leaderboard[i]->highscore), sizeof(int), 1, ftp);
    }
    
    fclose(ftp);
}

void sortLeaderboard(Player **leaderboard){
    if(!leaderboard){
        return;
    }

    for(int i = 0; i < MAX_HIGHSCORES_SHOWN - 1; ++i){
        for(int k = i + 1; k < MAX_HIGHSCORES_SHOWN; ++k){
            if(leaderboard[i]->highscore < leaderboard[k]->highscore){
                Player *temp = leaderboard[i];
                leaderboard[i] = leaderboard[k];
                leaderboard[k] = temp;
            }
        }
    }
}

int loadLeaderboard(Player **leaderboard){
    FILE *ftp;
    ftp = fopen(LEADERBOARD_FILE_PATH, "rb");
    if(!ftp){
        return ERROR_NUMBER;
    }

    for(int i = 0; i < MAX_HIGHSCORES_SHOWN; ++i){
        fread(&(leaderboard[i]->nameLength), sizeof(int), 1, ftp);

        leaderboard[i]->name = malloc((leaderboard[i]->nameLength + 1) * sizeof(char));
        if(!leaderboard[i]->name){
            fclose(ftp);
            return ERROR_NUMBER;
        }

        fread(leaderboard[i]->name, sizeof(char), leaderboard[i]->nameLength, ftp);
        leaderboard[i]->name[leaderboard[i]->nameLength] = '\0';
        fread(&(leaderboard[i]->highscore), sizeof(int), 1, ftp);
    }
    fclose(ftp);
    return leaderboard[MAX_HIGHSCORES_SHOWN - 1]->highscore;
}

void updateLeaderboard(char* name, int highscore, Player **leaderboard){
    free(leaderboard[MAX_HIGHSCORES_SHOWN - 1]->name);

    leaderboard[MAX_HIGHSCORES_SHOWN - 1]->name = malloc((strlen(name) + 1) * sizeof(char));
    if(!leaderboard[MAX_HIGHSCORES_SHOWN - 1]->name){
        return;
    }

    strcpy(leaderboard[MAX_HIGHSCORES_SHOWN - 1]->name, name);
    leaderboard[MAX_HIGHSCORES_SHOWN - 1]->nameLength = strlen(name);
    leaderboard[MAX_HIGHSCORES_SHOWN - 1]->highscore = highscore;
}

char* getName(MainSDLVariables *mainSDL){
    SDL_RenderClear(mainSDL->renderer);
    SDL_RenderPresent(mainSDL->renderer);

    char* inputName = calloc(MAX_NAME_LENGTH + 1, sizeof(char));
    if(!inputName){
        return NULL;
    }
    int counter = 0;
    SDL_Event eventGetInput;

    while(1){
        while(SDL_PollEvent(&eventGetInput)){
            if(eventGetInput.type == SDL_KEYDOWN){
                if(eventGetInput.key.keysym.sym == SDLK_RETURN){
                    inputName[counter] = '\0';
                    return inputName;
                }
                else if(((eventGetInput.key.keysym.sym > 96 && eventGetInput.key.keysym.sym < 123) || (eventGetInput.key.keysym.sym > 47 && eventGetInput.key.keysym.sym < 58)) && counter < MAX_SAVED_GAME_FILE_LENGTH){
                    inputName[counter] = (char)eventGetInput.key.keysym.sym;
                    ++counter;
                }
                if(eventGetInput.key.keysym.sym == SDLK_ESCAPE){
                    free(inputName);
                    return NULL;
                }
                if(eventGetInput.key.keysym.sym == SDLK_BACKSPACE){
                    if (counter > 0){
                        inputName[counter - 1] = '\0';
                        --counter;
                    }
                }
            }
        }

        SDL_Surface *surfaceText = TTF_RenderText_Solid(mainSDL->font, CONGRATS_ON_HIGHSCORE_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureText = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceText);

        SDL_RenderCopy(mainSDL->renderer, textureText, NULL, &(SDL_Rect){
            .x = WRITE_INSTR_X,
            .y = WRITE_INSTR_Y,
            .w = (surfaceText->w) * 0.6,
            .h = (surfaceText->h) * 0.6
        });

        SDL_DestroyTexture(textureText);
        SDL_FreeSurface(surfaceText);

        SDL_Surface *surfaceName = TTF_RenderText_Solid(mainSDL->font, WRITE_PLAYER_NAME_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureName = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceName);

        SDL_RenderCopy(mainSDL->renderer, textureName, NULL, &(SDL_Rect){
            .x = WRITE_INSTR_X,
            .y = WRITE_INSTR_Y + PLAYER_NAME_Y,
            .w = (surfaceName->w) * 0.6,
            .h = (surfaceName->h) * 0.6
        });

        SDL_DestroyTexture(textureName);
        SDL_FreeSurface(surfaceName);

        SDL_Surface *surfaceLine = TTF_RenderText_Solid(mainSDL->font, LINE, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureLine = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceLine);

        SDL_RenderCopy(mainSDL->renderer, textureLine, NULL, &(SDL_Rect){
            .x = LINE_X,
            .y = LINE_Y + PLAYER_NAME_Y,
            .w = (surfaceLine->w) * 0.8,
            .h = (surfaceLine->h) * 0.8
        });

        SDL_DestroyTexture(textureLine);
        SDL_FreeSurface(surfaceLine);

        if(counter > 0){
            SDL_Surface *surfaceInput = TTF_RenderText_Solid(mainSDL->font, inputName, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
            SDL_Texture *textureInput = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceInput);

            SDL_RenderCopy(mainSDL->renderer, textureInput, NULL, &(SDL_Rect){
                .x = INPUT_X,
                .y = INPUT_Y + INPUT_NAME_Y,
                .w = (surfaceInput->w) * 0.8,
                .h = (surfaceInput->h) * 0.8
            });

            SDL_DestroyTexture(textureInput);
            SDL_FreeSurface(surfaceInput);

        }
        SDL_RenderPresent(mainSDL->renderer);
        SDL_RenderClear(mainSDL->renderer);
    }
    free(inputName);
}

void renderLeaderboard(MainSDLVariables *mainSDL, Player **leaderboard){
    SDL_Surface *surfaceTitle = TTF_RenderText_Solid(mainSDL->font, TITLE_LEADEARBOARD_MSG, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
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

    for(int i = 0; i < MAX_HIGHSCORES_SHOWN; ++i){
        SDL_Surface *surfaceInstructions = TTF_RenderText_Solid(mainSDL->font, leaderboard[i]->name, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureInstructions = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceInstructions);

        SDL_RenderCopy(mainSDL->renderer, textureInstructions, NULL, &(SDL_Rect){
            .x = INSTR_LIST_X,
            .y = INSTR_LIST_Y + (i * INSTR_LIST_X),
            .w = (surfaceInstructions->w) * 0.8,
            .h = (surfaceInstructions->h) * 0.8,
        });

        SDL_DestroyTexture(textureInstructions);
        SDL_FreeSurface(surfaceInstructions);

        char* temp = malloc(5);
        sprintf(temp, "%d", leaderboard[i]->highscore);
        SDL_Surface *surfaceScore = TTF_RenderText_Solid(mainSDL->font, temp, (SDL_Color){TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_G, TEXT_COLOR_A});
        SDL_Texture *textureScore = SDL_CreateTextureFromSurface(mainSDL->renderer, surfaceScore);

        SDL_RenderCopy(mainSDL->renderer, textureScore, NULL, &(SDL_Rect){
            .x = INSTR_LIST_X + SCORE_X,
            .y = INSTR_LIST_Y + (i * INSTR_LIST_X),
            .w = (surfaceScore->w) * 0.8,
            .h = (surfaceScore->h) * 0.8,
        });

        free(temp);

        SDL_DestroyTexture(textureScore);
        SDL_FreeSurface(surfaceScore);
    }
}

void freeLeaderboard(Player **leaderboard){
    for (int i = 0; i < MAX_HIGHSCORES_SHOWN; ++i){
        free(leaderboard[i]->name); 
    }
}
