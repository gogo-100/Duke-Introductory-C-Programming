#include <stdio.h>
#include <stdlib.h>

struct _retire_info{
    int months;
    double contribution;
    double rate_of_return;
};

typedef struct _retire_info retire_info;

double calc(int startAge,double initial,retire_info info){
    int month;
    for(month = 0; month < info.months; month++){
        printf("Age %3d month %2d you have $%.2lf\n",(startAge+month)/12,(startAge+month)%12,initial);
        double delta = initial * info.rate_of_return + info.contribution;
        initial += delta;
    }
    return initial;
}

void retirement(int startAge,double initial,retire_info working,retire_info retired){
  initial = calc(startAge,initial,working);
  calc(startAge+working.months,initial,retired);
}

int main(){
    retire_info working,retired;
    working.months = 489;
    working.contribution = 1000;
    working.rate_of_return = 0.045/12;
    retired.months = 384;
    retired.contribution = -4000;
    retired.rate_of_return = 0.01/12;
    retirement(327,21345,working,retired);
    return 1;
}
