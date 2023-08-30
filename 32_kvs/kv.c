#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"


void splitLine(kvpair_t* kv, char* line){
    int index = 0;
    for(;line[index]!='\0'&&line[index]!='=';index++);
    kv->key = malloc((index + 1) * sizeof(char));
    strncpy(kv->key,line,index);
    kv->key[index] = '\0';
    int i;
    for(i = index+1; line[i]!='\n' && line[i]!= '\0';i++);
    kv->value = malloc((i - index)*sizeof(char));
    strncpy(kv->value,line + index + 1 ,i - index - 1);
    kv->value[i - index - 1] = '\0';
}

void readLines(FILE * f, kvarray_t* array){
    array -> kvarray = NULL;
    char * curr = NULL;
    size_t sz = 0;
    size_t i = 0;
    while(getline(&curr,&sz,f)>=0){
        array->kvarray = realloc(array->kvarray,(i+1)*sizeof(kvpair_t));
        splitLine(&(array->kvarray[i]),curr);
        i++;
    }
    array->length = i;
    free(curr);
    return ;
}


kvarray_t * readKVs(const char * fname) {
   FILE * f = fopen(fname, "r");
   kvarray_t* array = malloc(sizeof(kvarray_t));
   if (f == NULL) {
        perror("Could not open file");
        return NULL;
    }

    readLines(f,array);

    if (fclose(f) != 0) {
        perror("Failed to close the input file!");
        return NULL;
    }

   return array;
}

void freeKVs(kvarray_t * pairs) {
   for(int i = 0; i < pairs->length; i++){
       free(pairs->kvarray[i].key);
       free(pairs->kvarray[i].value);
   }
   free(pairs->kvarray);
   free(pairs);
}


void printKVs(kvarray_t * pairs) {
    for(int i = 0; i < pairs->length; i++){
        printf("key = '%s' value = '%s'\n",pairs->kvarray[i].key,pairs->kvarray[i].value);
    }
}



char * lookupValue(kvarray_t * pairs, const char * key) {
    for(int i = 0; i < pairs->length; i++){
        if(strcmp(key,pairs->kvarray[i].key) == 0) return pairs -> kvarray[i].value;
    }
    return NULL;
}

