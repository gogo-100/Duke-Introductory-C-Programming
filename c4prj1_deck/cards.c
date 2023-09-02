
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cards.h"


void assert_card_valid(card_t c) {
    assert(c.value >= 2 && c.value <= VALUE_ACE);
    assert(c.suit >= SPADES && c.suit <= CLUBS);
}



int check_card_valid(card_t c) {
    return (c.value >= 2 && c.value <= VALUE_ACE) && (c.suit >= SPADES && c.suit <= CLUBS);
}

const char * ranking_to_string(hand_ranking_t r) {
    switch(r){
        case 0:
            return "STRAIGHT_FLUSH";
        case 1:
            return "FOUR_OF_A_KIND";
        case 2:
            return "FULL_HOUSE";
        case 3:
            return "FLUSH";
        case 4:
            return "STRAIGHT";
        case 5:
            return "THREE_OF_A_KIND";
        case 6:
            return "TWO_PAIR";
        case 7:
            return "PAIR";
        case 8:
            return "NOTHING";
        default:
            return "";
    }
}

char value_letter(card_t c) {
    if(c.value >= 2 && c.value <= 9){
        return c.value - 0 + '0';
    }
    else if (c.value == 10){
        return '0';
    }
    else if(c.value == 11){
        return 'J';
    }
    else if(c.value == 12){
        return 'Q';
    }
    else if(c.value == 13){
        return 'K';
    }
    else{
        return 'A';
    }
}


char suit_letter(card_t c) {
    switch(c.suit){
        case 0:
            return 's';
        case 1:
            return 'h';
        case 2:
            return 'd';
        case 3:
            return 'c';
        default:
            return '?';
    }
}

void print_card(card_t c) {
    printf("%c%c",value_letter(c),suit_letter(c));
}

card_t card_from_letters(char value_let, char suit_let) {
    card_t temp;
    if(value_let>='2' && value_let<='9'){
        temp.value = value_let - '0';
    }
    else if(value_let == '0'){
        temp.value = 10;
    }
    else if(value_let == 'J'){
        temp.value = 11;
    }
    else if(value_let == 'Q'){
        temp.value = 12;
    }
    else if(value_let == 'K'){
        temp.value = 13;
    }
    else if(value_let == 'A'){
        temp.value = 14;
    }
    else{
        temp.value = 15;
    }

    if(suit_let == 's'){
        temp.suit = 0;
    }
    else if(suit_let == 'h'){
        temp.suit = 1;
    }
    else if(suit_let == 'd'){
        temp.suit = 2;
    }
    else if(suit_let == 'c'){
        temp.suit = 3;
    }
    else{
        temp.suit = 4;
    }
    //assert_card_valid(temp);
    return temp;
}

card_t card_from_num(unsigned c) {
  card_t temp;
  temp.value = c%13+2;
  temp.suit = c/13;
  return temp;
}

