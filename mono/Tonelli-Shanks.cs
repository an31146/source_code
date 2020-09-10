using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Numerics;
 
namespace TonelliShanks {
    class Solution {
        private readonly BigInteger root1, root2;
        private readonly bool exists;
 
        public Solution(BigInteger root1, BigInteger root2, bool exists) {
            this.root1 = root1;
            this.root2 = root2;
            this.exists = exists;
        }
 
        public BigInteger Root1() {
            return root1;
        }
 
        public BigInteger Root2() {
            return root2;
        }
 
        public bool Exists() {
            return exists;
        }
    }
 
    class Program {
		static uint[] primes2 = new uint[] {
			   2,  13,  23,  29,  47,  53,  59,  83,  89, 101, 107, 127, 131, 139, 163, 173, 179, 191, 193, 197, 199, 211, 
			 229, 233, 257, 263, 269, 277, 281, 283, 307, 313, 317, 331, 337, 347, 349, 353, 373, 383, 389, 401, 419, 421, 
			 431, 433, 449, 463, 487, 491, 499, 509, 523, 569, 571, 577, 587, 593, 613, 619, 631, 653, 659, 661, 673, 677, 
			 683, 701, 733, 739, 743, 751, 761, 769, 787, 809, 811, 823, 829, 853, 859, 877, 881, 887, 911, 929, 953, 967, 
			 983, 991,1013,1021,1031,1049,1051,1091,1103,1151,1153,1171,1181,1201,1223,1229,1231,1249,1283,1301,1361,1367,
			1381,1399,1427,1433,1447,1451,1459,1471,1483,1493,1543,1549,1559,1567,1571,1607,1609,1621,1627,1637,1657,1667,
			1699,1721,1723,1741,1759,1777,1783,1787,1831,1861,1877,1879,1889,1901,1907,1931,1951,1973,1987,1997,1999,2003,
			2027,2029,2039,2063,2083,2111,2129,2131,2141,2143,2179,2213,2221,2237,2243,2269,2273,2281,2293,2297,2309,2311,
			2333,2347,2351,2399,2411,2417,2423,2437,2441,2447,2473,2521,2543,2549,2551,2609,2617,2633,2663,2693,2707,2711,
			2713,2729,2731,2741,2749,2753,2767,2803,2833,2837,2843,2851,2857,2861,2887,2909,2927,2939,2963,2999,3011,3019,
			3023,3037,3049,3067,3079,3083,3089,3109,3137,3167,3229,3251,3253,3271,3301,3307,3319,3323,3343,3359,3371
    	};

		static uint[] primes = new uint[] {
		2,      5,      7,      19,     23,     31,     37,     41,     43,     53,     59,     67,     73,     79,     101,    103,    127,    131,    149,    179,    191,    199,    211,    223,   
		233,    251,    257,    269,    271,    283,    293,    311,    317,    359,    421,    431,    433,    457,    463,    479,    491,    523,    541,    563,    569,    571,    577,    599,
		607,    619,    631,    643,    647,    653,    659,    677,    683,    739,    751,    787,    797,    809,    811,    821,    829,    863,    877,    881,    887,    929,    937,    941,
		947,    953,    971,    977,    991,    997,    1009,   1019,   1021,   1031,   1051,   1061,   1069,   1093,   1103,   1109,   1129,   1153,   1201,   1217,   1223,   1229,   1231,   1237,
		1249,   1259,   1279,   1283,   1289,   1291,   1303,   1307,   1319,   1327,   1361,   1373,   1399,   1427,   1439,   1447,   1451,   1453,   1459,   1487,   1499,   1531,   1553,   1559,
		1571,   1583,   1607,   1609,   1621,   1627,   1663,   1667,   1669,   1697,   1699,   1709,   1733,   1759,   1777,   1789,   1801,   1823,   1831,   1847,   1867,   1871,   1873,   1879,
		1889,   1901,   1933,   1949,   1951,   1973,   1987,   1999,   2003,   2017,   2053,   2063,   2081,   2111,   2113,   2131,   2141,   2143,   2153,   2207,   2213,   2221,   2237,   2267,
		2273,   2281,   2287,   2309,   2333,   2339,   2341,   2351,   2357,   2377,   2417,   2423,   2473,   2477,   2521,   2539,   2543,   2549,   2551,   2557,   2609,   2617,   2621,   2647,
		2659,   2663,   2687,   2689,   2707,   2713,   2719,   2731,   2741,   2749,   2767,   2789,   2797,   2803,   2833,   2837,   2843,   2857,   2861,   2897,   2909,   2927,   2939,   2957,
		2969,   2999,   3001,   3011,   3019,   3023,   3037,   3049,   3061,   3079,   3089,   3119,   3137,   3181,   3203,   3209,   3217,   3221,   3257,   3299,   3313,   3319,   3331,   3343,
		3347,   3359,   3371,   3373,   3391,   3413,   3449,   3457,   3461,   3467,   3469,   3491,   3499,   3517,   3529,   3541,   3547,   3559,   3581,   3593,   3617,   3623,   3631,   3637,
		3659,   3673,   3701,   3709,   3719,   3733,   3739,   3767,   3793,   3797,   3803,   3821,   3833,   3853,   3863,   3877,   3881,   3889,   3911,   3917,   3919,   3923,   3943,   3967,
		3989,   4001,   4007,   4021,   4027,   4049,   4051,   4079,   4091,   4099,   4133,   4139,   4153,   4157,   4159,   4177,   4201,   4211,   4217,   4229,   4231,   4253,   4271,   4273,
		4289,   4337,   4357,   4391,   4409,   4421,   4441,   4447,   4451,   4463,   4481,   4493,   4507,   4513,   4549,   4567,   4583,   4597,   4621,   4643,   4651,   4663,   4679,   4691,
		4721,   4729,   4733,   4783,   4813,   4877,   4889,   4909,   4919,   4937,   4951,   4967,   4969,   4973,   4993,   5009,   5023,   5039,   5059,   5077,   5081,   5099,   5119,   5147,
		5153,   5167,   5179,   5189,   5197,   5227,   5231,   5233,   5237,   5261,   5273,   5279,   5297,   5303,   5309,   5323,   5347,   5393,   5399,   5407,   5413,   5471,   5479,   5483,
		5501,   5519,   5527,   5557,   5573,   5591,   5639,   5641,   5647,   5651,   5653,   5659,   5689,   5693,   5711,   5749,   5783,   5807,   5813,   5827,   5839,   5857,   5867,   5881,
		5923,   5981,   5987,   6029,   6037,   6047,   6101,   6113,   6131,   6143,   6151,   6163,   6197,   6203,   6211,   6217,   6221,   6257,   6271,   6299,   6317,   6329,   6337,   6343,
		6361,   6389,   6397,   6427,   6449,   6451,   6469,   6491,   6521,   6547,   6551,   6563,   6569,   6577,   6599,   6619,   6673,   6679,   6689,   6701,   6709,   6763,   6781,   6791,
		6793,   6829,   6833,   6857,   6863,   6883,   6899,   6917,   6949,   6959,   6977,   6997,   7013,   7019,   7027,   7039,   7057,   7069,   7079,   7109,   7121,   7127,   7151,   7207,
		7219,   7243,   7247,   7283,   7297,   7307,   7309,   7331,   7349,   7351,   7411,   7433,   7457,   7477,   7499,   7507,   7547,   7549,   7559,   7561,   7573,   7591,   7603,   7607,
		7639,   7649,   7669,   7681,   7691,   7717,   7723,   7757,   7759,   7793,   7817,   7841,   7867,   7879,   7883,   7919,   7933,   7949,   7963,   8011,   8039,   8053,   8069,   8089,
		8123,   8167,   8179,   8191,   8209,   8219,   8221,   8233,   8237,   8243,   8263,   8269,   8293,   8297,   8311,   8387,   8389,   8431,   8443,   8447,   8513,   8521,   8537,   8543,
		8573,   8623,   8641,   8681,   8699,   8731,   8741,   8753,   8807,   8819,   8821,   8849,   8863,   8887,   8893,   8933,   8941,   8951,   8963,   8969,   8971,   9013,   9041,   9067,
		9109,   9127,   9133,   9157,   9161,   9173,   9199,   9203,   9221,   9277,   9283,   9293,   9311,   9323,   9337,   9341,   9371,   9377,   9391,   9419,   9433,   9467,   9479,   9497,
		9587,   9601,   9613,   9619,   9623,   9649,   9661,   9689,   9697,   9719,   9739,   9743,   9767,   9787,   9811,   9851,   9857,   9859,   9871,   9901,   9931,   9949,   9973,  10009,
		10037,  10039,  10061,  10067,  10069,  10079,  10091,  10093,  10103,  10111,  10133,  10141,  10159,  10163,  10169,  10193,  10243,  10247,  10259,  10267,  10271,  10289,  10301, 10303,
		10321,  10333,  10343,  10357,  10369,  10391,  10399,  10427,  10429,  10433,  10457,  10463,  10499,  10501,  10513,  10531,  10559,  10589,  10597,  10631,  10651,  10667,  10691,  10709,
		10711,  10733,  10753,  10789,  10799,  10837,  10853,  10861,  10903,  10949,  11057,  11059,  11069,  11093,  11117,  11119,  11131,  11149,  11159,  11161,  11171,  11173,  11213,  11257, 
		11261,  11273,  11287,  11317,  11321,  11351,  11423,  11443,  11447,  11471,  11483,  11489,  11503,  11519,  11587,  11617,  11657,  11677,  11681,  11689,  11699,  11701,  11719,  11789, 
		11821,  11827,  11831,  11833,  11863,  11887,  11897,  11927,  11939,  11969,  11971,  11981,  11987,  12011,  12037,  12071,  12101,  12107,  12119,  12149,  12157,  12163,  12203,  12211, 
		12251,  12263,  12277,  12301,  12323,  12329,  12347,  12391,  12401,  12409,  12433,  12437,  12451,  12473,  12491,  12497,  12511,  12517,  12527,  12539,  12541,  12547,  12569,  12583, 
		12589,  12613,  12619,  12641,  12647,  12653,  12671,  12689,  12697,  12713,  12739,  12757,  12781,  12791,  12821,  12823,  12829,  12841,  12889,  12893,  12923,  12959,  12967,  12973, 
		12979,  12983,  13001,  13009,  13099,  13103,  13121,  13127,  13151,  13159,  13171,  13177,  13183,  13187,  13217,  13219,  13229,  13241,  13291,  13297,  13327,  13339,  13367,  13381, 
		13397,  13399,  13417,  13451,  13457,  13469,  13477,  13499,  13513,  13523,  13567,  13577,  13591,  13613,  13649,  13669,  13681,  13691,  13693,  13697,  13721,  13751,  13763,  13781, 
		13789,  13799,  13859,  13879,  13883,  13903,  13907,  13913,  13921,  13997,  14011,  14029,  14057,  14071,  14081,  14083,  14143,  14153,  14159,  14197,  14207,  14221,  14243,  14249, 
		14281,  14293,  14303,  14321,  14323,  14327,  14347,  14369,  14411,  14419,  14437,  14447,  14489,  14519,  14533,  14537,  14557,  14561,  14563,  14621,  14669,  14683,  14713,  14741, 
		14753,  14767,  14783,  14813,  14821,  14827,  14867,  14879,  14887,  14897,  14951,  14957,  14969,  14983,  15013,  15031,  15053,  15091,  15101,  15107,  15121,  15131,  15137,  15139, 
		15161,  15173,  15187,  15193,  15227,  15259,  15287,  15289,  15299,  15313,  15319,  15331,  15361,  15373,  15377,  15383,  15413,  15443,  15451,  15461,  15467,  15511,  15527,  15541, 
		15551,  15581,  15629,  15641,  15661,  15671,  15679,  15683,  15733,  15737,  15739,  15749,  15761,  15767,  15787,  15791,  15797,  15817,  15877,  15881,  15889,  15907,  15923,  15959, 
		15971,  16001,  16033,  16057,  16061,  16063,  16069,  16073,  16091,  16103,  16111,  16127,  16141,  16187,  16193,  16231,  16249,  16253,  16273,  16301,  16319,  16333,  16339,  16349, 
		16363,  16369,  16381,  16417,  16421,  16433,  16447,  16451,  16493,  16529,  16547,  16573,  16603,  16607,  16619,  16633,  16657,  16741,  16747,  16787,  16831,  16879,  16883,  16889, 
		16901,  16903,  16937,  16963,  16979,  16987,  17011,  17033,  17041,  17077,  17099,  17107,  17117,  17137,  17203,  17207,  17209,  17239,  17291,  17293,  17321,  17327,  17333,  17359, 
		17377,  17383,  17393,  17417,  17419,  17431,  17449,  17491,  17497,  17509,  17539,  17551,  17569,  17609,  17623,  17659,  17669,  17681,  17729,  17749,  17761,  17807,  17827,  17837, 
		17851,  17863,  17881,  17903,  17909,  17911,  17923,  17929,  17957,  17981,  18041,  18047,  18061,  18077,  18089,  18119,  18121,  18131,  18133,  18149,  18181,  18229,  18233,  18251, 
		18257,  18269,  18287,  18301,  18307,  18341,  18367,  18379,  18397,  18427,  18433,  18443,  18451,  18457,  18481,  18503,  18517,  18523,  18541,  18587,  18617,  18713,  18719,  18749, 
		18757,  18787,  18839,  18869,  18917,  18919,  18959,  18979,  19009,  19037,  19073,  19079,  19087,  19121,  19163,  19181,  19183,  19207,  19213,  19237,  19249,  19267,  19301,  19319, 
		19379,  19381,  19391,  19403,  19417,  19423,  19429,  19433,  19441,  19447,  19457,  19463,  19471,  19477,  19489,  19501,  19507,  19541,  19543,  19553,  19559,  19577,  19583,  19597, 
		19603,  19697,  19709,  19751,  19763,  19777,  19813,  19841,  19853,  19861,  19867,  19913,  19927,  20029,  20051,  20071,  20143,  20173,  20177,  20183,  20231,  20249,  20297,  20323, 
		20341,  20357,  20359,  20407,  20411,  20431,  20443,  20483,  20507,  20533,  20549,  20551,  20599,  20611,  20627,  20639,  20641,  20663,  20693,  20731,  20743,  20747,  20749,  20753, 
		20759,  20771,  20773,  20807,  20857,  20873,  20887,  20897,  20899,  20939,  20947,  20963,  21061,  21089,  21121,  21143,  21149,  21157,  21169,  21187,  21193,  21247,  21323,  21341, 
		21347,  21379,  21383,  21391,  21397,  21467,  21481,  21499,  21503,  21517,  21521,  21523,  21563,  21577,  21587,  21613,  21647,  21683,  21713,  21737,  21739,  21751,  21767,  21803, 
		21839,  21859,  21893,  21911,  21929,  21937,  21961,  21991,  22013,  22027,  22067,  22079,  22091,  22109,  22111,  22129,  22133,  22153,  22159,  22171,  22193,  22273,  22279,  22283, 
		22291,  22307,  22349,  22367,  22369,  22381,  22409,  22433,  22481,  22501,  22511,  22531,  22541,  22543,  22549,  22567,  22573,  22637,  22669,  22679,  22691,  22697,  22721,  22727, 
		22739,  22741,  22777,  22783,  22787,  22807,  22861,  22871,  22907,  22943,  22961,  22973,  22993,  23003,  23011,  23017,  23021,  23027,  23057,  23063,  23081,  23087,  23117,  23143, 
		23167,  23189,  23227,  23251,  23279,  23291,  23293,  23297,  23311,  23327,  23333,  23339,  23369,  23447,  23459,  23497,  23537,  23539,  23549,  23563,  23567,  23581,  23593,  23599, 
		23609,  23663,  23677,  23687,  23689,  23741,  23747,  23753,  23767,  23801,  23819,  23827,  23831,  23833,  23879,  23899,  23909,  23917,  23929,  23971,  23977,  23981,  23993,  24001, 
		24007,  24019,  24043,  24049,  24071,  24077,  24091,  24097,  24103,  24133,  24137,  24151,  24179,  24223,  24229,  24239,  24281,  24337,  24359,  24373,  24391,  24413,  24419,  24439, 
		24443,  24469,  24481,  24527,  24533,  24551,  24571,  24593,  24611,  24623,  24659,  24671,  24677,  24683,  24691,  24697,  24733,  24749,  24767,  24781,  24793,  24821,  24841,  24847, 
		24889,  24943,  24979,  24989,  25013,  25031,  25033,  25037,  25057,  25087,  25097,  25111,  25147,  25183,  25301,  25307,  25309,  25321,  25339,  25357,  25373,  25391,  25471,  25537, 
		25541,  25577,  25601,  25609,  25633,  25639,  25667,  25673,  25693,  25703,  25733,  25793,  25799,  25841,  25867,  25913,  25931,  25933,  25951,  25969,  25997,  25999,  26003,  26017, 
		26029,  26053,  26113,  26161,  26171,  26177,  26183,  26189,  26203,  26209,  26227,  26237,  26293,  26317,  26321,  26387,  26393,  26407,  26431,  26459,  26479,  26489,  26497,  26513, 
		26539,  26557,  26561,  26573,  26591,  26597,  26647,  26681,  26687,  26699,  26717,  26729,  26731,  26759,  26777,  26783,  26801,  26821,  26833,  26863,  26893,  26903,  26927,  26959, 
		26987,  27011,  27061,  27067,  27073,  27077,  27107,  27109,  27127,  27191,  27239,  27241,  27283,  27299,  27329,  27337,  27361,  27367,  27407,  27409,  27427,  27449,  27481,  27487, 
		27509,  27529,  27539,  27541,  27551,  27581,  27691,  27701,  27733,  27737,  27743,  27749,  27767,  27791,  27793,  27817,  27823,  27827,  27893,  27919,  27941,  27943,  27953,  27961, 
		28019,  28027,  28031,  28051,  28069,  28087,  28097,  28151,  28181,  28183,  28201,  28211,  28219,  28229,  28277,  28279,  28289,  28307,  28309,  28349,  28351,  28387,  28393,  28409, 
		28411,  28447,  28463,  28477,  28493,  28499,  28513,  28517,  28537,  28559,  28573,  28579,  28597,  28603,  28621,  28627,  28649,  28687,  28703,  28753,  28759,  28793,  28813,  28817, 
		28837,  28867,  28879,  28909,  28921,  28927,  28933,  28949,  28961,  29017,  29023,  29033,  29059,  29063,  29129,  29137,  29153,  29179,  29191,  29201,  29209,  29243,  29251,  29287, 
		29297,  29311,  29327,  29347,  29363,  29383,  29389,  29399,  29423,  29437,  29443,  29473,  29527,  29537,  29567,  29573,  29581,  29599,  29629,  29633,  29683,  29717,  29723,  29741, 
		29759,  29761,  29803,  29833,  29863,  29867,  29879,  29881,  29927,  29947,  29959,  29983,  30059,  30071,  30097,  30103,  30109,  30113,  30139,  30169,  30203,  30211,  30241,  30269, 
		30307,  30367,  30389,  30403,  30427,  30449,  30491,  30517,  30529,  30553,  30559,  30577,  30593,  30631,  30649,  30677,  30689,  30757,  30763,  30803,  30817,  30841,  30853,  30869, 
		30871,  30881,  30893,  30911,  30983,  31019,  31039,  31079,  31081,  31091,  31121,  31151,  31153,  31159,  31177,  31181,  31189,  31193,  31231,  31237,  31259,  31267,  31271,  31277, 
		31321,  31337,  31357,  31379,  31391,  31393,  31469,  31489,  31517,  31531,  31547,  31567,  31627,  31657,  31687,  31721,  31723,  31727,  31751,  31769,  31793,  31799,  31817,  31849, 
		31873,  31883,  31907,  31973,  32027,  32029,  32057,  32083,  32089,  32099,  32117,  32141,  32159,  32189,  32203,  32213,  32233,  32237,  32261,  32297,  32309,  32321,  32323,  32327, 
		32341,  32353,  32381,  32401,  32411,  32413,  32443,  32497,  32531,  32563,  32579,  32603,  32609,  32611,  32621,  32633,  32647,  32693,  32713,  32717,  32789,  32797,  32833,  32839, 
		32869,  32887,  32909,  32933,  32941,  32957,  32969,  32971,  32987,  33013,  33053,  33071,  33107,  33149,  33161,  33181,  33211,  33287,  33311,  33343,  33349,  33391,  33403,  33457, 
		33479,  33493,  33503,  33521,  33533,  33569,  33589,  33599,  33601,  33619,  33629,  33641,  33647,  33713,  33721,  33749,  33791,  33809,  33827,  33851,  33863,  33931,  33937,  33967, 
		34123,  34127,  34129,  34147,  34157,  34159,  34183,  34217,  34253,  34259,  34261,  34267,  34273,  34283,  34297,  34301,  34319,  34351,  34403,  34429,  34439,  34469,  34483,  34487, 
		34511,  34543,  34589,  34591,  34603,  34607,  34649,  34757,  34759,  34807,  34819,  34841,  34843,  34847,  34849,  34877,  34897,  34913,  34919,  34939,  34961,  35023,  35059,  35081, 
		35083,  35107,  35117,  35153,  35159,  35171,  35221,  35227,  35251,  35327,  35353,  35393,  35401,  35407,  35491,  35507,  35509,  35527,  35531,  35537,  35573,  35591,  35593,  35597, 
		35603,  35617,  35671,  35729,  35731,  35747,  35759,  35797,  35801,  35803,  35809,  35831,  35851,  35863,  35869,  35879,  35899,  35933,  35951,  35963,  35977,  35993,  36011,  36013, 
		36017,  36061,  36067,  36109,  36131,  36137,  36151,  36161,  36191,  36217,  36299,  36307,  36341,  36343,  36479,  36493,  36529,  36551,  36559,  36563,  36571,  36599,  36629,  36637, 
		36653,  36671,  36677,  36683,  36697,  36713,  36721,  36739,  36749,  36767,  36779,  36781,  36787,  36809,  36833,  36847,  36857,  36871,  36887,  36899,  36901,  36913,  36931,  36973, 
		36997,  37003,  37049,  37117,  37159,  37189,  37199,  37201,  37217,  37223,  37253,  37273,  37307,  37309,  37321,  37339,  37361,  37363,  37379,  37463,  37483,  37489,  37511,  37517, 
		37571,  37579,  37607,  37633,  37747,  37799,  37813,  37831,  37847,  37853,  37879,  37897,  37907,  37951,  37957,  37987,  37991,  37993,  38011,  38039,  38047,  38113,  38153,  38183, 
		38231,  38273,  38303,  38327,  38329,  38371,  38377,  38431,  38447,  38449,  38453,  38557,  38593,  38609,  38651,  38671,  38693,  38699,  38707,  38737,  38749,  38767,  38803,  38833, 
		38839,  38851,  38861,  38867,  38903,  38917,  38923,  38933,  38971,  38977,  39019,  39023,  39047,  39079,  39089,  39097,  39133,  39161,  39163,  39181,  39191,  39199,  39209,  39217, 
		39233,  39239,  39241,  39251,  39301,  39313,  39323,  39341,  39343,  39373,  39383,  39419,  39451,  39509,  39511,  39551,  39569,  39631,  39659,  39671,  39679,  39709,  39719,  39727, 
		39733,  39761,  39769,  39799,  39821,  39827,  39841,  39863,  39869,  39877,  39901,  39953,  39971,  39979,  39983,  40013,  40037,  40039,  40087,  40093,  40111,  40123,  40127,  40153, 
		40163,  40189,  40193,  40231,  40237,  40241,  40253,  40277,  40289,  40351,  40361,  40387,  40423,  40427,  40459,  40471,  40507,  40529,  40531,  40543,  40559,  40577,  40583,  40609, 
		40627,  40637,  40693,  40697,  40763,  40787,  40801,  40829,  40841,  40847,  40849,  40853,  40897,  40903,  40993,  41011,  41017,  41023,  41039,  41081,  41113,  41149,  41161,  41177, 
		41179,  41189,  41201,  41203,  41257,  41263,  41281,  41299,  41357,  41381,  41389,  41411,  41443,  41453,  41507,  41513,  41521,  41539,  41543,  41579,  41593,  41597,  41603,  41611, 
		41617,  41621,  41627,  41641,  41647,  41659,  41669,  41719,  41729,  41737,  41759,  41761,  41771,  41801,  41809,  41843,  41849,  41893,  41911,  41927,  41941,  41953,  41957,  41959, 
		41969,  42019,  42061,  42071,  42073,  42131,  42139,  42157,  42169,  42193,  42223,  42257,  42283,  42293,  42299,  42307,  42323,  42331,  42337,  42349,  42379,  42397,  42433,  42437, 
		42443,  42451,  42461,  42467,  42473,  42487,  42491,  42499,  42557,  42571,  42577,  42611,  42641,  42643,  42677,  42697,  42703,  42727,  42773,  42793,  42821,  42841,  42853,  42863, 
		42901,  42929,  42937,  42943,  42961,  42967,  42979,  42989,  43013,  43037,  43067,  43093,  43117,  43159,  43177,  43223,  43271,  43283,  43291,  43319,  43331,  43391,  43399,  43403, 
		43427,  43441,  43487,  43517,  43541,  43577,  43579,  43591,  43607,  43613,  43633,  43661,  43691,  43711,  43717,  43721,  43759,  43781,  43783,  43793,  43801,  43853,  43933,  43963, 
		43973,  43997,  44029,  44059,  44101,  44111,  44123,  44179,  44201,  44207,  44257,  44263,  44269,  44273,  44279,  44351,  44357,  44381,  44449,  44497,  44507,  44537,  44543,  44587, 
		44621,  44623,  44633,  44641,  44647,  44657,  44701,  44773,  44777,  44797,  44809,  44819,  44839,  44843,  44851,  44887,  44893,  44909,  44917,  44927,  44983,  44987,  45007,  45053, 
		45061,  45077,  45131,  45139,  45161,  45179,  45191,  45247,  45259,  45263,  45289,  45307,  45337,  45343,  45361,  45377,  45389,  45403,  45413,  45433,  45439,  45481,  45491,  45497, 
		45541,  45557,  45569,  45587,  45631,  45641,  45673,  45691,  45737,  45751,  45817,  45823,  45827,  45833,  45863,  45887,  45893,  45949,  45953,  45959,  45971,  46021,  46027,  46061, 
		46073,  46093,  46103,  46171,  46183,  46187,  46199,  46219,  46237,  46279,  46301,  46307,  46309,  46327,  46337,  46399,  46411,  46441,  46447,  46451,  46457,  46477,  46499,  46511, 
		46559,  46601,  46619,  46633,  46639,  46643,  46663,  46681,  46687,  46691,  46747,  46771,  46807,  46811,  46817,  46819,  46831,  46853,  46867,  46877,  46889,  46919,  46957,  46997, 
		47041,  47059,  47111,  47119,  47143,  47149,  47161,  47207,  47221,  47237,  47251,  47279,  47287,  47293,  47297,  47303,  47351,  47353,  47381,  47387,  47407,  47419,  47441,  47491, 
		47497,  47501,  47507,  47513,  47521,  47563,  47569,  47581,  47591,  47623,  47639,  47653,  47657,  47699,  47701,  47713,  47717,  47743,  47807,  47819,  47837,  47857,  47869,  47881, 
		47911,  47917,  47933,  47947,  47969,  47977,  47981,  48017,  48023,  48029,  48121,  48131,  48157,  48163,  48179,  48187,  48193,  48221,  48247,  48259,  48299,  48313,  48337,  48371, 
		48413,  48437,  48463,  48473,  48481,  48491,  48497,  48523,  48563,  48593,  48619,  48623,  48649,  48661,  48731,  48751,  48757,  48767,  48821,  48823,  48973,  48989,  48991,  49003, 
		49009,  49019,  49031,  49033,  49043,  49069,  49139,  49169,  49177,  49201,  49223,  49253,  49261,  49297,  49333,  49369,  49391,  49417,  49429,  49433,  49477,  49523,  49529,  49531, 
		49549,  49597,  49613,  49627,  49633,  49663,  49667,  49669,  49711,  49727,  49739,  49741,  49747,  49787,  49807,  49823,  49843,  49871,  49919,  49921,  49927,  49939,  49991,  49993, 
		49999,  50023,  50033,  50047,  50101,  50123,  50147,  50207,  50231,  50273,  50287,  50291,  50311,  50341,  50377,  50383,  50411,  50441,  50459,  50497,  50503,  50527,  50549,  50587, 
		50599,  50627,  50647,  50651,  50723,  50767,  50773,  50777,  50857,  50873,  50891,  50957,  50969,  50989,  51001,  51043,  51061,  51131,  51133,  51137,  51151,  51157,  51199,  51203, 
		51217,  51229,  51263,  51283,  51287,  51307,  51341,  51347,  51383,  51421,  51427,  51431,  51437,  51439,  51449,  51461,  51473,  51479,  51481,  51511,  51521,  51563,  51581,  51599, 
		51607,  51637,  51679,  51691,  51721,  51749,  51767,  51769,  51787,  51797,  51803,  51817,  51859,  51871,  51893,  51907,  51929,  51941,  51949,  51977,  52021,  52027,  52069,  52121, 
		52153,  52163,  52181,  52201
		};
	
		private static bool IsSmooth(BigInteger S, BigInteger pk)
        {
			
            BigInteger div = BigInteger.GreatestCommonDivisor(S, pk);
			if (div.IsZero || S.IsZero)
				return false;
			/*
			if (!(div.IsOne || S.IsOne))
				return IsSmooth(S / div, pk);
			else
				return S.IsOne;
			*/
            while (!div.IsOne)
            {
                S /= div;
                div = BigInteger.GreatestCommonDivisor(S, pk);
            }
            return S.IsOne;         // smooth number with prime bound in factor_base
			
        }

        private static uint[] GetPrimeFactorsII(BigInteger N)
        {
            uint[] factors = new uint[primes.Length];
            int i = 0;
			Console.Write("1 ");
            foreach (uint pr in primes)
            {
                factors[i] = 0;
                BigInteger rem = BigInteger.Zero;
                while (true)    // divisible by prime in factor_base[]
                {
					BigInteger Q = BigInteger.DivRem(N, pr, out rem);
					if (rem.IsZero)
					{
						N = Q;
						factors[i]++;
						Console.Write("* {0}",pr);
					}
					else break;
				}
                i++;

                if (N.IsOne) break;
            }
			Console.WriteLine();
			if (N.IsOne)         // smooth number with prime bound in factor_base
                return factors;
            else
                return null;
        }

        static Solution Ts(BigInteger n, BigInteger p) {
            if (BigInteger.ModPow(n, (p - 1) >> 1, p) != 1) {
                return new Solution(0, 0, false);
            }
 
            BigInteger Q = p - 1;
            uint S = 0;
            while ((Q & 1) == 0) {
                S += 1;
                Q = Q >> 1;
            }
 
            if (S == 1) {
                BigInteger r1 = BigInteger.ModPow(n, (p + 1) >> 2, p);
                return new Solution(r1, p - r1, true);
            }
 
			uint z = 2;
            foreach (uint zz in primes)
			{
				if (BigInteger.ModPow(zz, (p - 1) >> 1, p) == p - 1)
				{
					z = zz;
					break;
				}
                //z += 1;
            }
			// Console.WriteLine("{0,10}", z);
            BigInteger c = BigInteger.ModPow(z, Q, p);
            BigInteger r = BigInteger.ModPow(n, (Q + 1) >> 1, p);
            BigInteger t = BigInteger.ModPow(n, Q, p);
            BigInteger M = S;
 
            if (t == 0)
				return new Solution(0, 0, false);
			else
			{
                uint i = 0;
                BigInteger zz = t;
                while (zz != 1 && i < (M - 1)) {
                    zz = zz * zz % p;
                    i += 1;
					//Console.WriteLine("{0,8}", i);
                }
                BigInteger b = c;
                BigInteger e = M - i - 1;
                while (e > 0) {
                    b = b * b % p;
                    e = e - 1;
                }
                r = r * b % p;
                c = b * b % p;
                t = t * c % p;
                M = i;
            }
			if (t == 1)
				return new Solution(r, p - r, true);
			else
				return new Solution(0, 0, false);
        }

		static BigInteger SquareRoot(BigInteger n)
		{
			double HalfLogXBase2 = BigInteger.Log(n, 2) / 2.0;
			BigInteger d = n >> (int)HalfLogXBase2;
			BigInteger q, _d;
			Stopwatch sw = new Stopwatch();

			int i = 0;
			sw.Start();
			//Newton's Method
			do
			{
				q = n / d + d;
				_d = d;
				q >>= 1;
				d = q;
				i++;
			} while (q * q > n);
			sw.Stop();

			Debug.Assert(q * q <= n);
			return q;
		}
		
		private static void FindSmooth(BigInteger S, BigInteger p, BigInteger r, BigInteger start, BigInteger end, BigInteger primorial)
		{
			Debug.Assert( (S % p).IsZero );
			Stopwatch sw = new Stopwatch();
			bool smooth = false;
			
			sw.Start();
			BigInteger k = p + r;
			BigInteger N = S + k * k;
			//Debug.Fail("Attaching debugger...");
			
			for (; k < (end - start) && !smooth; k += p)
			{
				S = N - k * k;
				smooth = IsSmooth(S, primorial);
			}
			sw.Stop();
			
			Console.WriteLine("smooth loop time elapsed: {0:F1}", sw.Elapsed.TotalSeconds);
			
			if (smooth)
			{
				Console.WriteLine("\nS = {0}", S);
				uint[] factors = GetPrimeFactorsII(S);
				/*int i = 0;
				foreach (uint expo in factors)
					if (expo > 0)
						Console.Write("{0,6}^{1}", primes[i++], expo);
				Console.WriteLine();
				*/
				Debug.Assert(factors != null);
			}
		}
 
        static void Main(string[] args) {
            List<Tuple<long, long>> pairs = new List<Tuple<long, long>>() {
                new Tuple<long, long>(10, 13),
                new Tuple<long, long>(56, 101),
				new Tuple<long, long>(800, 2923),
                new Tuple<long, long>(1030, 10009),
                new Tuple<long, long>(1032, 10009),
                new Tuple<long, long>(44402, 100049),
                new Tuple<long, long>(665820697, 1000000009),
                new Tuple<long, long>(881398088036, 1000000000039),
            };
 
            foreach (var pair in pairs) {
                Solution sol = Ts(pair.Item1, pair.Item2);
                Console.WriteLine("n = {0}", pair.Item1);
                Console.WriteLine("p = {0}", pair.Item2);
                if (sol.Exists()) {
                    Console.WriteLine("root1 = {0}", sol.Root1());
                    Console.WriteLine("root2 = {0}", sol.Root2());
                } else {
                    Console.WriteLine("No solution exists");
                }
                Console.WriteLine();
            }

 
            BigInteger bn = BigInteger.Parse("41660815127637347468140745042827704103445750172002");
            BigInteger bp = BigInteger.Pow(10, 50) + 577;
            Solution bsol = Ts(bn, bp);
            Console.WriteLine("n = {0}", bn);
            Console.WriteLine("p = {0}", bp);
            if (bsol.Exists()) {
                Console.WriteLine("root1 = {0}", bsol.Root1());
                Console.WriteLine("root2 = {0}", bsol.Root2());
            } else {
                Console.WriteLine("No solution exists");
            }

			Console.WriteLine();
			
			//bn = BigInteger.Parse("4611686217423659867");
			//bn = BigInteger.Parse("81639369383890472319083144055093154391");
			bn = BigInteger.Parse("1675057545243128568837590196314063119");
			Console.WriteLine("n = {0}", bn);

			BigInteger a0 = SquareRoot(bn);
			BigInteger a1 = SquareRoot(bn << 1);
			Console.WriteLine("a0 = {0}\na1 = {1}\n", a0, a1);
			
			BigInteger fb_primorial = BigInteger.One;
			foreach (uint pr in primes)
			{
				if (pr > 1)
					fb_primorial *= pr;
			}
			/*
			bool smooth = IsSmooth(2548736398, 113418769711);
			Console.WriteLine("1. IsSmooth: {0}", smooth);
			
			smooth = IsSmooth(2548736398, 226837539422);
			Console.WriteLine("2. IsSmooth: {0}", smooth);
			
			smooth = IsSmooth(10094270504279, 2548736398);
			Console.WriteLine("3. IsSmooth: {0}", smooth);
			
			Debug.Assert(smooth);
			Console.ReadLine();			
			*/
			//bp = BigInteger.One;
			foreach (uint p in primes)
			{
				if (p < 10000) continue;
				//Console.Write("p = ");
				//uint q = 1;
				/*
				foreach (uint q in primes1)
				{
					if (q == p || q > 10000) continue;
					
					foreach (uint r in primes1)
					{
						if (r == p || r == q || r > 10000) continue;
						
						bp = p * q;
				*/				
				//Console.Write("bp = {0}\r", bp);
				bsol = Ts(bn, p);

				if (bsol.Exists())
				{
				/*
				if (bp.Equals(p * q))
						Console.WriteLine("p * q = {0} * {1} = {2}", p, q, bp);
					if (bp.Equals(p * r))
						Console.WriteLine("p * r = {0} * {1} = {2}", p, r, bp);
					if (bp.Equals(q * r))
						Console.WriteLine("q * r = {0} * {1} = {2}", q, r, bp);
				*/
					BigInteger r1 = bsol.Root1();
					BigInteger r2 = bsol.Root2();
					Console.Write("p = {0}\t", p);
					Console.Write("root1 = {0}\t", r1);
					Console.WriteLine("root2 = {0}\n", r2);
							
					BigInteger n1 = bn - (p + r1) * (p + r1);
					Debug.Assert( (n1 % p).IsZero );
					BigInteger q1 = n1 / p;

					Console.WriteLine("n1 / p = {0}", q1);
					Debug.Assert( (bn - p * q1) == (p + r1) * (p + r1) );
					Debug.Assert( ((bn - n1) % (p + r1)).IsZero );

					BigInteger n2 = bn - (p + r2) * (p + r2);
					Debug.Assert( (n2 % p).IsZero );
					BigInteger q2 = n2 / p;

					Console.WriteLine("n2 / p = {0}", q2);
					Debug.Assert( (bn - n2) == (p + r2) * (p + r2) );

					Debug.Assert( (n1 % q1).IsZero );
					Debug.Assert( (n2 % q2).IsZero );

					/*
					bsol = Ts(n1, p);
					if (bsol.Exists())
					{
						r1 = bsol.Root1();
						r2 = bsol.Root2();
								
						Debug.Assert( (q1 - r1 * r1) % p == 0 );
						Console.WriteLine("root1 = {0}", r1);
								
						Debug.Assert( (q1 - r2 * r2) % p == 0 );
						Console.WriteLine("root2 = {0}", r2);
					}
					else
					{
						Console.WriteLine("No solution exists for 2nd iteration.");
						//continue;
					}
					*/

					FindSmooth(n1, p, r1, a0, a1, fb_primorial);
					/*
					bool smooth = IsSmooth(n1, fb_primorial);
					if (smooth)
						Console.WriteLine("IsSmooth({0}): {1}", n1, smooth);
					
					smooth = IsSmooth(n2, fb_primorial);
					if (smooth)
						Console.WriteLine("IsSmooth({0}): {1}", n2, smooth);
					*/
					Console.WriteLine("\n{0}", new String('-', 80));
				} 
				else {
					//Console.WriteLine("No solution exists\n");
				}
				//}
			//}
			}
			
			Console.Write("\nCtrl-C: ");
			Console.ReadLine();
			
			Stopwatch sw = new Stopwatch();
			sw.Start();
			{
				int BigThree = (int)Math.Pow(3.0, 13.0);
				BigInteger BigThird = BigInteger.Pow(3, BigThree);
				BigThree *= 3;
				BigThird += BigInteger.Pow(3, BigThree);
				BigThird += 1;
				
				Console.WriteLine("\n\nLog10(3^(3^14) + 3^(3^13) + 1): {0}", BigInteger.Log10(BigThird));
			}
			sw.Stop();
			Console.WriteLine("BigThird took {0:F1} secs.", sw.Elapsed.TotalSeconds);
        }
    }
}
