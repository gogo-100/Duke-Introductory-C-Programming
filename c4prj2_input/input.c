#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "deck.h"
#include "future.h"

/*
   Recall that the input
   file has one hand per line (and that we represent a hand
   with a deck_t).  You should allocate a deck_t for each hand
   and place it into an array of pointers to deck_ts, which is your
   answer.

   As you read the input, if you encounter future cards
   (?0, ?1, ...),
   add_empty_card to
   add_future_card
   add_card_to


 Note that most of the rest of the code assumes that a poker hand
 has AT LEAST 5 cards in it.  Your read_input function should enforce
 this requirement. If there are fewer than 5 cards, print
 a useful error message and exit.
*/

deck_t * hand_from_string(const char * str, future_cards_t * fc){
    deck_t* deck = create_deck();
    int lastValid = strlen(str)-1;
    for(;str[lastValid]=='\0'||str[lastValid]=='\n'||str[lastValid]==' ';lastValid--);
    for(int i = 0; i + 1 <=  lastValid; i = i + 3 ){//考虑换行符
        char suit = str[i+1];
        char value = str[i];
        if(value == '?'){//future cards
            card_t* future_card = add_empty_card(deck);
	    int blank = i+2;
	    for(;blank < strlen(str) -1 && str[blank]!=' ';blank++);
	    int length = blank - i - 1;
	    char ch[15] = "";
            strncpy(ch,str+i+1,length);
	    ch[length]='\0';
	    int index = atoi(ch);
            add_future_card(fc,index, future_card);
	    i = i + (blank - i - 2);
	    // printf("?%d\n",index);
        }
        else{
            card_t card = card_from_letters(value,suit);
            add_card_to(deck, card);
        }
    }
    //printf("%s",str);
    //print_hand(deck);
    //printf("\n");
    if(deck->n_cards < 5){
        perror("cards are fewer than 5 cards");
        exit(1);
    }
    return deck;
}



deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc){
    deck_t ** decks = NULL;
    char * curr = NULL;
    size_t sz;
    size_t i = 0;
    while(getline(&curr,&sz,f)>=0){
        decks = realloc(decks,(i+1)*sizeof(deck_t*));
        decks[i] = hand_from_string(curr,fc);
        i++;
    }
    free(curr);
    *n_hands = i;
    return decks;
}

