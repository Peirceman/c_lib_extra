#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <math.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

#define LOAD_FACTOR_DEF     0.75
#define LOAD_FACTOR_NO_GROW 2.0
#define ENTRIES_CAP_DEF     11

#define HT_init_default(key_typ, val_typ, hash_func, eq_func) \
	HT_init(sizeof(key_typ), sizeof(val_typ),                 \
			ENTRIES_CAP_DEF, LOAD_FACTOR_DEF,                 \
			hash_func, eq_func)                               \

typedef int (*eq_func) ();
typedef unsigned int (*hash_func) ();

typedef struct Entry {
	struct Entry *next;
	void *val;
	void *key;
} Entry;

typedef struct {
	Entry   **entries;
	size_t    count;
	size_t    entries_cap;
	double    load_factor; 
	size_t    key_sz;
	size_t    val_sz;
	hash_func hash_func;
	eq_func   eq_func;
} Hash_table;

Hash_table *HT_init(size_t key_sz, size_t val_sz, size_t entries_cap,
		double load_factor, hash_func hash_func, eq_func eq_func);
void *HT_get(const Hash_table *hash_table, const void *key);
void *HT_del(Hash_table *hash_table, const void *key);
void *HT_put(Hash_table *hash_table, const void *key, void *val);
void  HT_free(Hash_table *hash_table);
void  HT_clear(Hash_table *hash_table);
int   HT_contains_key(const Hash_table *hash_table, const void *key);

#endif /* HASH_TABLE_H_ */

#ifndef DEFAULT_HASH_FUNCS_
#define DEFAULT_HASH_FUNCS_

inline static unsigned int def_char_hash(const char *a) {
	return (unsigned int) *a;
}

inline static unsigned int def_s_char_hash(const signed char *a) {
	return (unsigned int) *a;
}

inline static unsigned int def_u_char_hash(const unsigned char *a) {
	return (unsigned int) *a;
}

inline static unsigned int def_short_hash(const short *a) {
	return (unsigned int) *a;
}

inline static unsigned int def_u_short_hash(const unsigned short *a) {
	return (unsigned int) *a;
}

inline static unsigned int def_int_hash(const int *a) {
	return (unsigned int) *a;
}

inline static unsigned int def_u_int_hash(const unsigned int *a) {
	return *a;
}

inline static unsigned int def_long_hash(const long *a) {
#if INT_MAX == LONG_MAX
	return (unsigned int) *a;
#else
	return (unsigned int) (*a ^ ((unsigned long) *a >> 32));
#endif
}

inline static unsigned int def_u_long_hash(const unsigned long *a) {
#if UINT_MAX == ULONG_MAX
	return (unsigned int) *a;
#else
	return (unsigned int) (*a ^ (*a >> 32));
#endif
}

inline static unsigned int def_long_long_hash(const long long *a) {
	return (unsigned int) (*a ^ (*a >> 32));
}

inline static unsigned int def_u_long_long_hash(const unsigned long long *a) {
	return (unsigned int) (*a ^ (*a >> 32));
}

inline static unsigned int def_float_hash(const float *a) {
	return *(unsigned int*) a;
}

inline static unsigned int def_double_hash(const double *a) {
	unsigned long long al = *(unsigned long long *) a;
	return (unsigned int) (al ^ (al >> 32));
}

inline static unsigned int def_long_double_hash(const long double *a) {
	unsigned long long a0 = *(unsigned long long*) a;
	unsigned long long a1 = *((unsigned long long*) a + 8);

	return (unsigned int) a0 ^ (a0 >> 32) ^ a1 ^ (a1 >> 32);
}

inline static unsigned int def_str_hash(const char **a) {
	const char *str = *a;

	unsigned int h = 0;
	char c;
	while((c = *str++) != 0) {
		h = 31 * h + (c & 0xff);
	}

	return h;
}

inline static unsigned int def_s_str_hash(const signed char **a) {
	return def_str_hash((const char **)a);
}

inline static unsigned int def_u_str_hash(const unsigned char **a) {
	return def_str_hash((const char **)a);
}

#endif /* DEFAULT_HASH_FUNCS_ */

#ifndef DEFAULT_EQ_FUNCS_
#define DEFAULT_EQ_FUNCS_

inline static int def_char_eq(const char *a, const char *b) {
	return *a == *b;
}

inline static int def_s_char_eq(const signed char *a, const signed char *b) {
	return *a == *b;
}

inline static int def_u_char_eq(const unsigned char *a, const unsigned char *b) {
	return *a == *b;
}

inline static int def_short_eq(const short *a, const short *b) {
	return *a == *b;
}

inline static int def_u_short_eq(const unsigned short *a, const unsigned short *b) {
	return *a == *b;
}

inline static int def_int_eq(const int *a, const int *b) {
	return *a == *b;
}

inline static int def_u_int_eq(const unsigned int *a, const unsigned int *b) {
	return *a == *b;
}

inline static int def_long_eq(const long *a, const long *b) {
	return *a == *b;
}

inline static int def_u_long_eq(const unsigned long *a, const unsigned long *b) {
	return *a == *b;
}

inline static int def_long_long_eq(const long long *a, const long long *b) {
	return *a == *b;
}

inline static int def_u_long_long_eq(const unsigned long long *a, const unsigned long long *b) {
	return *a == *b;
}

inline static int def_float_eq(const float *a, const float *b) {
	return (*(float *)&(unsigned){(*(unsigned *) &(float){*a - *b}) & (0x7fffffff)}) < 0.0000001F;
}

inline static int def_double_eq(const double *a, const double *b) {
	return (*(double *)&(long long unsigned){(*(long long unsigned *) &(double){*a - *b}) & (0x7fffffffffffffff)}) < 0.0000001;
}

/*
 * linking with libm is not nececary if you won't use this function (I think)
 */
inline static int def_long_double_eq(const long double *a, const long double *b) {
	const long double diff = fabsl(*a - *b);

	return diff < (long double) 0.0000001;
}

inline static int def_str_eq(const char **a, const char **b) {
	return strcmp(*a, *b) == 0;
}

inline static int def_s_str_eq(const signed char **a, const signed char **b) {
	return strcmp((const char*) *a,(const char*) *b) == 0;
}

inline static int def_u_str_eq(const unsigned char **a, const unsigned char **b) {
	return strcmp((const char*) *a,(const char*) *b) == 0;
}

#endif /* DEFAULT_EQ_FUNCS_ */
