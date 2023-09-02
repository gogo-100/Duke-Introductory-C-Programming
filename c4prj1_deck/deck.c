#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"



void print_hand(deck_t * hand){
    card_t** cards = hand -> cards;
    int invalid_count = 0;
    for(int i = 0 ;i < hand -> n_cards; i++){
        if (check_card_valid(*cards[i])) {
            print_card(*cards[i]);
        }
        else {
            printf("?%d", invalid_count++);
        }
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

void add_card_to(deck_t * deck, card_t c){
    deck->cards = realloc(deck->cards,(++(deck->n_cards))*sizeof(card_t*));
    //deck->cards[deck->n_cards-1] = &c;   
    //传过来是值，等于是复制了一个card在这个函数里，离开这个函数，c的内存自然free了
    card_t* card = malloc(sizeof(card_t));
    card->suit = c.suit;
    card->value = c.value;
    deck->cards[deck->n_cards - 1] = card;
    return;
}

/*Add a card whose value and suit are both 0, and return a pointer to it in the deck.
   This will add an invalid card to use as a placeholder for an unknown card.
*/

card_t * add_empty_card(deck_t * deck){
    card_t placeholder; //是0.0吗
    placeholder.suit = 0;
    placeholder.value = 0;
    add_card_to(deck, placeholder);
    return deck->cards[deck->n_cards - 1];
}


deck_t* create_deck() {
    deck_t* deck = malloc(sizeof(deck_t));
    deck->cards = NULL;
    deck->n_cards = 0;
    return deck;
}


/*
   Create a deck that is full EXCEPT for all the cards
   that appear in excluded_cards.  For example,
   if excluded_cards has Kh and Qs, you would create
   a deck with 50 cards---all of them except Kh and Qs.
   You will need to use malloc to allocate this deck.
*/
deck_t * make_deck_exclude(deck_t * excluded_cards){
    deck_t* deck = create_deck();
    for(int i = 0; i < 52; i++){
        card_t card = card_from_num(i);
        if(deck_contains(excluded_cards,card) == 0){
            add_card_to(deck,card);                   
        }
    }
    return deck;
}


/*
This function takes an array of hands (remember
   that we use deck_t to represent a hand).  It then builds
   the deck of cards that remain after those cards have
   been removed from a full deck.  For example, if we have
   two hands:
      Kh Qs ?0 ?1 ?2 ?3 ?4
      As Ac ?0 ?1 ?2 ?3 ?4
   then this function should build a deck with 48
   cards (all but As Ac Kh Qs).  You can just build
   one deck with all the cards from all the hands
   (remember you just wrote add_card_to),
   and then pass it to make_deck_exclude.
*/
deck_t* build_remaining_deck(deck_t** hands, size_t n_hands) {
    deck_t* excluded = create_deck();
    for (int i = 0; i < n_hands; i++) {
        for (int j = 0; j < hands[i]->n_cards; j++) {
            add_card_to(excluded, *(hands[i]->cards[j]));
        }
    }
    deck_t* res = make_deck_exclude(excluded);  //注意要不要处理那些empty牌
    free_deck(excluded);
    return res;
}
/*
   Free the memory allocated to a deck of cards.
   For example, if you do
     deck_t * d = make_excluded_deck(something);
     free_deck(d);
   it should free all the memory allocated by make_excluded_deck.
   Once you have written it, add calls to free_deck anywhere you
   need to to avoid memory leaks.
*/
void free_deck(deck_t * deck){
    for(int i = 0; i < deck->n_cards; i++){
        free(deck->cards[i]);
    }
    free(deck->cards);
    free(deck);
}



