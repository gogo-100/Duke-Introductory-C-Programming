#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"


counts_t * createCounts(void) {
  counts_t* counts = malloc(sizeof(counts_t));
  counts -> length = 0;
  counts -> unknown = 0;
  counts -> counts = NULL;
  return counts;
}

int findIndex(counts_t * c, const char * name) {
    for(int i = 0 ; i < c->length; i++){
        if(strcmp(name,c->counts[i]->str) == 0) return i;
    }
    return -1;
}

int addOneCount(counts_t * c, const char * name){
    c->counts = realloc(c->counts,(c->length+1)*sizeof(one_count_t*));
    c->counts[c->length] = malloc(sizeof(one_count_t));
    c->counts[c->length]->count = 1;
    //这个name会被free掉吗？？
    c->counts[c->length]->str = (char*)name;
    c->length++;
    return c->length - 1;
}

void countIncrement(counts_t * c,int index){
    c->counts[index]->count++;
}

void addCount(counts_t * c, const char * name) {
    if(name == NULL){
	c->unknown++;
	return;
    }
    int index = findIndex(c,name);
    if(index == -1){
        index = addOneCount(c,name);
    }
    else countIncrement(c,index);
}


void printCounts(counts_t * c, FILE * outFile) {
    for(int i = 0; i < c->length; i++){
        fprintf(outFile,"%s: %d\n",c->counts[i]->str,c->counts[i]->count);
    }
    if(c->unknown != 0){
        fprintf(outFile,"<unknown> : %d\n",c->unknown);
    }
}


void freeCounts(counts_t * c) {
    for(int i = 0 ; i < c->length; i++){
        //free(c->counts[i]->str); str is not heap memory
        free(c->counts[i]);
    }
    free(c->counts);
    free(c);
}

