#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned power (unsigned x, unsigned y);

int main(){
    if(power(0,0)!=1)return EXIT_FAILURE;
    if(power(1,23)!=1)return EXIT_FAILURE;
    if(power(2,10)!=1024)return EXIT_FAILURE;
    if(power(3,3)!=27)return EXIT_FAILURE;
    if(power(4,1)!=4)return EXIT_FAILURE;
    if(power(5,0)!=1)return EXIT_FAILURE;
    if(power(0,23)!=0)return EXIT_FAILURE;
    if(power(5,7)!=78125)return EXIT_FAILURE;
    if(power(5,2)!=25)return EXIT_FAILURE;
    if(power(22,7)!=2494357888)return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

