#include<stdio.h>
#include<stdlib.h>

size_t maxSeq(int * array, size_t n);

int main(){
    int a[3] = {1,3,2};
    int b[3] = {2,3,4};
    int c[9] = {1,2,3,0,4,5,6,7,9};
    int d[11] = {1,3,2,4,0,8,-3,6,3,6,9};
    int e[5] = {1,1,1,1,2};
    int f[10] = {1,2,1,3,5,7,2,4,6,9};
    int g[1] = {1};
    int h[3] = {1,1,1};
    int i[] = {3,2,3};
    int j[] = {-3,0,-1,-2,-4,-2,-8,-7,-6,-5,-4};
    int k[] = {5,4,3,2,1,-44,-59};
    if( maxSeq(a,3) != 2 ) return EXIT_FAILURE;
    if( maxSeq(b,3) != 3 ) return EXIT_FAILURE;
    if( maxSeq(d,11) != 3 ) return EXIT_FAILURE;
    if( maxSeq(e,5) != 2 ) return EXIT_FAILURE;
    if( maxSeq(f,10) != 4 ) return EXIT_FAILURE;
    if( maxSeq(c,9) != 6 ) return EXIT_FAILURE;
    if( maxSeq(g,1) != 1 ) return EXIT_FAILURE;
    if( maxSeq(h,3) != 1 ) return EXIT_FAILURE;
    if( maxSeq(i,2) != 1 ) return EXIT_FAILURE;
    if( maxSeq(i,3) != 2 ) return EXIT_FAILURE;
    if( maxSeq(i,0) != 0 ) return EXIT_FAILURE;
    if( maxSeq(j,11) != 5 ) return EXIT_FAILURE;
    if( maxSeq(k,7) != 1) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
