#include<stdio.h>
size_t maxSeq(int * array, size_t n){
    if ( n == 0) return 0;
    size_t count = 1;
    size_t maxCount = 1;
    for(int i = 1; i < n; i++){
        if( *(array + i) > *(array + i - 1)){
            count = count + 1;
        }
        else{
            if( count > maxCount ) maxCount= count;
            count = 1;
        }
    }
    return maxCount > count ? maxCount : count;
}
