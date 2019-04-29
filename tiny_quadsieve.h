
#include <stdint.h>
#include <gmp.h>

#define POSITIVE 0
#define NEGATIVE 1

#define LOGPRIME_SCALE_TINY 2

static const uint8_t logprime_list[] = 
{
    2,  3,  5,  6,  7,  7,  8,  8,  9,  10,
    10, 10, 11, 11, 11, 11, 12, 12, 12, 12,
    12, 13, 13, 13, 13, 13, 13, 13, 14, 14,
    14, 14, 14, 14, 14, 14, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 18, 18, 18, 18, 18, 18, 18, 18,
    18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
    18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
    18, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 21,
};

#define BIT(x) ((uint64_t)(1) << (x))
#define MIN(a,b) ((a) < (b)? (a) : (b))
#define MAX(a,b) ((a) > (b)? (a) : (b))

static const uint64_t bitmask[64] = 
{
	BIT( 0), BIT( 1), BIT( 2), BIT( 3), BIT( 4), BIT( 5), BIT( 6), BIT( 7),
	BIT( 8), BIT( 9), BIT(10), BIT(11), BIT(12), BIT(13), BIT(14), BIT(15),
	BIT(16), BIT(17), BIT(18), BIT(19), BIT(20), BIT(21), BIT(22), BIT(23),
	BIT(24), BIT(25), BIT(26), BIT(27), BIT(28), BIT(29), BIT(30), BIT(31),
	BIT(32), BIT(33), BIT(34), BIT(35), BIT(36), BIT(37), BIT(38), BIT(39),
	BIT(40), BIT(41), BIT(42), BIT(43), BIT(44), BIT(45), BIT(46), BIT(47),
	BIT(48), BIT(49), BIT(50), BIT(51), BIT(52), BIT(53), BIT(54), BIT(55),
	BIT(56), BIT(57), BIT(58), BIT(59), BIT(60), BIT(61), BIT(62), BIT(63),
};

#define MIN_FB_OFFSET 1
#define NUM_PRIMES_TINY (sizeof(logprime_list) / sizeof(uint8_t))
#define MAX_FB_SIZE_TINY 85
#define NUM_EXTRA_RELATIONS_TINY 16
#define MAX_RELATIONS_TINY (MAX_FB_SIZE_TINY + NUM_EXTRA_RELATIONS_TINY)
#define MIN_FB_OFFSET_TO_SIEVE_TINY 7
#define SMALL_PRIME_FUDGE_TINY (LOGPRIME_SCALE_TINY * 10)
#define MAX_POLY_TINY 20
#define SIEVE_BLOCK_SIZE_TINY 32768
#define MAX_FACTORS_TINY 20
#define LOG2_PARTIAL_TABLE_SIZE 10
#define LARGE_PRIME_HASH(x) (((uint32_t)(x) * ((uint32_t)40499 * 65543)) >> \
				(32 - LOG2_PARTIAL_TABLE_SIZE))

#define PRECOMPUTED_PRIME_BOUND 100000
#define PRECOMPUTED_NUM_PRIMES 9592
extern const uint8_t prime_delta[PRECOMPUTED_NUM_PRIMES];

typedef struct 
{
	uint32_t sieve_offset;
	uint16_t large_prime;
	uint8_t poly_num;
	uint8_t num_factors;
	uint8_t fb_offsets[MAX_FACTORS_TINY];
} tiny_relation;

typedef struct 
{
	uint16_t prime;
	uint8_t logprime;
	uint16_t modsqrt;
	uint16_t root1;
	uint16_t root2;
	uint32_t next_loc1;
	uint32_t next_loc2;
} tiny_fb;
	
typedef struct 
{
	mpz_t n;
	uint32_t multiplier;

	mpz_t target_a;
	uint8_t poly_num;
	uint32_t curr_poly_offset;
	uint32_t poly_offset[MAX_POLY_TINY];
	mpz_t poly_b[MAX_POLY_TINY];

	double align_me;
	uint8_t sieve_block[SIEVE_BLOCK_SIZE_TINY];
	uint8_t num_sieve_blocks;

	uint32_t fb_size;
	tiny_fb factor_base[MAX_FB_SIZE_TINY];

	uint32_t num_relations;
	uint16_t large_prime_max;
	uint32_t error_bits;
	tiny_relation relation_list[MAX_RELATIONS_TINY];
	tiny_relation partial_list[1 << LOG2_PARTIAL_TABLE_SIZE];

	uint64_t matrix[MAX_FB_SIZE_TINY][(MAX_RELATIONS_TINY+63) / 64];
	uint16_t null_vectors[MAX_RELATIONS_TINY];

	uint32_t seed1;
	uint32_t seed2;
} tiny_qs_params;

static inline uint32_t mp_modinv_1(uint32_t a, uint32_t p) 
{
	uint32_t ps1, ps2, dividend, divisor, rem, q, t;
	uint32_t parity;

	q = 1; rem = a; dividend = p; divisor = a;
	ps1 = 1; ps2 = 0; parity = 0;

	while (divisor > 1) {
		rem = dividend - divisor;
		t = rem - divisor;
		if (rem >= divisor) { q += ps1; rem = t; t -= divisor;
		if (rem >= divisor) { q += ps1; rem = t; t -= divisor;
		if (rem >= divisor) { q += ps1; rem = t; t -= divisor;
		if (rem >= divisor) { q += ps1; rem = t; t -= divisor;
		if (rem >= divisor) { q += ps1; rem = t; t -= divisor;
		if (rem >= divisor) { q += ps1; rem = t; t -= divisor;
		if (rem >= divisor) { q += ps1; rem = t; t -= divisor;
		if (rem >= divisor) { q += ps1; rem = t;
		if (rem >= divisor) {
			q = dividend / divisor;
			rem = dividend % divisor;
			q *= ps1;
		} } } } } } } } }

		q += ps2;
		parity = ~parity;
		dividend = divisor;
		divisor = rem;
		ps2 = ps1;
		ps1 = q;
	}
	
	if (parity == 0)
		return ps1;
	else
		return p - ps1;
}

static inline uint32_t mp_modmul_1(uint32_t a, uint32_t b, uint32_t n) 
{
	uint64_t acc = (uint64_t)a * (uint64_t)b;
	return (uint32_t)(acc % n);
}

static inline uint32_t get_rand(uint32_t *rand_seed, uint32_t *rand_carry) 
{  
	/* A multiply-with-carry generator by George Marsaglia.
	   The period is about 2^63. */

	#define RAND_MULT 2131995753

	uint64_t temp;

	temp = (uint64_t)(*rand_seed) * 
		       (uint64_t)RAND_MULT + 
		       (uint64_t)(*rand_carry);
	*rand_seed = (uint32_t)temp;
	*rand_carry = (uint32_t)(temp >> 32);
	return (uint32_t)temp;
}

