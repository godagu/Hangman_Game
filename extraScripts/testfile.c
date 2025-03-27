#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_HIGSCORES_SHOWN 8
#define MAX_NAME_LENGTH 15

typedef struct Player{
    int nameLength;
    char* name;
    int highscore;
} Player;

Player** initializeLeaderBoard();
void savePlayer(char* name, int highscore, Player **player, int index);
void saveLeaderBoard(Player **player);
void sortLeaderBoard(Player **player);
void loadLeaderBoard(Player **player);
void updateLeaderBoard(Player **player);
char* getName();

int main(){
    Player** leaderBoard = initializeLeaderBoard();
    savePlayer("Goda", 10, leaderBoard, 0);
    savePlayer("Adrija", 8, leaderBoard, 1);
    savePlayer("Tomas", 50, leaderBoard, 2);
    savePlayer("Godas", 10, leaderBoard, 3);
    savePlayer("Tomas", 50, leaderBoard, 4);

    sortLeaderBoard(leaderBoard);

    saveLeaderBoard(leaderBoard);
    loadLeaderBoard(leaderBoard);

    for(int i = 0; i < MAX_HIGSCORES_SHOWN; ++i){
        printf("%s \n", leaderBoard[i]->name);
        printf("%d \n", leaderBoard[i]->highscore);
    }
}

Player** initializeLeaderBoard(){
    Player** players = malloc(sizeof(Player*) * MAX_HIGSCORES_SHOWN);
    if(!players){
        return NULL;
    }

    for(int i = 0; i < MAX_HIGSCORES_SHOWN; ++i){
        players[i] = malloc(sizeof(Player));
        if(!players[i]){
            return NULL;
        }

        players[i]->highscore = 0;
        players[i]->name = malloc(5 * sizeof(char));
        if(!players[i]->name){
            return NULL;
        }
        strcpy(players[i]->name, "----");
        players[i]->nameLength = strlen(players[i]->name);
    }
    return players;

    //kazkur freeint reikia!!!!!
}

void savePlayer(char* name, int highscore, Player **player, int index){
    if(index < MAX_HIGSCORES_SHOWN){
        player[index]->name = malloc((strlen(name) + 1) * sizeof(char));
        if(!player[index]->name){
            return;
        }

        strcpy(player[index]->name, name);
        player[index]->nameLength = strlen(name);
        player[index]->highscore = highscore;
    }
}

void saveLeaderBoard(Player **player){
    FILE *ftp;
    ftp = fopen("/Users/mac/Downloads/hangman/highscore.bin", "wb");
    if(!ftp){
        return;
    }

    for(int i = 0; i < MAX_HIGSCORES_SHOWN; ++i){ //jei padedu i < MAX_HIGHSCORE_SHOWN - crashina
        fwrite(&(player[i]->nameLength), sizeof(int), 1, ftp);
        fwrite(player[i]->name, sizeof(char), player[i]->nameLength, ftp);
        fwrite(&(player[i]->highscore), sizeof(int), 1, ftp);
    }

    fclose(ftp);
}

void loadLeaderBoard(Player **player){
    FILE *ftp;
    ftp = fopen("/Users/mac/Downloads/hangman/highscore.bin", "rb");
    if(!ftp){
        return;
    }

     for(int i = 0; i < MAX_HIGSCORES_SHOWN; ++i){
        fread(&(player[i]->nameLength), sizeof(int), 1, ftp);

        player[i]->name = malloc((player[i]->nameLength + 1) * sizeof(char));
        if(!player[i]->name){
            fclose(ftp);
            return;
        }
        fread(player[i]->name, sizeof(char), player[i]->nameLength, ftp);
        player[i]->name[player[i]->nameLength] = '\0';
        fread(&(player[i]->highscore), sizeof(int), 1, ftp);
    }

    fclose(ftp);
}

void freeLeaderBoard(Player **players) {
    for (int i = 0; i < MAX_HIGSCORES_SHOWN; ++i) {
        free(players[i]->name); //improve wtf is this????
    }
    free(players);
}

void sortLeaderBoard(Player **player){
    if(!player){
        return;
    }

    for(int i = 0; i < MAX_HIGSCORES_SHOWN - 1; ++i){
        for(int k = i + 1; k < MAX_HIGSCORES_SHOWN; ++k){
            if(player[i]->highscore < player[k]->highscore){
                Player *temp = player[i];
                player[i] = player[k];
                player[k] = temp;
            }
        }
    }
}

void updateLeaderBoard(Player **player){
    
}