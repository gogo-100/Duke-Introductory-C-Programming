#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

void rotate(char matrix[10][10]){
    char res[10][10];
    for(int i = 0; i < 10; i++){
        for(int j = 0; j< 10; j++){
            res[j][10-1-i] = matrix[i][j];
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j< 10; j++){
            matrix[i][j] = res[i][j];
        }
    }
    return ;
}

void print_mat(char matrix[10][10]){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("%c",matrix[i][j]);
        }
        printf("\n");
    }
}

int get_mat(FILE* f, char mat[10][10]){
    int c;
    int i = 0,j = 0;
    while(( c = fgetc(f)) != EOF){
        if(j == 10 && c == '\n'){ //newline
            j = 0;
            i++;
        }
        else if(j == 10 && c != '\n'){ //too long
            fprintf(stderr, "input row too long\n");
            return 0;
        }
        else if(i == 10){ //row count over 10
            fprintf(stderr, "input too much row\n");
            return 0;
        }
        else{
            mat[i][j++] = c;
        }
    }
    if(i != 10 || j != 0 ){
        fprintf(stderr, "input invalid\n");
        return 0;
    }
    else return 1;
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "please input filename\n");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");

    if(f == NULL){
       perror("Could not open file\n");
       return EXIT_FAILURE;
    }

    char mat[10][10];

    if(!get_mat(f,mat)){
       perror("Could not open file\n");
       return EXIT_FAILURE;
    }

    rotate(mat);

    print_mat(mat);

    if (fclose(f) != 0) {
        perror("Failed to close the input file!\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

