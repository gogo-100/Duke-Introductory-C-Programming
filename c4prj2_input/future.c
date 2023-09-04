#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "deck.h"
#include "cards.h"
#include "future.h"

void add_future_card(future_cards_t * fc, size_t index, card_t * ptr) {
    size_t size = fc->n_decks;
    if(index >= size){
        fc->decks = realloc(fc->decks,(index + 1) * sizeof(deck_t));
        fc->n_decks = index + 1;
        for(int i = size; i <= index; i++ ){
            fc->decks[i].cards = NULL;
            fc->decks[i].n_cards = 0;
        }
    }
    size_t length = fc->decks[index].n_cards;
    fc->decks[index].cards = realloc(fc->decks[index].cards,(length + 1)* sizeof(card_t*));
    fc->decks[index].n_cards++;
    fc->decks[index].cards[length] = ptr;

}

void future_cards_from_deck(deck_t * deck, future_cards_t * fc){
    for(int i = 0 ;i < fc->n_decks; i++){
        if(deck->n_cards <= i){
            perror("deck's length is too short");
            return ;
        }
        for(int j = 0; j < fc->decks[i].n_cards; j++){
            fc->decks[i].cards[j]->value = deck->cards[i]->value;
            fc->decks[i].cards[j]->suit = deck->cards[i]->suit;
        }
    }
    return;
}

