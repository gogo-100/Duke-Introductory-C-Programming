#include<stdio.h>
#include<stdlib.h>
#include "cards.h"
#include "deck.h"
#include <assert.h>


int main(){
    card_t c1 = card_from_num(12);
    card_t c2 = card_from_num(4);
    card_t c3 = card_from_num(32);
    card_t c4 = card_from_num(42);
    printf("print card\n");
    print_card(c1);
    print_card(c2);
    print_card(c3);
    print_card(c4);
    printf("\n\n");
    deck_t excludes[2];
    deck_t* temp[2];
    temp[0] = &excludes[0];
    temp[1] = &excludes[1];
    temp[0]->n_cards = 0;
    temp[1]->n_cards = 0;
    temp[0]->cards = NULL;
    temp[1]->cards = NULL;
    add_card_to(temp[0],c1);
    add_card_to(temp[0],c2);
    add_card_to(temp[1],c3);
    add_card_to(temp[1],c4);
    printf("print hand\n");
    print_hand(temp[0]);
    printf("\n");
    print_hand(temp[1]);
    printf("\n\n");
    add_empty_card(temp[0]);
    add_empty_card(temp[0]);
    add_empty_card(temp[1]);
    add_empty_card(temp[1]);
    printf("print hand\n");
    print_hand(temp[0]);
    printf("\n");
    print_hand(temp[1]);
    printf("\n\n");
    deck_t *d = build_remaining_deck(temp,2);
    print_hand(d);
    free(d);
    return 0;
}

