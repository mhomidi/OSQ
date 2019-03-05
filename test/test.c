
#include <stdio.h>
#include <time.h>
#include <math.h>
int main()
{
    int i;
    clock_t CPU_time_1 = clock();
    printf("CPU start time is : %d \n", CPU_time_1);
    for(i = 0; i < 150000000; i++);
    clock_t CPU_time_2 = clock();
    printf("CPU end time is : %d", CPU_time_2);
}