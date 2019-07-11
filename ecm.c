/*****************************************************************************/
/* This program is free software; you can redistribute it and/or modify	     */
/* it under the terms of the GNU General Public License as published by	     */
/* the Free Software Foundation; either version 2 of the License, or	     */
/* (at your option) any later version.					     */
/* This program is distributed in the hope that it will be useful,	     */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of	     */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	     */
/* GNU General Public License for more details.				     */
/*									     */
/* You should have received a copy of the GNU General Public License	     */
/* along with this program; if not, write to the Free Software		     */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*****************************************************************************/
/*	Coded by Ardoino Paolo <ardoino.gnu@disi.unige.it>                   */
/*****************************************************************************/
/*****************************************************************************/
/*	Idea from:     Jeff Hamnlin's   				     */
/*****************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <gmp.h>

#define VERSION "0.1"
#define MAXFBINT 2000000
#define MAXNSZ 200

void help(char *arg0);
long long int getprimes(char *fb_file, long long int **prime);
void genprimes(char *maxfbint, char *fb_file);
void chk_perfectpow(mpz_t *mpz_N);
void fec(char *N_file, char *fb_file, char *res_file);

int main(int argc, char *argv[])
{
  printf("Elliptic Curve Method Implementation v%s for large integers factorization.\n", VERSION);
  printf("Code by Ardoino Paolo <ardoino.gnu@disi.unige.it>\n");
  if (argc < 4)
    help(*argv);
  else if (argc == 4 && strcmp(*(argv + 1), "-g") == 0 && *(argv + 2) && *(argv + 3))
    genprimes(*(argv + 2), *(argv + 3));
  else if (argc == 5 && strcmp(*(argv + 1), "-f") == 0 && *(argv + 2) && *(argv + 3) && *(argv + 4))
    fec(*(argv + 2), *(argv + 3), *(argv + 4));
  else
    help(*argv);
  return 0;
}

void fec(char *N_file, char *fb_file, char *res_file)
{
  mpz_t mpz_N, a , gcd, exp;
  mpz_t x, y, x1, y1, tmp1, b;
  FILE *fp=NULL;
  struct timeval tm0, tm1;
  long long int ctr, *prime, pnum=0, pctr=0;
  char N[MAXNSZ] = {'\0'};
  int foundfactor=0;

  if (!(fp = fopen(N_file, "r"))) {
    fprintf(stderr,"Error: cannot open N file.\n");
    exit(EXIT_FAILURE);
  }
  fscanf(fp, "%s", N);
  fclose(fp);
  fp = fopen(res_file, "w");
  printf("N[integer to factorize]: %s\n", N);
  fprintf(fp, "N[integer to factorize]: %s\n", N);
  pnum = getprimes(fb_file, &prime);
  fprintf(stderr,"Loaded %ld primes in memory as factor base.\n",pnum);
  fprintf(fp, "Loaded %ld primes in memory as factor base.\n",pnum);
  mpz_init_set_str(mpz_N, N, 10);
  mpz_init(a);
  mpz_init(b);
  mpz_init(gcd);
  mpz_init(exp);
  mpz_init(x);
  mpz_init(y);
  mpz_init(x1);
  mpz_init(y1);
  mpz_init(tmp1);
  gettimeofday(&tm0, NULL);
  while (mpz_mod_ui(tmp1, mpz_N, 2) == 0) {
    printf("Factor: 2\n");
    fprintf(fp, "Factor: 2\n");
    mpz_div_ui(mpz_N, mpz_N, 2);
  }
  if (mpz_cmp_ui(mpz_N, 1) == 0) {
    gettimeofday(&tm1, NULL);
    //printf("Factorization has been completed in %ld microseconds.\n"\
    //,(tm1.tv_sec*1000000 + tm1.tv_usec) - (tm0.tv_sec*1000000 + \
    //tm0.tv_usec));
    //fprintf(fp, "Factorization has been completed in %ld \
    //microseconds.\n", (((long double)tm1.tv_sec)*1000000 + tm1.tv_usec)\
    //- (((long double)tm0.tv_sec)*1000000 + tm0.tv_usec)); 
    printf("Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
    fprintf(fp, "Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
    mpz_clear(mpz_N);
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(gcd);
    mpz_clear(exp);
    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(x1);
    mpz_clear(y1);
    mpz_clear(tmp1);
    free(prime);
    fclose(fp);
    exit(EXIT_SUCCESS);
  }
  if (mpz_probab_prime_p(mpz_N, 10) > 0) {
    gmp_printf("Factor: %Zd\n", mpz_N);
    gmp_fprintf(fp, "Factor: %Zd\n", mpz_N);
    gettimeofday(&tm1, NULL);
    //printf("Factorization has been completed in %ld microseconds.\n",\
    //(tm1.tv_sec*1000000 + tm1.tv_usec) - (tm0.tv_sec*1000000 +\
    //tm0.tv_usec)); 
    //fprintf(fp, "Factorization has been completed in %ld \
    //microseconds.\n", (tm1.tv_sec*1000000 + tm1.tv_usec) - \
    //(tm0.tv_sec*1000000 + tm0.tv_usec)); 
    printf("Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
    fprintf(fp, "Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
    mpz_clear(mpz_N);
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(gcd);
    mpz_clear(exp);
    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(x1);
    mpz_clear(y1);
    mpz_clear(tmp1);
    free(prime);
    fclose(fp);
    exit(EXIT_SUCCESS);
  }
  printf("No trivial prime factors found!\n");
  while (1) {
    /*if (foundfactor == 1) {
      chk_perferctpow(&mpz_N);
      foundfactor = 0;
    }*/
    //mpz_random(x, mpz_size(mpz_N));
    //mpz_random(y, mpz_size(mpz_N));
    mpz_random(a, mpz_size(mpz_N));
    //mpz_mod(x, x, mpz_N);
    //mpz_mod(y, y, mpz_N);
    mpz_mod(a, a, mpz_N);
    mpz_set_ui(x, 0);
    mpz_set_ui(y, 1);
    mpz_pow_ui(tmp1, x, 3);
    mpz_pow_ui(b, y, 2);
    mpz_sub(b, b, tmp1);
    mpz_mul(tmp1, a, x);
    mpz_sub(b, b, tmp1);
    mpz_pow_ui(tmp1, a, 3);
    mpz_pow_ui(b, b, 2);
    mpz_mul_ui(tmp1, tmp1, 4);
    mpz_mul_ui(b, b, 27);
    mpz_add(tmp1, tmp1, b);
    mpz_gcd(gcd, tmp1, mpz_N);
    if (mpz_cmp(gcd, mpz_N) != 0) {
      if (mpz_cmp_ui(gcd, 1) > 0) {
        gmp_printf("Factor: %Zd\n", gcd);
        gmp_fprintf(fp, "Factor: %Zd\n", gcd);
        mpz_div(mpz_N, mpz_N, gcd);
        foundfactor = 1;
        if (mpz_cmp_ui(mpz_N, 1) == 0) {
          gettimeofday(&tm1, NULL);
          printf("Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
          fprintf(fp, "Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
          mpz_clear(mpz_N);
          mpz_clear(a);
          mpz_clear(b);
          mpz_clear(gcd);
          mpz_clear(exp);
          mpz_clear(x);
          mpz_clear(y);
          mpz_clear(x1);
          mpz_clear(y1);
          mpz_clear(tmp1);
          free(prime);
          fclose(fp);
          exit(EXIT_SUCCESS);
      }  // if (mpz_cmp_ui
      if (mpz_probab_prime_p(mpz_N, 10) > 0) {
          gmp_printf("Factor: %Zd\n", mpz_N);
          gmp_fprintf(fp, "Factor: %Zd\n", mpz_N);
          gettimeofday(&tm1, NULL);
          printf("Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
          fprintf(fp, "Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
          mpz_clear(mpz_N);
          mpz_clear(a);
          mpz_clear(b);
          mpz_clear(gcd);
          mpz_clear(exp);
          mpz_clear(x);
          mpz_clear(y);
          mpz_clear(x1);
          mpz_clear(y1);
          mpz_clear(tmp1);
          free(prime);
          fclose(fp);
          exit(EXIT_SUCCESS);
        }
      } else {
        mpz_set(x1, x);
        mpz_set(y1, y);
        printf("Next round...\n");
            for (ctr=1; ctr<pnum; ctr++) {
              mpz_mul(x, x1, x1);
              mpz_mul_ui(x, x, 3);
              mpz_add(x, x, a);
              mpz_mul_ui(y, y1, 2);
              mpz_div(x, x, y);
              mpz_set(y, x);
              mpz_mul(x, x, x);
              mpz_mul_ui(tmp1, x1, 2);
              mpz_sub(x, x, tmp1);
              mpz_sub(x1, x1, x);
              mpz_mul(y, y, x1);
              mpz_sub(y1, y, y1);
              mpz_gcd(gcd, x1, mpz_N);
              //gmp_printf("x= %Zd\n", x);
              if (mpz_probab_prime_p(gcd, 10) > 0 && mpz_cmp_ui(gcd, 1) != 0) {
                gmp_printf("Factor: %Zd\n", gcd);
                gmp_fprintf(fp, "Factor: %Zd\n", gcd);
                mpz_div(mpz_N, mpz_N, gcd);
                foundfactor = 1;
                if (mpz_cmp_ui(mpz_N, 1) == 0) {
                  gettimeofday(&tm1, NULL);
                  printf("Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
                  fprintf(fp, "Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
                  mpz_clear(mpz_N);
                  mpz_clear(a);
                  mpz_clear(b);
                  mpz_clear(gcd);
                  mpz_clear(exp);
                  mpz_clear(x);
                  mpz_clear(y);
                  mpz_clear(x1);
                  mpz_clear(y1);
                  mpz_clear(tmp1);
                  free(prime);
                  fclose(fp);
                  exit(EXIT_SUCCESS);
                }
                if (mpz_probab_prime_p(mpz_N, 10) > 0) {
                  gmp_printf("Factor: %Zd\n", mpz_N);
                  gmp_fprintf(fp, "Factor: %Zd\n", mpz_N);
                  gettimeofday(&tm1, NULL);
                  printf("Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
                  fprintf(fp, "Factorization has been completed in %ld seconds.\n", tm1.tv_sec - tm0.tv_sec); 
                  mpz_clear(mpz_N);
                  mpz_clear(a);
                  mpz_clear(b);
                  mpz_clear(gcd);
                  mpz_clear(exp);
                  mpz_clear(x);
                  mpz_clear(y);
                  mpz_clear(x1);
                  mpz_clear(y1);
                  mpz_clear(tmp1);
                  free(prime);
                  fclose(fp);
                  exit(EXIT_SUCCESS);
                }
              }  // if (mpz_probab_prime_p
              //printf("%d, ", ctr);
            }  // for (ctr
          //}
        //}  // while (pctr < pnum)
        pctr = 0;
      }  // else
    }  // if (mpz_cmp
  }  // while (1)
  return;
}

void chk_perfectpow(mpz_t *mpz_N)
{
  mpz_t mpz_rt;
  unsigned long long int rexp=3;

  gmp_printf("checking for perfect pow: %Zd\n",*mpz_N);
  mpz_init(mpz_rt);
  mpz_set(mpz_rt, *mpz_N);
  while (mpz_cmp_ui(mpz_rt, 2) > 0) {
    if (mpz_root(mpz_rt, *mpz_N, rexp) != 0) {
      mpz_set(*mpz_N, mpz_rt);
      break;
    } else 
      rexp += 2;
  }
}

void genprimes(char *maxfbint, char *fb_file)
{
  FILE *fp=NULL;
  mpz_t mpz_max, mpz_prime;

  printf("Generating primes file.\n");
  fp = fopen(fb_file, "w");
  mpz_init_set_str(mpz_max, maxfbint, 10);
  mpz_init_set_ui(mpz_prime, 1);
  while (mpz_cmp(mpz_prime, mpz_max) < 0) {
    mpz_nextprime(mpz_prime, mpz_prime);
    gmp_fprintf(fp, "%Zd\n", mpz_prime);
  }
  printf("Done.\n");
  return;
}

long long int getprimes(char *fb_file, long long int **prime)
{
  FILE *fp=NULL;
  long long int pctr=0, tprime=0;
  int size=10;
  char primech[10];
  
  if (!(fp = fopen(fb_file, "r"))) {
    fprintf(stderr,"Error: cannot open fb file.\n");
    exit(EXIT_FAILURE);
  }
  while (!feof(fp) && pctr < MAXFBINT) {
    fgets(primech, size, fp);
    pctr++;
  }
  rewind(fp);
  *prime = malloc(sizeof(long long int) * pctr);
  pctr = 0;
  while (!feof(fp) && pctr < MAXFBINT) {
    fgets(primech, size, fp);
    tprime = atol(primech);
    if (pctr > 0 && tprime != *(*prime + pctr - 1))
      *(*prime + pctr) = tprime;
    pctr++;
  }
  pctr--;
  fclose(fp);
  return pctr;
}

void help(char *arg0)
{
  printf("Usage:\n");
  printf("%s -g <max_fb_int> <fb_file>\n", arg0);
  printf("\t<fb_file> is the file containing small prime factors.\n");
  printf("%s -f <N_file> <fb_file> <results_file>\n", arg0);
  printf("\t<N_file> is the file containing ONLY the integer N to factorize.\n");
  printf("\t<fb_file> is the file containing small prime factors.\n");
  printf("\t<results_file> is the file where ECMI store results.\n");
  exit(EXIT_FAILURE);
}
