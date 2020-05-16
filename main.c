#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gmp.h>


int main()
{
    int flag = 0,flag2 = 1;
    mpz_t p,phi,primitive,test,one,tmp,test1,i;
    mpz_inits(p,phi,primitive,test,one,tmp,i,test1,'\0');

    mpz_add_ui(one,one,1);
    mpz_add_ui(primitive,primitive,2);
    mpz_add_ui(i,i,1);

    srand(time(NULL));

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    
    for(int i = 0; i < rand();i++){
        mpz_urandomb(tmp,state,16);
    }

    mpz_nextprime(p, tmp);

    puts("Randomly generated p:");
    mpz_out_str(stdout,10,p);

    mpz_sub_ui(phi,p,1);

    while(flag2 == 1){
        mpz_powm(test,primitive,phi,p);

        if((mpz_cmp(test,one) == 0)){ // Если по модулю равен 1, то нужна проверка на второе условие 
            for(;(mpz_cmp(i,phi) < 0) && (flag == 0);mpz_add_ui(i,i,1)){
                mpz_powm(test1,primitive,i,p);
                if(mpz_cmp(test1,one) == 0){  
                    flag = 1;
                }
            }
            if(flag == 0) flag2 = 0;
        } else {mpz_add_ui(primitive,primitive,1);}
    }


    puts("\nPrimitive root:");
    mpz_out_str(stdout,10,primitive);

    mpz_clears(p,phi,primitive,test,one,tmp,'\0');

    return 0;
}
