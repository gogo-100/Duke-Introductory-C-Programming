#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "deck.h"
#include "future.h"

deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc);

int main(){
    future_cards_t fc;
    fc.decks = NULL;
    fc.n_decks = 0;
    size_t count = 0;
    FILE * f = fopen("tests.txt", "r");
    if (f == NULL) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }
    deck_t** temp = read_input(f,&count,&fc);
    printf("add read in\n");
    for(int i = 0; i < count ;i++){
        print_hand(temp[i]);
        printf("\n");
    }
    printf("add: future\n");
    future_cards_from_deck(temp[count-1],&fc);
    for(int i = 0; i < count ;i++){
        print_hand(temp[i]);
        printf("\n");
    }
    future_cards_from_deck(temp[count-2],&fc);
    for(int i = 0; i < count; i++){
    	print_hand(temp[i]);
    	free(temp[i]);
	printf("\n");
    }
    free(temp);
    if (fclose(f) != 0) {
        perror("Failed to close the input file!");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

