#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
void print_hand(deck_t * hand){
    card_t** cards = hand -> cards;
    for(int i = 0 ;i < hand -> n_cards; i++){
        print_card(*cards[i]);
    }
    return;
}

int is_same_card(card_t a, card_t c){
    return a.value==c.value && a.suit==c.suit;
}

int deck_contains(deck_t * d, card_t c) {
    card_t** cards = d -> cards;
    for(int i = 0 ;i < d -> n_cards; i++){
        if(is_same_card(*cards[i],c))return 1;
    }
  return 0;
}

void swap(card_t* a, card_t* b){
   card_t temp;
   temp = *a;
   *a = *b;
   *b = temp;
}

void shuffle(deck_t * d){
    card_t** cards = d -> cards;
    for(int i = 0; i < d -> n_cards ; i++ ){
        swap(cards[i], cards[rand()%d -> n_cards]);
    }
    return;
}

void assert_full_deck(deck_t * d) {
    int no_repeated = 1;
    card_t** cards = d -> cards;
    for(int i = 0 ;i < d -> n_cards; i++){
        for (int j = 0; j < d -> n_cards; j++){
            if(i == j) continue;
            if(is_same_card(*cards[i],*cards[j]))no_repeated = 0;
        }
    }
    assert(no_repeated);
}
