/*--------------------------------------------------------------------
This source distribution is placed in the public domain by its author,
Jason Papadopoulos. You may use it for any purpose, free of charge,
without having to notify anyone. I disclaim any responsibility for any
errors.

Optionally, please be nice and tell me if you find this source to be
useful. Again optionally, if you add to the functionality present here
please consider making those additions public too, so that others may 
benefit from your work.	

$Id$
--------------------------------------------------------------------*/

//
// gcc.exe -Wall -O2 -o tiny_quadsieve.exe tiny_quadsieve.c prime_delta.c -l gmp
//

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiny_quadsieve.h"

/* Basic MPQS implementation, intended for tiny inputs. Note
   that this code is very old and inefficient; it's only here
   to factor numbers that are small enough to be 'below the radar' 
   of the main SIQS routines. The practical upper limit on the
   input size is 85 bits, about 25-26 digits. */

/*---------------------------------------------------------------*/
int32_t mpz_legendre_ui(uint32_t a, uint32_t p) {

	uint32_t x, y, tmp;
	int32_t out = 1;

	x = a;
	y = p;
	while (x) {
		while ((x & 1) == 0) {
			x /= 2;
			if ( (y & 7) == 3 || (y & 7) == 5 )
				out = -out;
		}

		tmp = x;
		x = y;
		y = tmp;

		if ( (x & 3) == 3 && (y & 3) == 3 )
			out = -out;

		x %= y;
	}
	if (y == 1)
		return out;
	return 0;
}   // mpz_legendre_ui

/*----------------------------------------------------------------------*/
static uint32_t init_one_fb_tiny(uint32_t fb_size, mpz_ptr n, tiny_fb *factor_base) 
{
	uint32_t i, j;
	uint32_t prime;
	mpz_t tmp;

	i = MIN_FB_OFFSET;
	prime = 2;
	factor_base[i].prime = prime;
	mpz_init(tmp);

	for (i++, j = 1; i < fb_size && j < NUM_PRIMES_TINY; j++) {

		tiny_fb *fbptr = factor_base + i;
		uint32_t nmodp;

		prime += prime_delta[j];
		nmodp = mpz_fdiv_ui(n, prime);
		mpz_set_ui(tmp, prime);
		if (nmodp == 0) {
			fbptr->prime = (uint16_t)prime;
			fbptr->logprime = logprime_list[j];
			i++;
		}
		else if (mpz_legendre_ui(nmodp, prime) == 1) {
			fbptr->prime = (uint16_t)prime;
			fbptr->logprime = logprime_list[j];
			fbptr->modsqrt = (uint16_t)sqrt(nmodp) % prime;
			i++;
		}
	}

	return i;
}   // init_one_fb_tiny

/*----------------------------------------------------------------------*/
static void init_fb_tiny(tiny_qs_params *params) 
{
	uint32_t i, j;
	tiny_fb next_fb_array[MAX_FB_SIZE_TINY];
	uint32_t next_fb_size;
	double score, best_score;
	mpz_t n2;
	uint16_t mult_list[] = {1, 3, 5, 7, 11, 13, 15, 17, 19, 21, 23,
                            29, 31, 33, 35, 37, 39, 41, 43, 47, 51,
                            53, 55, 57, 59, 61, 65, 67, 69, 71, 73};

	best_score = 1000.0;
	score = 0;

    mpz_init(n2);
	for (i = 0; i < sizeof(mult_list) / sizeof(uint16_t); i++) {

		uint32_t curr_mult = mult_list[i];

		mpz_mul_ui(n2, params->n, curr_mult);

		next_fb_size = init_one_fb_tiny(MAX_FB_SIZE_TINY, n2, next_fb_array);
		
		if ((params->n->_mp_d[0] & 7) == 1)
			score = 0.5 * log((double)curr_mult) - 2 * M_LN2;
		else if ((params->n->_mp_d[0] & 7) == 5)
			score = 0.5 * log((double)curr_mult) - M_LN2;
		else
			score = 0.5 * log((double)curr_mult) - 0.5 * M_LN2;

		for (j = MIN_FB_OFFSET + 1; j < next_fb_size; j++) {
			tiny_fb *fbptr = next_fb_array + j;
			uint32_t prime = fbptr->prime;

			if (prime <= 73 && curr_mult % prime == 0)
				score -= log((double)prime) / (prime - 1);
			else
				score -= 2.0 * log((double)prime) / (prime - 1);
		}

		if (score < best_score) {
			memcpy(params->factor_base, next_fb_array, sizeof(params->factor_base));
			best_score = score;
			params->fb_size = next_fb_size;
			params->multiplier = curr_mult;
		}
	}

	mpz_mul_ui(params->n, params->n, params->multiplier);
	mpz_clear(n2);
}   // init_fb_tiny

/*----------------------------------------------------------------------*/
static void fill_sieve_block_tiny(tiny_qs_params *params) {

	uint32_t i;
	uint32_t fb_size = params->fb_size;
	uint8_t *sieve_block = params->sieve_block;
	tiny_fb *factor_base = params->factor_base;

	for (i = MIN_FB_OFFSET_TO_SIEVE_TINY; i < fb_size; i++) {
		tiny_fb *fbptr = factor_base + i;
		uint32_t prime = fbptr->prime;
		uint8_t logprime = fbptr->logprime;
		uint32_t root1 = fbptr->next_loc1;
		uint32_t root2 = fbptr->next_loc2;

		if (fbptr->root1 == (uint16_t)(-1))
			continue;

		while (root2 < SIEVE_BLOCK_SIZE_TINY) {
			sieve_block[root1] -= logprime;
			sieve_block[root2] -= logprime;
			root1 += prime;
			root2 += prime;
		}

		if (root1 < SIEVE_BLOCK_SIZE_TINY) {
			sieve_block[root1] -= logprime;
			root1 += prime;
			fbptr->next_loc1 = root2 - SIEVE_BLOCK_SIZE_TINY;
			fbptr->next_loc2 = root1 - SIEVE_BLOCK_SIZE_TINY;
		}
		else {
			fbptr->next_loc1 = root1 - SIEVE_BLOCK_SIZE_TINY;
			fbptr->next_loc2 = root2 - SIEVE_BLOCK_SIZE_TINY;
		}
	}
}   // fill_sieve_block_tiny

/*----------------------------------------------------------------------*/
static void check_sieve_val_tiny(tiny_qs_params *params, mpz_srcptr a, mpz_srcptr b, mpz_srcptr c, 
				uint32_t sieve_offset, uint32_t sign_of_index, uint32_t bits) 
{
	uint32_t i, j;
	uint32_t fb_size = params->fb_size;
	tiny_fb *factor_base = params->factor_base;
	uint32_t num_factors = 0;
	mpz_t res, tmp;
	uint32_t cutoff2;
	tiny_relation *relation = params->relation_list + params->num_relations;
	uint8_t *fb_offsets = relation->fb_offsets;

    mpz_inits(res, tmp, NULL);
	mpz_mul_ui(res, a, sieve_offset);
	if (sign_of_index == POSITIVE)
		mpz_add(res, res, b);
	else
		mpz_sub(res, res, b);

	mpz_mul_ui(res, res, sieve_offset);
	if (mpz_cmp(res, c) >= 0) {
		mpz_sub(res, res, c);
	}
	else {
		mpz_sub(res, c, res);
		fb_offsets[num_factors++] = 0;
	}

	cutoff2 = LOGPRIME_SCALE_TINY * mpz_sizeinbase(res, 2) - params->error_bits;

	i = mpz_scan1(res, 0);
	mpz_fdiv_q_2exp(res, res, i);
	for (bits += i * LOGPRIME_SCALE_TINY; i; i--)
		fb_offsets[num_factors++] = MIN_FB_OFFSET;

	for (i = MIN_FB_OFFSET + 1; i < MIN_FB_OFFSET_TO_SIEVE_TINY; i++) {
		tiny_fb *fbptr = factor_base + i;
		uint16_t prime = fbptr->prime;
		uint8_t logprime = fbptr->logprime;
		uint16_t root1 = fbptr->root1;
		uint16_t root2 = fbptr->root2;

		j = sieve_offset % prime;

		if (root1 == (uint16_t)(-1)) {
			if (mpz_mod_ui(tmp, res, prime) == 0) {
				do {
					if (num_factors >= MAX_FACTORS_TINY)
						return;
					bits += logprime;
					fb_offsets[num_factors++] = i;
					mpz_fdiv_r_ui(res, res, prime);
					j = mpz_fdiv_ui(res, prime);
				} while (j == 0);
			}
			continue;
		}

		if (sign_of_index == NEGATIVE) {
			root2 = prime - root2;
			if (root1)
				root1 = prime - root1;
		}

		if (j == root1 || j == root2) {
			do {
				if (num_factors >= MAX_FACTORS_TINY)
					return;
				bits += logprime;
				fb_offsets[num_factors++] = i;
				mpz_fdiv_r_ui(res, res, prime);
				j = mpz_fdiv_ui(res, prime);
			} while (j == 0);
		}
	}

	if (bits < cutoff2)
		return;

	for (; i < fb_size; i++) {
		tiny_fb *fbptr = factor_base + i;
		uint16_t prime = fbptr->prime;
		uint16_t root1 = fbptr->root1;
		uint16_t root2 = fbptr->root2;

		j = sieve_offset % prime;

		if (root1 == (uint16_t)(-1)) {
			if (mpz_fdiv_ui(res, prime) == 0) {
				do {
					if (num_factors >= MAX_FACTORS_TINY)
						return;
					fb_offsets[num_factors++] = i;
					mpz_fdiv_r_ui(res, res, prime);
					j = mpz_fdiv_ui(res, prime);
				} while (j == 0);
			}
			continue;
		}

		if (sign_of_index == NEGATIVE) {
			root2 = prime - root2;
			if (root1)
				root1 = prime - root1;
		}

		if (j == root1 || j == root2) {
			do {
				if (num_factors >= MAX_FACTORS_TINY)
					return;
				fb_offsets[num_factors++] = i;
				mpz_fdiv_r_ui(res, res, (uint32_t)prime);
				j = mpz_fdiv_ui(res, prime);
			} while (j == 0);
		}
	}

	if (res->_mp_alloc > 1)
		return;

	if (sign_of_index == NEGATIVE)
		sieve_offset |= 0x80000000;

	relation->sieve_offset = sieve_offset;
	relation->num_factors = num_factors;
	relation->poly_num = params->poly_num - 1;

	if (res->_mp_d[0] == 1) {
		relation->large_prime = 1;
		params->num_relations++;
		return;
	}

	if (res->_mp_d[0] < params->large_prime_max) {
		uint32_t table_idx = LARGE_PRIME_HASH(res->_mp_d[0]);
		uint16_t hash_entry = params->partial_list[table_idx].large_prime;

		relation->large_prime = res->_mp_d[0];
		if (hash_entry == res->_mp_d[0])
			params->num_relations++;
		else if (hash_entry == 0)
			memcpy(params->partial_list + table_idx, relation,
					sizeof(tiny_relation));
	}
	mpz_clears(res, tmp, NULL);
}   // check_sieve_val_tiny

/*----------------------------------------------------------------------*/
#define PACKED_SIEVE_MASK ((uint64_t)0x80808080 << 32 | 0x80808080)

static void sieve_next_poly_tiny(tiny_qs_params *params) 
{
	uint32_t i, j, k;
	uint32_t fb_size = params->fb_size;
	uint8_t *sieve_block = params->sieve_block;
	uint64_t *packed_sieve_block = (uint64_t *)params->sieve_block;
	uint32_t block_start;
	tiny_fb *factor_base = params->factor_base;
	mpz_t a, b, c, tmp1, tmp2;
	mpz_t n;
	uint32_t cutoff1;
	uint8_t poly_num = params->poly_num;
	uint8_t num_sieve_blocks = params->num_sieve_blocks;
	uint32_t target_relations = params->fb_size + NUM_EXTRA_RELATIONS_TINY;

    mpz_inits(a, b, c, tmp1, tmp2, NULL);
    mpz_init_set(n, params->n);
    
	i = params->curr_poly_offset;
	do {
		mpz_add_ui(a, params->target_a, i);
		mpz_nextprime(tmp1, a);
		mpz_sub(tmp2, tmp1, a);
		//gmp_printf("tmp2: %Zd\n", tmp2);
		i += tmp2->_mp_d[0];
	} while(mpz_legendre(n, tmp1) != 1);

	//	mp_modsqrt2(n, &tmp, &b, &params->seed1, &params->seed2);           // b*b ≡ n mod tmp
    mpz_sqrtrem(b, n, tmp1);
	mpz_mul(a, tmp1, tmp1);
        //gmp_printf("tmp1: %Zd\na: %Zd\n", tmp1, a);

	params->curr_poly_offset = i;
	params->poly_offset[poly_num] = i;
	mpz_init_set(params->poly_b[poly_num], b);
	params->poly_num++;

	mpz_mul(tmp1, b, b);
	mpz_sub(tmp1, n, tmp1);		
	mpz_fdiv_q(c, tmp1, a);

	for (i = MIN_FB_OFFSET + 1; i < fb_size; i++) {
		tiny_fb *fbptr = factor_base + i;
		uint32_t prime = fbptr->prime;
		uint32_t modsqrt = fbptr->modsqrt;
		uint32_t amodp = mpz_fdiv_ui(a, prime);
		uint32_t bmodp = prime - mpz_fdiv_ui(b, prime);
		uint32_t root1, root2;

		if (amodp == 0 || 
		    (prime < 64 && params->multiplier % prime == 0)) {
			fbptr->root1 = (uint16_t)(-1);
			fbptr->root2 = (uint16_t)(-1);
			continue;
		}

		amodp = mp_modinv_1(amodp, prime);
		root1 = mp_modmul_1(amodp, bmodp + modsqrt, prime);
		root2 = mp_modmul_1(amodp, bmodp + prime - modsqrt, prime);

		if (root1 < root2) {
			fbptr->root1 = root1;
			fbptr->root2 = root2;
			fbptr->next_loc1 = root1;
			fbptr->next_loc2 = root2;
		}
		else {
			fbptr->root1 = root2;
			fbptr->root2 = root1;
			fbptr->next_loc1 = root2;
			fbptr->next_loc2 = root1;
		}
	}

	cutoff1 = LOGPRIME_SCALE_TINY * mpz_sizeinbase(c, 2) - params->error_bits - SMALL_PRIME_FUDGE_TINY;
	mpz_add(b, b, b);
	
	for (i = block_start = 0; i < num_sieve_blocks; i++) {

		memset(sieve_block, (int32_t)(cutoff1 - 1), (size_t)SIEVE_BLOCK_SIZE_TINY);
		fill_sieve_block_tiny(params);

		for (j = 0; j < SIEVE_BLOCK_SIZE_TINY / 8; j += 4) {
			uint64_t accum = packed_sieve_block[j] |
					packed_sieve_block[j+1] |
					packed_sieve_block[j+2] |
					packed_sieve_block[j+3];

			if ((accum & PACKED_SIEVE_MASK) == (uint64_t)(0))
				continue;

			for (k = 0; k < 32; k++) {
				uint32_t bits = sieve_block[8 * j + k];
				if (bits <= cutoff1)
					continue;
				if (params->num_relations >= target_relations)
					return;
				check_sieve_val_tiny(params, a, b, c,
					block_start + 8 * j + k, 
					POSITIVE, cutoff1 + 257 - bits);
			}
		}
		block_start += SIEVE_BLOCK_SIZE_TINY;
	}

	for (i = MIN_FB_OFFSET + 1; i < fb_size; i++) {
		tiny_fb *fbptr = factor_base + i;
		uint32_t prime = fbptr->prime;

		fbptr->next_loc1 = prime - fbptr->root2;
		fbptr->next_loc2 = prime - fbptr->root1;
	}
		
	for (i = block_start = 0; i < num_sieve_blocks; i++) {

		memset(sieve_block, (int32_t)(cutoff1 - 1), 
					(size_t)SIEVE_BLOCK_SIZE_TINY);
		fill_sieve_block_tiny(params);

		for (j = 0; j < SIEVE_BLOCK_SIZE_TINY / 8; j += 4) {
			uint64_t accum = packed_sieve_block[j] |
					packed_sieve_block[j+1] |
					packed_sieve_block[j+2] |
					packed_sieve_block[j+3];

			if ((accum & PACKED_SIEVE_MASK) == (uint64_t)(0))
				continue;

			for (k = 0; k < 32; k++) {
				uint32_t bits = sieve_block[8 * j + k];
				if (bits <= cutoff1)
					continue;
				if (params->num_relations >= target_relations)
					return;
				check_sieve_val_tiny(params, a, b, c,
					block_start + 8 * j + k, 
					NEGATIVE, cutoff1 + 257 - bits);
			}
		}
		block_start += SIEVE_BLOCK_SIZE_TINY;
	}
	mpz_clears(a, b, c, tmp1, tmp2, n, NULL);
}   // sieve_next_poly_tiny

/*----------------------------------------------------------------------*/
static void solve_linear_system_tiny(tiny_qs_params *params)
{
	uint32_t i, j, k, start_row;
	uint32_t nrows = params->fb_size;
	uint32_t ncols = params->num_relations;
	uint8_t rowperm[MAX_FB_SIZE_TINY];
	uint8_t pivot[MAX_FB_SIZE_TINY];
	uint8_t row = 0;

	memset(params->matrix, 0, sizeof(params->matrix));

	for (i = 0; i < ncols; i++) {
		tiny_relation *r = params->relation_list + i;
		for (j = 0; j < r->num_factors; j++) {
			row = r->fb_offsets[j];
			params->matrix[row][i / 64] ^= bitmask[i & 63];
		}
		if (r->large_prime > 1) {
			r = params->partial_list + 
				LARGE_PRIME_HASH(r->large_prime);
			for (j = 0; j < r->num_factors; j++) {
				row = r->fb_offsets[j];
				params->matrix[row][i / 64] ^= bitmask[i & 63];
			}
		}
	}
	for (i = 0; i < nrows; i++)
		rowperm[i] = i;
	for (i = 0; i < ncols; i++)
		params->null_vectors[i] = (uint16_t)get_rand(&params->seed1, &params->seed2);

	for (i = start_row = 0; start_row < nrows && i < ncols; i++) {
		
		for (j = start_row; j < nrows; j++) {
			row = rowperm[j];
			if (params->matrix[row][i / 64] & bitmask[i & 63])
				break;
		}
		if (j == nrows)
			continue;

		rowperm[j] = rowperm[start_row];
		rowperm[start_row] = row;
		pivot[start_row++] = i;

		for (j++; j < nrows; j++) {
			uint8_t row2 = rowperm[j];
			if (params->matrix[row2][i / 64] & bitmask[i & 63]) {
				for (k = i / 64; k < (ncols + 63) / 64; k++) {
					params->matrix[row2][k] ^=
					params->matrix[row][k];
				}
			}
		}
	}

	for (i = start_row - 1; (int32_t)i >= 0; i--) {
		uint16_t accum;
		row = rowperm[i];

		for (j = pivot[i] + 1, accum = 0; j < ncols; j++) {
			if (params->matrix[row][j / 64] & bitmask[j & 63])
				accum ^= params->null_vectors[j];
		}
		params->null_vectors[pivot[i]] = accum;
	}
}   // solve_linear_system_tiny

/*----------------------------------------------------------------------*/
static uint32_t find_factors_tiny(tiny_qs_params *params, mpz_ptr factor1, mpz_ptr factor2)
{
	mpz_t x, y, t0, t1;
	mpz_t n;
	uint16_t i, j, k;
	uint16_t mask;
	uint16_t fb_counts[MAX_FB_SIZE_TINY];

    mpz_inits(x, y, t0, t1, NULL); 
    mpz_init_set(n, params->n);
	
	for (mask = 1; mask; mask <<= 1) {

		memset(fb_counts, 0, sizeof(fb_counts));
		mpz_set_ui(x, 1);
		mpz_set_ui(y, 1);

		for (i = 0; i < params->num_relations; i++) {

			if (!(params->null_vectors[i] & mask))
				continue;

			for (j = 0; j < 2; j++) {
				tiny_relation *r = params->relation_list + i;
				uint32_t poly_num;

				if (j == 1) {
					if (r->large_prime > 1) {
						r = params->partial_list +
					    		LARGE_PRIME_HASH(
								r->large_prime);
						mpz_mul_ui(t0, y, r->large_prime);
						mpz_mod(y, t0, n);
					}
					else {
						break;
					}
				}
				poly_num = r->poly_num;
	
				mpz_add_ui(t0, params->target_a, 
					params->poly_offset[poly_num]);
				mpz_mul(y, t0, y);
				mpz_mod(y, y, n);
	
				for (k = 0; k < r->num_factors; k++)
					fb_counts[r->fb_offsets[k]]++;
	
				mpz_mul(t1, t0, t0);
				mpz_mul_ui(t1, t1, r->sieve_offset & 0x7fffffff);
				if (r->sieve_offset >> 31)
					mpz_sub(t1, t1, params->poly_b[poly_num]);
				else
					mpz_add(t1, t1, params->poly_b[poly_num]);
				mpz_mul(x, x, t1);
				mpz_mod(x, x, n);
			}
		}

		for (i = MIN_FB_OFFSET; i < params->fb_size; i++) {
			uint16_t mask2 = 0x8000;
			uint16_t exponent = fb_counts[i] / 2;
			uint32_t prime = params->factor_base[i].prime;

			if (exponent == 0)
				continue;

			mpz_clear(t0);
			t0->_mp_alloc = 1;
			t0->_mp_d[0] = prime;
			mpz_set(t1, t0);

			while (!(exponent & mask2))
				mask2 >>= 1;

			for (mask2 >>= 1; mask2; mask2 >>= 1) {
				mpz_mul(t0, t0, t0);
				mpz_mod(t0, t0, n);
				if (exponent & mask2) {
					mpz_mul(t0, t0, t1);
					mpz_mod(t0, t0, n);
				}
			}
			mpz_mul(y, t0, y);
			mpz_mod(y, y, n);
		}

		for (i = 0; i < 2; i++) {
			if (i == 0)
				mpz_add(t0, x, y);
			else if (mpz_cmp(x, y) > 0)
				mpz_sub(t0, x, y);
			else
				mpz_sub(t0, y, x);

			mpz_gcd(t1, t0, n);
			if (!mpz_cmp_ui(t1, 1) == 0 && mpz_cmp(t1, n) == 0) {
				if (params->multiplier > 1) {
					mpz_t m;
					mpz_init(m);
					m->_mp_alloc = 1; 
					m->_mp_d[0] = params->multiplier;
					mpz_gcd(t0, t1, m);
					mpz_fdiv_r_ui(t1, t1, t0->_mp_d[0]);
				}
				if (!mpz_cmp_ui(t1, 1) == 0) {
					mpz_fdiv_r_ui(t0, n, params->multiplier);
					mpz_fdiv_q(factor1, t0, t1);
					mpz_set(factor2, t1);
					return 1;           // found both factors
				}
			}
		}
	}
    mpz_clears(x, y, t0, t1, n, NULL);
    
	return 0;
}   // find_factors_tiny

/*----------------------------------------------------------------------*/
uint32_t tinyqs(mpz_srcptr n, mpz_ptr factor1, mpz_ptr factor2) 
{
	tiny_qs_params *params;
	uint32_t i, bits;
	uint32_t fb_size, status = 0;
	uint16_t bound;
	uint16_t large_prime_mult;
	mpz_t tmp1, tmp2;

	params = (tiny_qs_params *)malloc(sizeof(tiny_qs_params));

    mpz_inits(tmp1, tmp2, NULL);
	mpz_init_set(params->n, n);
	mpz_init(params->target_a);
	params->num_relations = 0;
	params->seed1 = 287643287;
	params->seed2 = 833277363;
	params->curr_poly_offset = 0;
	params->poly_num = 0;

	init_fb_tiny(params);
	bits = mpz_sizeinbase(params->n, 2);

	if (bits < 70) {
		fb_size = MIN(params->fb_size, 40);
		params->num_sieve_blocks = 4;
		large_prime_mult = 1;
	}
	else if (bits < 75) {
		fb_size = MIN(params->fb_size, 50);
		params->num_sieve_blocks = 4;
		large_prime_mult = 5;
	}
	else if (bits < 80) {
		fb_size = MIN(params->fb_size, 60);
		params->num_sieve_blocks = 4;
		large_prime_mult = 10;
	}
	else if (bits < 85) {
		fb_size = MIN(params->fb_size, 70);
		params->num_sieve_blocks = 8;
		large_prime_mult = 10;
	}
	else {
		fb_size = MIN(params->fb_size, 85);
		params->num_sieve_blocks = 8;
		large_prime_mult = 15;
	}
	params->fb_size = fb_size;

	mpz_add(tmp1, params->n, params->n);
	mpz_sqrt(tmp2, tmp1);
	mpz_fdiv_r_ui(tmp2, tmp2, (uint32_t)(params->num_sieve_blocks * SIEVE_BLOCK_SIZE_TINY));
	mpz_sqrt(params->target_a, tmp2);

	bound = params->factor_base[params->fb_size - 1].prime;
	bound *= large_prime_mult;
	params->large_prime_max = bound;
	params->error_bits = (uint32_t)(LOGPRIME_SCALE_TINY * (log((double)bound) / M_LN2 + 1));
	for (i = 0; i < (1 << LOG2_PARTIAL_TABLE_SIZE); i++)
		params->partial_list[i].large_prime = 0;

	while (params->poly_num < MAX_POLY_TINY &&
	       params->num_relations < fb_size + NUM_EXTRA_RELATIONS_TINY) {
		sieve_next_poly_tiny(params);
	}

	if (params->num_relations >= fb_size + NUM_EXTRA_RELATIONS_TINY) {
		solve_linear_system_tiny(params);
		status = find_factors_tiny(params, factor1, factor2);
	}

	mpz_clears(tmp1, tmp2, NULL);
	free(params);

	return status;
}   // tinyqs

/*----------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    mpz_t n, p, q;
    
    mpz_inits(n, p, q, NULL);
    if (argc == 2)
    {
        mpz_set_str(n, argv[1], 10);
        if (tinyqs(n, p, q) != 0) 
        {
            gmp_printf("Found factors: %Zd, %Zd\n", p, q);
        }                
    }
    return 0;
}
