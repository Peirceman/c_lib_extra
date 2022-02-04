#include <vector.h>
#include <stdio.h>

int main(void) {
	vec_type(char*) fruits = NULL;

	vec_push(fruits, "apple");
	vec_push(fruits, "pear");
	vec_push(fruits, "orange");
	vec_push(fruits, "strawberry");
	vec_push(fruits, "mandarin");
	vec_push(fruits, "clementine");
	vec_push(fruits, "banana");
	 
	for(size_t i = 0; i < vec_sz(fruits); i++) printf("%s\n", fruits[i]);

	printf("fruits size: %zu\n", vec_sz(fruits));
	printf("fruits cap:  %zu\n", vec_cap(fruits));
	printf("fruits ptr:  %p\n", fruits);

	vec_free(fruits);

	vec_type(int) nums = NULL;
	vec_reserve(nums, 6);

	for (int i = 0; (size_t) i < vec_cap(nums); i++)
		vec_insert(nums, i, vec_sz(nums) / 2 + vec_sz(nums) % 2);

	for (size_t i = 0; i < vec_sz(nums); i++) printf("%d\n", nums[i]);

	printf("nums size: %zu\n", vec_sz(nums));
	printf("nums cap:  %zu\n", vec_cap(nums));
	printf("nums ptr:  %p\n", nums);
}
