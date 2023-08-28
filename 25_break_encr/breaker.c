#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

void get_stats(FILE* f, int* stats){
    int c;
    while(( c = fgetc(f)) != EOF){
        if(!isalpha(c)) continue;
        stats[ tolower(c) - 'a']++;
    }
}

int get_answer(int* stats){
    int max_index = 0;
    for(int i = 0; i < 26 ; i++){
        if( stats[i] > stats[max_index] ) max_index = i;
    }
    return (max_index - 4 + 26)%26;
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "please input filename");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");
    int count[26] = { 0 };

    if(f == NULL){
       perror("Could not open file");
       return EXIT_FAILURE;
    }

    get_stats(f, count);

    printf("%d\n",get_answer(count));

    if (fclose(f) != 0) {
        perror("Failed to close the input file!");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


