#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

char** readFiles(const char *path, short *counter);
bool correctReadFiles(char** files);

int main(){
    char* directoryFiles[] = {"unitTest_readFiles.c", "unitTest_SortLeaderboard.c", "unitTest_generateRandomWord.c"};
    assert(correctReadFiles(directoryFiles));
    return 0;
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

bool correctReadFiles(char** files){
    short counter = 0;
    char** testFiles = readFiles("unitTests", &counter);

    bool found;
    for(int i = 0; i < 3; ++i){
        found = false;
        for(int k = 0; k < 3; ++k){
            if(strcmp(testFiles[i], files[k]) == 0){
                found = true;
                break;
            }
        }
        if(!found){
            return false;
        } 
    }
    return true;
}