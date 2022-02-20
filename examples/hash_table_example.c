#include <hash_table.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int int_hash(int *i) {
	return *i;
}

int int_eq(int *a, int *b) {
	return *a == *b;
}

int main(void) {
	Hash_table *ht = HT_init_default(int, int, &int_hash, &int_eq);

	printf("ht cap at start: %zu\n", ht->entries_cap);

	for (int i = 0; i < 20; i++) {
		HT_put(ht, &(int){i}, &(int){i + 2});
	}

	for (int i = 19; i >= 0; i--) {
		int *res = HT_get(ht, &(int){i});
		printf("%d + 2 = %d\n", i, *res);
	}

	printf("%d\n", *(int *) HT_get(ht, &(int){1}));
	printf("%d\n", *(int *) HT_get(ht, &(int){2}));
	printf("ht %s\n",
	 		(HT_contains_key(ht, &(int){4}) ?
	 		 "contains 4" : "not contains 4"));

	HT_del(ht, &(int){4});
	printf("ht %s\n",
	 		(HT_contains_key(ht, &(int){4}) ?
	 		 "contains 4" : "not contains 4"));

	printf("ht entry count: %zu\n", ht->count);
	printf("ht entry cap:   %zu\n", ht->entries_cap);

	printf("ht ptr:        %p\n", (void *) ht);
	printf("ht entry ptr:  %p\n", (void *) ht->entries);
	HT_free(ht);

	Hash_table *ht2 = HT_init(sizeof(int), sizeof(char *),
			11, 0.75,
			&int_hash, &int_eq);
	printf("ht2 ptr:       %p\n", (void *) ht2);
	printf("ht2 entry ptr: %p\n", (void *) ht2->entries);

	HT_put(ht2, &(int){1}, "1");
	HT_put(ht2, &(int){2}, "2");
	HT_put(ht2, &(int){3}, "3");
	HT_put(ht2, &(int){4}, "4");
	HT_put(ht2, &(int){5}, "5");

	HT_put(ht2, &(int){1}, "2");
	HT_put(ht2, &(int){2}, "1");
	HT_put(ht2, &(int){3}, "5");
	HT_put(ht2, &(int){4}, "3");
	HT_put(ht2, &(int){5}, "44");

	for (int i = 1; i < 6; i++) {
		printf("%d: %s\n", i, (char *) HT_get(ht2, &(int){i}));
	}

	return 0;
}
