#ifndef SET_H_
#define SET_H_

#include <stddef.h>

#define LOAD_FACTOR_DEF     0.75
#define LOAD_FACTOR_NO_GROW 2.0
#define ENTRIES_CAP_DEF     11

#define SET_init_default(key_typ, hash_func, eq_func) \
	SET_init(sizeof(key_typ),                         \
			ENTRIES_CAP_DEF, LOAD_FACTOR_DEF,         \
			hash_func, eq_func)                       \


typedef int (*eq_func) ();
typedef int (*hash_func) ();

typedef struct Entry {
	struct Entry *next;
	void *key;
} Entry;

typedef struct {
	Entry   **entries;
	size_t    count;
	size_t    entries_cap;
	double    load_factor;
	size_t    key_sz;
	hash_func hash_func;
	eq_func   eq_func;
} Set;

Set *SET_init(size_t key_sz, size_t entries_cap,
		double load_factor, hash_func hash_func, eq_func eq_func);
int  SET_add(Set *set, void *key);
int  SET_del(Set *set, void *key);
int  SET_contains(Set *set, void *key);
void SET_free(Set *set);
void SET_clear(Set *Set);

#endif /* SET_H_ */
