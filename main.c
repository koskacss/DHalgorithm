#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

int is_primitive(mpz_t num, mpz_t phi, mpz_t p); // Function for checking if prime is primitive
void generate_private_key(mpz_t *key); // Function for generating random number
void get_public_key(mpz_t *key,mpz_t *p, mpz_t *g, mpz_t* exp); // Function for calculating public keys
void get_final_key(mpz_t *final_key,mpz_t *g, mpz_t *a, mpz_t *b, mpz_t *p); // Function for getting final key
void crypt_message(mpz_t *key, char *msg); // Function for crypt message via key

int main()
{
    int seed;
    int run = 1,j;
    char *Alice_message = malloc(512*sizeof(char));;
    mpz_t p,phi,primitive,one,Alice_key_private,Bob_key_private,Alice_public_key,Bob_public_key,final_key_Alice,final_key_Bob;
    mpz_inits(p,phi,primitive,one,Alice_key_private,Bob_key_private,Alice_public_key,Bob_public_key,final_key_Alice,final_key_Bob,'\0');


    mpz_add_ui(one,one,1);
    mpz_add_ui(primitive,primitive,2);

    srand(time(NULL));
    seed = rand();

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
    
    puts("What is Alice message?:");
    getchar();
    fgets(Alice_message,512,stdin);
    Alice_message[strlen(Alice_message) - 1] = '\0';

    for(j = 0; j < rand() && ((mpz_probab_prime_p(p,15)) != 2); j++) {
       mpz_urandomb(p,state,768);
    }

    mpz_sub(phi,p,one);

    while((is_primitive(primitive,phi,p)) != 1){
        mpz_nextprime(primitive,primitive);
    }

    generate_private_key(&Alice_key_private);
    generate_private_key(&Bob_key_private);

    get_public_key(&Alice_public_key,&p,&primitive,&Alice_key_private);
    get_public_key(&Bob_public_key,&p,&primitive,&Bob_key_private);

    get_final_key(&final_key_Alice, &primitive, &Alice_key_private, &Bob_key_private, &p);
    get_final_key(&final_key_Bob, &primitive, &Bob_key_private, &Alice_key_private, &p);

    crypt_message(&final_key_Alice, Alice_message);

    puts("Randomly generated p:");
    mpz_out_str(stdout,10,p);
    puts("\nPrimitive root:");
    mpz_out_str(stdout,10,primitive);
    puts("\nAlice`s private key:");
    mpz_out_str(stdout,10,Alice_key_private);
    puts("\nBob`s private key:");
    mpz_out_str(stdout,10,Bob_key_private);
    puts("\nAlice`s public key:");
    mpz_out_str(stdout,10,Alice_public_key);
    puts("\nBob`s public key:");
    mpz_out_str(stdout,10,Bob_public_key);
    puts("\nAlice`s final key:");
    mpz_out_str(stdout,10,final_key_Alice);
    puts("\nBob`s final key:");
    mpz_out_str(stdout,10,final_key_Bob);
    puts("\nCrypted Message:");
    puts(Alice_message);
    puts("");


    mpz_clears(p,phi,primitive,one, '\0');

    return 0;
}

void generate_private_key(mpz_t *key)
{
    int j,seed;


    seed = rand();

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);

    for(j = 0; j < rand(); j++) {
       mpz_urandomb(*key,state,256);
    }

}

void get_public_key(mpz_t *key,mpz_t *p, mpz_t *g, mpz_t* exp)
{
    mpz_powm(*key,*g,*exp,*p);
}

void get_final_key(mpz_t *final_key,mpz_t *g, mpz_t *a, mpz_t *b, mpz_t *p)
{
    mpz_t tmp;
    mpz_init(tmp);

    mpz_mul(tmp, *a, *b);

    mpz_powm(*final_key, *g, tmp ,*p);
}

int is_primitive(mpz_t num, mpz_t phi, mpz_t p)
{
    int res = 1, run = 1;
    mpz_t test, one, i, test1;
    mpz_inits(test, one, i, test1 ,'\0');
    mpz_add_ui(one,one,1);
    mpz_add_ui(i,i,1);

    mpz_powm(test,num,phi,p);
    if((mpz_cmp(test,one)) == 0){
        while(mpz_cmp(i,phi) < 0){
            mpz_powm(test1,num,i,p);
            if(mpz_cmp(test1,one) == 0){
                res = 0;
                return res;
            }
            mpz_add_ui(i,i,1);
        }
    }

    mpz_clears(test, one, i, test1 ,'\0');

    return res;
}

void crypt_message(mpz_t *key, char *msg)
{
    int i;
    mpz_t shift, module;
    mpz_inits(shift, module, '\0');

    mpz_add_ui(module,module,127);

    mpz_mod(shift,*key,module);

    mpz_get_ui(shift);

    for(i = 0; i < strlen(msg)+1; i++){
        msg[i] = msg[i] + i;
    }
    msg[i+1] = '\0';
}