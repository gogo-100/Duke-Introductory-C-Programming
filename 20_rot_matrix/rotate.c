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

