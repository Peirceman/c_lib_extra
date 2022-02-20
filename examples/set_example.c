#include <stdio.h>
#include <set.h>

unsigned int int_hash(int *a) {
	return *a;
}

int int_eq(int *a, int *b) {
	return *a == *b;
}

int main(void) {
	Set *even_nums = SET_init_default(int, &int_hash, &int_eq);

	printf("set cap at start: %zu\n", even_nums->entries_cap);

	for (int i = 0; i <= 10; i++) {
		SET_add(even_nums, &(int){i*2});
	}

	for (int i = 0; i <= 10*2; i++) {
		printf("%d %s\n", i,
				SET_contains(even_nums, &(int){i}) ? "is even" : "isn't even" );
	}

	printf("%d %s\n", 6, SET_contains(even_nums, &(int){6}) ? "is even" : "isn't even");
	printf("%d %s\n", 9, SET_contains(even_nums, &(int){9}) ? "is even" : "isn't even");

	SET_del(even_nums, &(int){6});

	printf("%d %s\n", 6, SET_contains(even_nums, &(int){6}) ? "is even" : "isn't even");

	printf("set entry count: %zu\n", even_nums->count);
	printf("set entry cap:   %zu\n", even_nums->entries_cap);

	printf("set ptr:         %p\n", (void *) even_nums);
	printf("set entry ptr:   %p\n", (void *) even_nums->entries);
	SET_free(even_nums);

	Set *set2 = SET_init(sizeof(int), 5,
		LOAD_FACTOR_NO_GROW, &int_hash, &int_eq);

	printf("\nset2 cap at start: %zu\n", set2->entries_cap);

	SET_add(set2, &(int){15});
	SET_add(set2, &(int){6});
	SET_add(set2, &(int){12});
	SET_add(set2, &(int){3});
	SET_add(set2, &(int){-6});

	printf("set2 entry count: %zu\n", set2->count);
	printf("set2 entry cap:   %zu\n", set2->entries_cap);

	printf("set2 ptr:         %p\n", (void *) set2);
	printf("set2 entry ptr:   %p\n", (void *) set2->entries);
}
