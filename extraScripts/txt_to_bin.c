#include <stdio.h>
#include <stdlib.h>

int main(){

    FILE *in;
    FILE *out;
    char ch;
    int num;

    in = fopen("words.txt", "r");
    if (in == NULL){
        printf("The file could not be oepned...");
    }

    out = fopen("words.bin", "wb");

    if(out == NULL){
        printf("FIle could not be opened...");
    }

    while(!feof(in)){
        fread(&ch, sizeof(char), 1, in);
        fwrite(&ch, sizeof(char), 1, out);

    }
    
    out = fopen("words.bin", "rb");
     char chars[256];
    fread(chars, sizeof(char), 5, out);
    printf("%s\n", chars);


    fclose(in);
    fclose(out);

    return 0;
}