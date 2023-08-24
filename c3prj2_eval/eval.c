#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int card_ptr_comp(const void * vp1, const void * vp2) {
    card_t *a = vp1;
    card_t *b = vp2;
    if(a -> value > b -> value) return -1;
    else if (a -> value < b -> value) return 1;
    else  return a -> suit > b -> suit;
}

typedef enum {
  SPADES,
  HEARTS,
  DIAMONDS,
  CLUBS,
  NUM_SUITS
} suit_t;

suit_t flush_suit(deck_t * hand) {
  card_t** cards = hand -> cards;
  int count [4] = {0,0,0,0};
  for(int i = 0; i < hand -> n_cards ; i++){
     count[cards[i][0].suit]++;
  }
  for(int i = 0; i < 4; i++){
    if(count[i]>=5) return (enum suit_t) i;
  }
  return NUM_SUITS;
}

unsigned get_largest_element(unsigned * arr, size_t n) {
  unsigned maxVal = arr[0];
  for(int i = 0; i < n ; i++){
     if( arr[i] > maxVal ) maxVal = arr[i];
  }
  return maxVal;
}

size_t get_match_index(unsigned * match_counts, size_t n,unsigned n_of_akind){
  for(size_t i = 0; i < n; i++){
    if(match_counts[i] == n_of_akind) return i;
  }
  abort();
  return -1;
}

// 如果牌是4d 8c 9h 4h 4c
// 那么match_counts 就是 3 1 1 3 3
// 统计的是出现的次数
ssize_t  find_secondary_pair(deck_t * hand,
			     unsigned * match_counts,
			     size_t match_idx) {
  if( match_idx + match_counts[match_idx] > 1 ) return match_idx + match_counts[match_idx];
  else return -1;
}

//检测花色是否合适
int check_suit(suit_t fs, suit_t now){
    if(fs == NUM_SUITS) return 1;
    else return fs == now;
}

//找花色为x值为y的卡牌
//如果找到则返回index，没找到则返回-1
int find_next_different_value(deck_t * hand, size_t index, suit_t fs, signed value){
    card_t** cards = hand -> cards;
    for(;index < hand -> n_cards; index++){
        if( cards[index][0].value == value && check_suit(fs, cards[index][0].suit) ) return index;
    }
    return -1;
}


int is_n_length_straight_at(deck_t * hand, size_t index, suit_t fs, int n){
    if(n == 0) return 1;  //递归出口
    signed nowValue = hand -> cards[index][0].value;
    size_t nextIndex = find_next_different_value(hand, index, fs, nowValue-1);
    if(nextIndex > 0) return is_n_length_straight_at(hand, nextIndex, fs, n-1);
    else return 0;
}

int is_ace_low_straight_at(deck_t * hand, size_t index, suit_t fs){
    card_t** cards = hand -> cards;
    size_t nextIndex = find_next_different_value(hand,0,fs,5);
    if(nextIndex < 0) return 0;
    else{
        if(is_n_length_straight_at(hand, nextIndex, fs, 3))return -1;
    }
    return 0;

}

//index的suit和给的suit会不会不一样
int is_straight_at(deck_t * hand, size_t index, suit_t fs) {
  card_t** cards = hand -> cards;
  if(cards[index][0].value == 1){//要考虑ace-low的情况
     return is_ace_low_straight_at(hand , 0, fs);
  }
  else if (cards[index][0].value < 5) { //4.3.2开头的话没有顺子
     return 0;
  }
  else{
     return is_n_length_straight_at(hand, index, fs, 4);
  }
}


//从[left,right]添加count张单牌
int add_single_cards(deck_t * hand, card_t * card, int left, int right, int count){
  if(left > right) return 0;
  int i = 0;
  for(; left + i <= right && i < count ; i ++){
      card[5 - count + i] = hand -> cards[left + i][0];
  }
  return i;
}

//从[left,right]添加对子
int  add_pair_cards(deck_t * hand, card_t * card, int left, int right, int index){
  if(left > right) return 0;
  int i = 0;
  for(; left + i <= right ; i ++){
      if(left + i + 1 < hand -> n_cards && card[left + i].value == card[left + i + 1].value ){
           card[index] = hand ->  cards[left + i][0];
           card[index + 1] = hand -> cards[left + i + 1][0];
           return 2;
      }
  }
  return 0;
}

hand_eval_t build_hand_from_match(deck_t * hand,
				  unsigned n,
				  hand_ranking_t what,
				  size_t idx) {
  hand_eval_t ans;
  ans.ranking = what;
  for(int i = 0; i < n ;i++ ){
    ans.cards[i] = hand -> cards[idx + 0];
  }
  if(what == hand_ranking_t.TWO_PAIR || what == hand_ranking_t.FULL_HOUSE){//找对子
      int count = what == hand_ranking_t.TWO_PAIR? 2:3;
      //找对子
      int add = add_pair_cards(hand, ans.cards, 0, idx - 1, count);
      if(add == 0) add_pair_cards(hand, ans.cards, idx + n, hand -> n_cards - 1, count);
      //找单牌
      if(n + 2 < 5){
          int count = add_single_cards(hand, ans.cards, 0, idx - 1, 1 );
          if(count == 0){
              int i;
              for(i = idx + 2; i < hand -> n_cards && hand->cards[i][0].value!= ans.cards[3]->value; i++ );
              count = add_single_cards(hand, ans.cards, idx + 2, i - 1, 1 );
              add_single_cards(hand, ans.cards, i + 2, hand -> n_cards - 1, 1 - count );
          }
      }
  }
  else{//找出单张最大的5-n张 deck_t * hand, card_t * card, int left, int right, int count
      //找前面
      int count = add_single_cards(hand, ans.cards, 0, idx - 1, 5 - n );
      //找后面
      if(count + n < 5) add_single_cards(hand, ans.cards, idx + n, hand -> n_cards - 1, 5 - n - count);
  }
  return ans;
}


int compare_hands(deck_t * hand1, deck_t * hand2) {
  qsort(hand1->cards,hand1->n_cards,sizeof(card_t*), card_ptr_comp);
  qsort(hand2->cards,hand2->n_cards,sizeof(card_t*), card_ptr_comp);
  hand_eval_t eval1 = evaluate_hand(hand1);
  hand_eval_t eval2 = evaluate_hand(hand2);
  if(eval1.ranking!=eval2.ranking){
    return eval2.ranking - eval1.ranking;
  }
  else{// 此时比较字符而不是类型，而且类型已经确定所以不会有多的什么对子了
    for(int i = 0 ; i < 5 ; i++){
        if(hand1->cards[i][0].value == hand2->cards[i][0].value) continue;
        else return hand1->cards[i][0].value - hand2->cards[i][0].value;
    }
  }
  return 0;
}



//You will write this function in Course 4.
//For now, we leave a prototype (and provide our
//implementation in eval-c4.o) so that the
//other functions we have provided can make
//use of get_match_counts.
unsigned * get_match_counts(deck_t * hand) ;

// We provide the below functions.  You do NOT need to modify them
// In fact, you should not modify them!


//This function copies a straight starting at index "ind" from deck "from".
//This copies "count" cards (typically 5).
//into the card array "to"
//if "fs" is NUM_SUITS, then suits are ignored.
//if "fs" is any other value, a straight flush (of that suit) is copied.
void copy_straight(card_t ** to, deck_t *from, size_t ind, suit_t fs, size_t count) {
  assert(fs == NUM_SUITS || from->cards[ind]->suit == fs);
  unsigned nextv = from->cards[ind]->value;
  size_t to_ind = 0;
  while (count > 0) {
    assert(ind < from->n_cards);
    assert(nextv >= 2);
    assert(to_ind <5);
    if (from->cards[ind]->value == nextv &&
	(fs == NUM_SUITS || from->cards[ind]->suit == fs)){
      to[to_ind] = from->cards[ind];
      to_ind++;
      count--;
      nextv--;
    }
    ind++;
  }
}


//This looks for a straight (or straight flush if "fs" is not NUM_SUITS)
// in "hand".  It calls the student's is_straight_at for each possible
// index to do the work of detecting the straight.
// If one is found, copy_straight is used to copy the cards into
// "ans".
int find_straight(deck_t * hand, suit_t fs, hand_eval_t * ans) {
  if (hand->n_cards < 5){
    return 0;
  }
  for(size_t i = 0; i <= hand->n_cards -5; i++) {
    int x = is_straight_at(hand, i, fs);
    if (x != 0){
      if (x < 0) { //ace low straight
	assert(hand->cards[i]->value == VALUE_ACE &&
	       (fs == NUM_SUITS || hand->cards[i]->suit == fs));
	ans->cards[4] = hand->cards[i];
	size_t cpind = i+1;
	while(hand->cards[cpind]->value != 5 ||
	      !(fs==NUM_SUITS || hand->cards[cpind]->suit ==fs)){
	  cpind++;
	  assert(cpind < hand->n_cards);
	}
	copy_straight(ans->cards, hand, cpind, fs,4) ;
      }
      else {
	copy_straight(ans->cards, hand, i, fs,5);
      }
      return 1;
    }
  }
  return 0;
}


//This function puts all the hand evaluation logic together.
//This function is longer than we generally like to make functions,
//and is thus not so great for readability :(
hand_eval_t evaluate_hand(deck_t * hand) {
  suit_t fs = flush_suit(hand);
  hand_eval_t ans;
  if (fs != NUM_SUITS) {
    if(find_straight(hand, fs, &ans)) {
      ans.ranking = STRAIGHT_FLUSH;
      return ans;
    }
  }
  unsigned * match_counts = get_match_counts(hand);
  unsigned n_of_a_kind = get_largest_element(match_counts, hand->n_cards);
  assert(n_of_a_kind <= 4);
  size_t match_idx = get_match_index(match_counts, hand->n_cards, n_of_a_kind);
  ssize_t other_pair_idx = find_secondary_pair(hand, match_counts, match_idx);
  free(match_counts);
  if (n_of_a_kind == 4) { //4 of a kind
    return build_hand_from_match(hand, 4, FOUR_OF_A_KIND, match_idx);
  }
  else if (n_of_a_kind == 3 && other_pair_idx >= 0) {     //full house
    ans = build_hand_from_match(hand, 3, FULL_HOUSE, match_idx);
    ans.cards[3] = hand->cards[other_pair_idx];
    ans.cards[4] = hand->cards[other_pair_idx+1];
    return ans;
  }
  else if(fs != NUM_SUITS) { //flush
    ans.ranking = FLUSH;
    size_t copy_idx = 0;
    for(size_t i = 0; i < hand->n_cards;i++) {
      if (hand->cards[i]->suit == fs){
	ans.cards[copy_idx] = hand->cards[i];
	copy_idx++;
	if (copy_idx >=5){
	  break;
	}
      }
    }
    return ans;
  }
  else if(find_straight(hand,NUM_SUITS, &ans)) {     //straight
    ans.ranking = STRAIGHT;
    return ans;
  }
  else if (n_of_a_kind == 3) { //3 of a kind
    return build_hand_from_match(hand, 3, THREE_OF_A_KIND, match_idx);
  }
  else if (other_pair_idx >=0) {     //two pair
    assert(n_of_a_kind ==2);
    ans = build_hand_from_match(hand, 2, TWO_PAIR, match_idx);
    ans.cards[2] = hand->cards[other_pair_idx];
    ans.cards[3] = hand->cards[other_pair_idx + 1];
    if (match_idx > 0) {
      ans.cards[4] = hand->cards[0];
    }
    else if (other_pair_idx > 2) {  //if match_idx is 0, first pair is in 01
      //if other_pair_idx > 2, then, e.g. A A K Q Q
      ans.cards[4] = hand->cards[2];
    }
    else {       //e.g., A A K K Q
      ans.cards[4] = hand->cards[4];
    }
    return ans;
  }
  else if (n_of_a_kind == 2) {
    return build_hand_from_match(hand, 2, PAIR, match_idx);
  }
  return build_hand_from_match(hand, 0, NOTHING, 0);
}
