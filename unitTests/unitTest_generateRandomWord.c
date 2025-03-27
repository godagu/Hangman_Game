#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 14
#define MAX_WRONG_GUESS_COUNT 9
#define DICTIONARY_FILE "words.bin"
#define ERROR_WORD "FAILED"

//function to be tested
char* generateRandomWord();
bool correctWord(char *word);
int main(){

    assert(correctWord(generateRandomWord()));


    return 0;
}

char* generateRandomWord(){
    char* word = malloc(sizeof(char) * MAX_WORD_LENGTH + 1);
    if(!word){
        return NULL;
    }
    int wordLength;
    char ch;
    FILE *ftp;
    int i = 0;
    ftp = fopen(DICTIONARY_FILE, "rb");
    if(!ftp){
        word = ERROR_WORD;
        wordLength = strlen(ERROR_WORD);
        return NULL;
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
        word[i] = ch;
        ++i;
        ch = fgetc(ftp);
    }

    word[i] = '\0';

    fclose(ftp);

    wordLength = strlen(word);

    return word;
}

bool correctWord(char* word){
    if(!word){
        return false;
    }
    int wordLength = strlen(word);

    for(int i = 0; i < wordLength; ++i){
        if(!isalpha(word[i])){
            return false;
        }
    }

    if(wordLength < 0 || wordLength > MAX_WORD_LENGTH || wordLength == 0){
        return false;
    }

    return true;
}