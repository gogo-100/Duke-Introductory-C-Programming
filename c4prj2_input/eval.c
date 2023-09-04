#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/*
    比较两张牌的大小
    先比较值，若值相等则比较花色
    因为比较的值是牌的指针的指针所以要先做一次转换
*/
int card_ptr_comp(const void * vp1, const void * vp2) {
    const card_t * a = (*(card_t **)vp1);
    const card_t * b = (*(card_t **)vp2);
    if(a -> value > b -> value) return -1;
    else if (a-> value < b-> value) return 1;
    else  return a-> suit <  b-> suit;
}

/*
    判断有没有同花
*/
suit_t flush_suit(deck_t * hand) {
  //printf("check flush......\n");
  qsort(hand->cards,hand->n_cards,sizeof(card_t*), card_ptr_comp);
  card_t** cards = hand -> cards;
  int count [4] = {0,0,0,0};
  for(int i = 0; i < hand -> n_cards ; i++){
     count[cards[i]->suit]++;
  }
  for(int i = 0; i < 4; i++){
    if(count[i]>=5){
        //printf("FLUSH\n");
        suit_t suit = i;
        return suit;
    }
  }
  //printf("NO FLUSH\n");
  return NUM_SUITS;
}

/*
   返回最大值
*/
unsigned get_largest_element(unsigned * arr, size_t n) {
  unsigned maxVal = arr[0];
  for(int i = 0; i < n ; i++){
     if( arr[i] > maxVal ) maxVal = arr[i];
  }
  return maxVal;
}

/*
    在n张牌中找到第一个有n_of_akind连的点数

    match_counts的意思是 统计出现的次数
    如果牌是4d 8c 9h 4h 4c
    那么match_counts 就是 3 1 1 3 3
    统计的是出现的次数

*/
size_t get_match_index(unsigned * match_counts, size_t n,unsigned n_of_akind){
  for(size_t i = 0; i < n; i++){
    if(match_counts[i] == n_of_akind) return i;
  }
  return -1;
}

/*
    找到第二个对子（2连）
*/

ssize_t  find_secondary_pair(deck_t * hand,
			     unsigned * match_counts,
			     size_t match_idx) {
  size_t nextIndex = match_idx + match_counts[match_idx];
  for(size_t i = nextIndex; i < hand -> n_cards; i++ ){
      if(match_counts[i] > 1 && hand -> cards[i][0].value != hand -> cards[match_idx][0].value) return i;
  }
  for (size_t i = 0; i < match_idx; i++) {
      if (match_counts[i] > 1 && hand->cards[i][0].value != hand->cards[match_idx][0].value) return i;
  }
  return -1;
}


/*
    检测花色是否符合要求
*/
int check_suit(suit_t fs, suit_t now){
    if(fs == NUM_SUITS) return 1;
    else return fs == now;
}

/*
    找花色为fs,值为value的卡牌
    如果找到则返回index，没找到则返回-1
*/

int find_next_different_value(deck_t * hand, size_t index, suit_t fs, signed value){
    card_t** cards = hand -> cards;
    for(;index < hand -> n_cards; index++){
        if( cards[index][0].value == value && check_suit(fs, cards[index][0].suit) ) return index;
    }
    return -1;
}

/*
    判断从起点index开始是否是一个花色为fs,长度为n的顺子
*/
int is_n_length_straight_at(deck_t * hand, size_t index, suit_t fs, int n){
    //printf("check N Length STRAIGHT,which starts at %zu, flush is %d, length is %d/// is_n_length_straight_at \n",index,fs,n);
    if(n == 0) return 1;  //递归出口
    signed nowValue = hand -> cards[index][0].value;
    int nextIndex = find_next_different_value(hand, index, fs, nowValue-1);
    if(nextIndex > 0) return is_n_length_straight_at(hand, nextIndex, fs, n-1);
    else return 0;
}

/*
    判断是不是ace_low顺子，如果是返回-1，不是返回0
*/
int is_ace_low_straight_at(deck_t * hand, size_t index, suit_t fs){
    //printf("check ACE-LOW STRAIGHT which index= %zu，suit=%d ///// is_ace_low_straight_at  \n",index,fs);
    int  nextIndex = find_next_different_value(hand,0,fs,5);
    if(nextIndex < 0) return 0;
    else{
        if(is_n_length_straight_at(hand, nextIndex, fs, 3))return -1;
    }
    return 0;

}

/*
    判断从Index开头是不是花色为fs的顺子
*/
int is_straight_at(deck_t * hand, size_t index, suit_t fs) {
    //print_hand(hand);
    //printf("check STRAIGHT which starts at %zu, flush is %d//// is_straight_at\n",index,fs);
    card_t** cards = hand -> cards;
    if(!check_suit(fs,cards[index][0].suit)) return 0;
    if(cards[index][0].value == 14){//要考虑ace-low的情况但也要考虑AKQJ0的情况
        //printf("ace-low situation\n");
        if (is_ace_low_straight_at(hand, 0, fs) == -1) return -1;
    }
    if (cards[index][0].value < 5) { //4.3.2开头的话没有顺子
        //printf("no straight\n");
        return 0;
    }
    else{
        //printf("recursion\n");
        return is_n_length_straight_at(hand, index, fs, 4);
    }
}

/*
    从[left,right]搜索count张单牌，往手牌里添加
    返回成功添加的总张数i
*/
int add_single_cards(deck_t * hand, card_t ** card, int left, int right, int count){
  if(left > right) return 0;
  int i = 0;
  for(; left + i <= right && i < count ; i ++){
      card[5 - count + i] = hand -> cards[left + i];
  }
  return i;
}


/*
    从[left,right]搜索对子，往手牌里添加（存储位置从index开始）
    返回成功添加的总张数
*/

int  add_pair_cards(deck_t * hand, card_t ** card, int left, int right, int index){
  if(left > right) return 0;
  int i = 0;
  for(; left + i <= right ; i ++){
      if(left + i + 1 < hand -> n_cards && card[left + i]->value == card[left + i + 1]->value ){
           card[index] = hand ->  cards[left + i];
           card[index + 1] = hand -> cards[left + i + 1];
           return 2;
      }
  }
  return 0;
}

/*
    构建手牌
    在主体的前面和后面寻找tiebreak
*/
hand_eval_t build_hand_from_match(deck_t * hand,
				  unsigned n,
				  hand_ranking_t what,
				  size_t idx) {
  hand_eval_t ans;
  ans.ranking = what;
  for(int i = 0; i < n ;i++ ){
    ans.cards[i] = hand -> cards[idx + i];
  }
  //找出单张最大的5-n张 deck_t * hand, card_t * card, int left, int right, int count
  //找前面
  int count = add_single_cards(hand, ans.cards, 0, idx - 1, 5 - n );
  //找后面
  if(count + n < 5) add_single_cards(hand, ans.cards, idx + n, hand -> n_cards - 1, 5 - n - count);
  return ans;
}

/*
    比较手牌
*/
int compare_hands(deck_t * hand1, deck_t * hand2) {
  hand_eval_t eval1 = evaluate_hand(hand1);
  hand_eval_t eval2 = evaluate_hand(hand2);
  if(eval1.ranking!=eval2.ranking){
    return eval2.ranking - eval1.ranking;
  }
  else{// 此时比较字符而不是类型，而且类型已经确定所以不会有多的什么对子了
    for(int i = 0 ; i < 5 ; i++){
       if(eval1.cards[i][0].value == eval2.cards[i][0].value) continue;
       else return eval1.cards[i][0].value - eval2.cards[i][0].value;
    }
  }
  return 0;
}

/*
   You will find its prototype after the code you wrote
   in Course 3, and before the functions we
   provided for you.  Replace the prototype with
   your implementation.

   Given a hand (deck_t) of cards, this function
   allocates an array of unsigned ints with as
   many elements as there are cards in the hand.
   It then fills in this array with
   the "match counts" of the corresponding cards.
   That is, for each card in the original hand,
   the value in the match count array
   is how many times a card of the same
   value appears in the hand.  For example,
   given
     Ks Kh Qs Qh 0s 9d 9c 9h
   This function would return
     2  2  2  2  1  3  3  3
   because there are 2 kings, 2 queens,
   1 ten, and 3 nines.
*/
unsigned * get_match_counts(deck_t * hand){
    unsigned* match = malloc(hand->n_cards*sizeof(unsigned));
    int hash[15] = {0};
    for(int i = 0; i < hand->n_cards; i++){
        hash[hand->cards[i]->value]++;
    }
    for(int i = 0; i < hand->n_cards; i++){
        match[i] = hash[hand->cards[i]->value];
    }
    return match;
}


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





