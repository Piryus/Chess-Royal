#include <stdio.h>
#include <stdlib.h>

int checkDiviseursPremiers(int Mul1,int Mul2){
    int result = 1;
    int a = Mul1*Mul2;
    int b = a-1;
    do{
    if(a%b == 0)if((a!=Mul1)&(a!=Mul2)){result = 0;}
    b --;
    }while(b>1);
    return result;
}

int main()
{
    int Mul1 = 2;
    int Mul2 = 3;
    int Result;
    int quit=1;
    do{
        while(quit>=0){
            if(checkDiviseursPremiers(Mul1,Mul2)){printf(" %9d = %9d  x  %9d \n",Mul1*Mul2,Mul1,Mul2);}
            quit--;
        }
        printf("\nIter : ");
        scanf("%d",&quit);
    }while(quit>=0);
    printf("Hello world!\n");
    return 0;
}
