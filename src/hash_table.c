#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <hash_table.h>

Hash_table *HT_init(size_t key_sz, size_t val_sz, size_t entries_cap,
		double load_factor, hash_func hash_func, eq_func eq_func) {
	Hash_table *hash_table = malloc(sizeof(Hash_table));

	if (!hash_table) return NULL;

	if (!(hash_table->entries = calloc(entries_cap, sizeof(Entry)))) {
		free(hash_table->entries);
		return NULL;
	}


	hash_table->count       = 0;
	hash_table->entries_cap = entries_cap;
	hash_table->eq_func     = eq_func;
	hash_table->key_sz      = key_sz;
	hash_table->val_sz      = val_sz;
	hash_table->hash_func   = hash_func;
	hash_table->load_factor = load_factor;

	return hash_table;
}

void *HT_get(const Hash_table *hash_table, const void *key) {
	int idx = (*hash_table->hash_func)(key) % hash_table->entries_cap;

	Entry *e = hash_table->entries[idx];

	while (e) {
		if ((*hash_table->eq_func)(key, e->key)) {
			return e->val;
		}

		e = e->next;
	}

	return NULL;
}

void *HT_del(Hash_table *hash_table, const void *key) {
	int idx = (*hash_table->hash_func)(key) % hash_table->entries_cap;

	Entry *cur = hash_table->entries[idx];
	Entry *prev = NULL;

	while (cur) {
		if ((*hash_table->eq_func)(key, cur->key)) {
			void *val  = cur->val;

			if (prev) {
				prev->next = cur->next;
			} else {
				hash_table->entries[idx] = cur->next;
			}

			free(cur->key);
			free(cur);
			cur = NULL;
			hash_table->count--;

			return val;
			return NULL;
		}

		prev = cur;
		cur  = cur->next;
	}

	return NULL;
}

static void HT_grow(Hash_table *hash_table, size_t new_cap) {
	if (hash_table->entries_cap >= new_cap) return;

	Entry **entries = calloc(new_cap, sizeof(Entry));

	if (!entries) {
		free(entries);
		return;
	}

	for (int i = 0; i < hash_table->entries_cap; i++) {
		Entry *e = hash_table->entries[i];

		while (e) {
			int idx = (*hash_table->hash_func)(e->key) % new_cap;

			e->next = entries[idx];
			entries[idx] = e;

			e = e->next;
		}
	}

	free(hash_table->entries);
	hash_table->entries_cap = new_cap;
	hash_table->entries = entries;
}

void *HT_put(Hash_table *hash_table, const void *key, void *val) {
	int idx = (*hash_table->hash_func)(key) % hash_table->entries_cap;

	Entry *e = hash_table->entries[idx];

	while (e) {
		if ((*hash_table->eq_func)(key, e->key)) {
			void *old_val = e->val;
			e->val = val;
			return old_val;
		}

		e = e->next;
	}

	e = malloc(sizeof(Entry));
	assert(e);

	e->key = malloc(hash_table->key_sz);
	e->val = malloc(hash_table->val_sz);

	assert(e->key);
	assert(e->val);

	memcpy(e->key, key, hash_table->key_sz);
	memcpy(e->val, val, hash_table->val_sz);

	e->next = hash_table->entries[idx];
	hash_table->entries[idx] = e;
	hash_table->count++;

	if (hash_table->count >= hash_table->entries_cap * hash_table->load_factor) {
		HT_grow(hash_table, hash_table->entries_cap * 2);
	}

	return NULL;
}

void HT_clear(Hash_table *hash_table) {
	for (int i = 0; i < hash_table->entries_cap; i++) {
		Entry *e = hash_table->entries[i];

		while(e) {
			Entry *next = e->next;

			free(e->key);
			free(e->val);
			free(e);

			e = next;
		}
	}
}

void HT_free(Hash_table *hash_table) {
	HT_clear(hash_table);
	free(hash_table->entries);
	free(hash_table);
}

int HT_contains_key(const Hash_table *hash_table, const void *key) {
	int idx = (*hash_table->hash_func)(key) % hash_table->entries_cap;

	Entry *e = hash_table->entries[idx];

	while (e) {
		if ((*hash_table->eq_func)(key, e->key)) {
			return 1;
		}

		e = e->next;
	}

	return 0;
}
