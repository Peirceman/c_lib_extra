#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stddef.h>

#define LOAD_FACTOR_DEF     0.75
#define LOAD_FACTOR_NO_GROW 2.0
#define ENTRIES_CAP_DEF     11

#define HT_init_default(key_typ, val_typ, hash_func, eq_func) \
	HT_init(sizeof(key_typ), sizeof(val_typ),                 \
			ENTRIES_CAP_DEF, LOAD_FACTOR_DEF,                 \
			hash_func, eq_func)                               \

typedef int (*eq_func) ();
typedef int (*hash_func) ();

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
