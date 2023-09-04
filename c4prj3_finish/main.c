#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"



int main(int argc, char ** argv) {
  //initialize
  if (argc > 3 || argc < 2) {
    fprintf(stderr,"Usage: input_file_name <Monte Carlo trials>\n");
    return EXIT_FAILURE;
  }
  const unsigned int MONTE_CARLO = argc == 3 ? atoi(argv[2]) : 10000;
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
     perror("Could not open file");
     return EXIT_FAILURE;
  }
  size_t n_hands = 0;
  future_cards_t fc;
  fc.decks = NULL;
  fc.n_decks = 0;
  deck_t** decks = read_input(f,&n_hands,&fc);
  deck_t* remain_deck = build_remaining_deck(decks, n_hands);
  unsigned int win[n_hands + 1];
  for(size_t i = 0; i <= n_hands; i++){
    win[i] = 0;
  }

  //calculate
  for(size_t i = 0; i < MONTE_CARLO; i++){
    shuffle(remain_deck);
    future_cards_from_deck(remain_deck,&fc);
    size_t max_index = 0;
    for(size_t j = 0; j < n_hands; j++){
        if(compare_hands(decks[max_index],decks[j])< 0) max_index = j;
    }
    if(max_index == 0 && compare_hands(decks[0],decks[1]) == 0) win[n_hands]++;
    else win[max_index]++;
  }

  //print your results
  for(size_t i = 0; i < n_hands; i++){
    printf("Hand %zu won %u / %u times (%.2f%%)\n", i, win[i], MONTE_CARLO, (float)win[i]/MONTE_CARLO*100);
  } 
  printf("And there were %u ties\n",win[n_hands]);

  //free any memory you allocated
  free(remain_deck);
  free(fc.decks);
  for(size_t i = 0; i < n_hands; i++){
    free(decks[i]);
  }
  free(decks);

  //close any open files
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

