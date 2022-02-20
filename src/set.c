#include <stdlib.h>
#include <set.h>
#include <string.h>
#include <assert.h>

Set *SET_init(size_t key_sz, size_t entries_cap,
		double load_factor, hash_func hash_func, eq_func eq_func) {
	Set *set = malloc(sizeof(Set));

	if (!set) return NULL;

	if (!(set->entries = calloc(entries_cap, sizeof(Entry)))) {
		free(set->entries);
		return NULL;
	}

	set->count       = 0;
	set->entries_cap = entries_cap;
	set->load_factor = load_factor;
	set->key_sz      = key_sz;
	set->hash_func   = hash_func;
	set->eq_func     = eq_func;

	return set;
}

static void SET_grow(Set *set, size_t new_cap) {
	if (set->entries_cap >= new_cap) return;

	Entry **entries = calloc(new_cap, sizeof(Entry));

	if (!entries) {
		free(entries);
		return;
	}

	for (int i =0; i < set->entries_cap; i++) {
		Entry *e = set->entries[i];

		while (e) {
			unsigned int idx = (*set->hash_func)(e->key) % new_cap;

			e->next = entries[idx];
			entries[idx] = e;

			e = e->next;
		}
	}

	free(set->entries);
	set->entries_cap = new_cap;
	set->entries = entries;
}

int SET_add(Set *set, void *key) {
	unsigned int idx = (*set->hash_func)(key) % set->entries_cap;

	Entry *e = set->entries[idx];

	while (e) {
		if ((*set->eq_func)(key, e->key)) {
			return 1;
		}

		e = e->next;
	}

	e = malloc(sizeof(Entry));
	assert(e);

	e->key = malloc(set->key_sz);
	assert(e->key);
	memcpy(e->key, key, set->key_sz);
	e->next = set->entries[idx];
	set->entries[idx] = e;
	set->count++;

	if (set->count >= set->entries_cap * set->load_factor) {
		SET_grow(set, set->entries_cap * 2);
	}

	return 0;
}

int SET_del(Set *set, void *key) {
	unsigned int idx = (*set->hash_func)(key) % set->entries_cap;

	Entry *cur  = set->entries[idx];
	Entry *prev = NULL;

	while (cur) {
		if ((*set->eq_func)(key, cur->key)) {
			if (prev) {
				prev->next = cur->next;
			} else {
				set->entries[idx] = cur->next;
			}

			free(cur);
			free(cur->key);
			cur = NULL;
			set->count--;

			return 1;
		}

		prev = cur;
		cur  = cur->next;
	}

	return 0;
}

int SET_contains(Set *set, void *key) {
	unsigned int idx = (*set->hash_func)(key) % set->entries_cap;

	Entry*e = set->entries[idx];

	while (e) {
		if ((*set->eq_func)(key, e->key)) {
			return 1;
		}

		e = e->next;
	}

	return 0;
}

void SET_clear(Set *set) {
	for (int i = 0; i < set->entries_cap; i++) {
		Entry *e = set->entries[i];

		while (e) {
			Entry *next = e->next;

			free(e->key);
			free(e);

			e = next;
		}
	}
}

void SET_free(Set *set) {
	SET_clear(set);
	free(set->entries);
	free(set);
}

