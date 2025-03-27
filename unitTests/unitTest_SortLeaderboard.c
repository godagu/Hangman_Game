#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_HIGHSCORES_SHOWN 8
#define EMPTY_NAME "----"

// struct for a player (for adding to leaderboard)
typedef struct Player{
    int nameLength;
    char* name;
    int highscore;
}Player;

bool leaderboardSorted(Player **leaderboard);
void generateRandomLeaderboard(Player **leaderboard);
//function to test
void sortLeaderboard(Player **leaderboard);


int main(){

Player** leaderboard = malloc(sizeof(Player*) * MAX_HIGHSCORES_SHOWN);
if(!leaderboard){
    return 0;
}

generateRandomLeaderboard(leaderboard);
sortLeaderboard(leaderboard);
assert(leaderboardSorted(leaderboard));

for(int i = 0; i < MAX_HIGHSCORES_SHOWN){
    free(leaderboard[i]->name);
}
free(leaderboard);

return 0;

}

bool leaderboardSorted(Player **leaderboard){
    for(int i = 0, j = i + 1; i < MAX_HIGHSCORES_SHOWN - 1 && j < MAX_HIGHSCORES_SHOWN; ++i, ++j){
        if(leaderboard[i]->highscore < leaderboard[j]->highscore){
            return false;
        }
    }

    return true;
}

void generateRandomLeaderboard(Player **leaderboard){
    for(int i = 0; i < MAX_HIGHSCORES_SHOWN; ++i){
        leaderboard[i] = malloc(sizeof(Player));
        if(!leaderboard[i]){
            return;
        }

        leaderboard[i]->highscore = rand() % 100;
        leaderboard[i]->name = malloc((strlen(EMPTY_NAME) + 1) * sizeof(char)); 
        if(!leaderboard[i]->name){
            return;
        }

        strcpy(leaderboard[i]->name, EMPTY_NAME); 
        leaderboard[i]->nameLength = strlen(leaderboard[i]->name);
    }
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