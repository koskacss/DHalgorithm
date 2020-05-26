#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <gmp.h>

void Diffie_Hellman(int mode); // Function for demonstrate Diffie-Hellman Algorithm
int is_primitive(mpz_t num, mpz_t phi, mpz_t p); // Function for checking if prime is primitive
void generate_private_key(mpz_t *key); // Function for generating random number
void get_public_key(mpz_t *key,mpz_t *p, mpz_t *g, mpz_t* exp); // Function for calculating public keys
void get_final_key(mpz_t *final_key,mpz_t *g, mpz_t *a, mpz_t *b, mpz_t *p); // Function for getting final key
void crypt_message(mpz_t *key, char *msg); // Function for crypt message via key
void Pohlig_algorithm(mpz_t *key, mpz_t *primitie, mpz_t *module,int degree, mpz_t *A, mpz_t *B); // Function that immitate pohlig-hellman algorithm
void Pohlig_calculating_func(int degree,mpz_t *module, mpz_t *key, mpz_t *primitive, mpz_t *x);
void print_factorization(int degree);
void check_z(mpz_t *z, mpz_t *module,mpz_t *q);


int main()
{
    short chooce;

    puts("1 - Demonstrate Diffie - Hellman");
    puts("2 - Demonstrate Pohlig - Hellman");
    scanf("%hi", &chooce);

    Diffie_Hellman(chooce);

    return 0;
}

void Diffie_Hellman(int mode)
{
    int run = 1,j,degree,seed;
    char *Alice_message = malloc(512*sizeof(char));;
    mpz_t  tmp_two,two,p,phi,primitive,one,Alice_key_private,Bob_key_private,Alice_public_key,Bob_public_key,final_key_Alice,final_key_Bob;
    mpz_inits(tmp_two,two,p,phi,primitive,one,Alice_key_private,Bob_key_private,Alice_public_key,Bob_public_key,final_key_Alice,final_key_Bob,'\0');


    mpz_add_ui(one,one,1);
    mpz_add_ui(primitive,primitive,2);

    srand(time(NULL));

    if(mode == 1){
        degree = rand();

        gmp_randstate_t state;
        gmp_randinit_mt(state);
        gmp_randseed_ui(state, seed);


        for(j = 0; j < rand() && (mpz_probab_prime_p(p,15) != 2); j++) {
            mpz_urandomb(p,state,768);
        }
    } else {
        mpz_add_ui(two,two,2);
        degree = rand() % 256 + 10; // Generate degree of two

        mpz_pow_ui(tmp_two,two,degree);
        mpz_add_ui(tmp_two,tmp_two,1);

        while((mpz_probab_prime_p(tmp_two,15)) != 2){
            degree = rand() % 256 + 10; // Generate degree of two

            mpz_pow_ui(tmp_two,two,degree);
            mpz_add_ui(tmp_two,tmp_two,1);
        }

        mpz_set(p, tmp_two);
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
    if(mode == 2){
        puts("\nPohlig algorithm:");
        Pohlig_algorithm(&final_key_Alice,&primitive,&p,degree, &Alice_public_key, &Bob_public_key);
    }
    puts("");

    mpz_clears(p,phi,primitive,one, '\0');

}

void generate_private_key(mpz_t *key)
{
    int j,seed;


    seed = rand();

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);

    for(j = 0; j < rand(); j++) {
       mpz_urandomb(*key,state,768);
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

void Pohlig_algorithm(mpz_t *key, mpz_t *primitive, mpz_t *module, int degree, mpz_t *A, mpz_t *B)
{
    mpz_t x1,x2,final_key;
    mpz_inits(x1,x2,final_key,'\0');

    Pohlig_calculating_func(degree,module,A,primitive, &x1);
    Pohlig_calculating_func(degree,module,B,primitive, &x2);

    puts("\nWe know that g^(a*b) = final key mod p...");
    mpz_mul(x1,x1,x2);
    mpz_powm(final_key,*primitive,x1,*module);
    puts("\nCalculated final key:");
    mpz_out_str(stdout,10,final_key);

}

void Pohlig_calculating_func(int degree,mpz_t *module, mpz_t *key, mpz_t *primitive, mpz_t *x)
{
    mpz_t z_tmp_tmp,z_tmp,q,mul,neg_q,m,z,module_tmp;
    mpz_inits(z_tmp_tmp,z_tmp,q,mul,neg_q,m,z,module_tmp,'\0');

    puts("\np-1 factorise as: \n");

    print_factorization(degree); // Print factorization of p-1

    mpz_sub_ui(module_tmp,*module,1); // Setting p = p-1
    mpz_divexact_ui(m,module_tmp,2);

    for(int i = 0; i < degree; i++){
        if(i == 0){
            mpz_powm(z,*key,m,*module);
            check_z(&z,module,&q);
            mpz_neg(neg_q,q);
            mpz_powm(z,*primitive,neg_q,*module);
            mpz_mul(z,z,*key);
            mpz_divexact_ui(m,module_tmp,4);
        } else {
            mpz_set(z_tmp_tmp,z);
            mpz_powm(z,z,m,*module);
            check_z(&z,module,&q);
            mpz_neg(neg_q,q);
            mpz_mul_ui(mul,neg_q,pow(2,i));
            mpz_powm(z_tmp,*primitive,mul,*module);
            mpz_mod(z,z_tmp_tmp,*module);
            mpz_mul(z,z,z_tmp);
            mpz_mod(z,z,*module);
            mpz_divexact_ui(m,module_tmp,pow(2,i+2));
        }
        mpz_mul_ui(mul,q,pow(2,i));
        mpz_add(*x,mul,*x);
    }

    printf("\nCalculated key:");
    mpz_out_str(stdout,10,*x);

    mpz_clears(z_tmp_tmp,z_tmp,q,mul,neg_q,m,z,module_tmp,'\0');
}

void print_factorization(int degree)
{
    for(int i = 0; i < degree; i++){
        if(i == degree-1) printf("q%d*2^%d", i,i);
        else printf("q%d*2^%d+",i,i);
    }
}

void check_z(mpz_t *z, mpz_t *module, mpz_t *q)
{
    mpz_t one,neg_one,tmp;
    mpz_inits(one,neg_one,tmp,'\0');

    mpz_set_ui(one,1);
    mpz_neg(neg_one, one);

    mpz_sub_ui(tmp,*module,1);

    if((mpz_cmp(*z,tmp)) == 0) mpz_set_ui(*q,1);
    if((mpz_cmp(*z,one)) == 0) mpz_set_ui(*q,0);

    mpz_clears(one,neg_one,tmp,'\0');
}


