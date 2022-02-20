#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef VECTOR_H_
#define VECTOR_H_

#define vec_type(type) type *

#define vec_cap(vec)                                                                             \
	((vec) ? ((size_t *) (vec))[-1] : (size_t) 0)                                                \

#define vec_sz(vec)                                                                              \
	((vec) ? ((size_t *) (vec))[-2] : (size_t) 0)                                                \

#define vec_end(vec)                                                                             \
	((vec) ? &((vec)[vec_sz(vec)]) : NULL)                                                       \

#define vec_set_cap(vec, cap)                                                                    \
	do {                                                                                         \
		if ((vec)) {                                                                             \
			((size_t *) (vec))[-1] = (cap);                                                      \
		}                                                                                        \
	} while (0)                                                                                  \

#define vec_set_sz(vec, sz)                                                                      \
	do {                                                                                         \
		if ((vec)) {                                                                             \
			((size_t *) (vec))[-2] = (sz);                                                       \
		}                                                                                        \
	} while (0)                                                                                  \

#define vec_reserve(vec, cap)                                                                    \
	do {                                                                                         \
		if (vec_cap(vec) < (cap)) {                                                              \
			vec_grow((vec), (cap));                                                              \
		}                                                                                        \
	} while (0)                                                                                  \

#define vec_grow(vec, cap)                                                                       \
	do {                                                                                         \
		const size_t new_sz = (cap) * sizeof(*(vec)) + 2*sizeof(size_t);                         \
		if ((vec)) {                                                                             \
			size_t *old_vec = &((size_t *) (vec))[-2];                                           \
			size_t *new_vec = realloc(old_vec, new_sz);                                          \
			assert(new_vec);                                                                     \
			(vec) = (void *) (&new_vec[2]);                                                      \
			vec_set_cap((vec), (cap));                                                           \
		} else {                                                                                 \
			size_t *new_vec = malloc(new_sz);                                                    \
			assert(new_vec);                                                                     \
			(vec) = (void *) (&new_vec[2]);                                                      \
			vec_set_cap((vec), (cap));                                                           \
			vec_set_sz((vec), 0);                                                                \
		}                                                                                        \
	} while(0)                                                                                   \

#define vec_push(vec, val)                                                                       \
	do {                                                                                         \
		size_t sz = vec_sz(vec);                                                                 \
		size_t cap = vec_cap(vec);                                                               \
		if (sz >= cap) {                                                                         \
			vec_grow((vec), (cap ? cap * 2 : 1));                                                \
		}                                                                                        \
		(vec)[sz] = (val);                                                                       \
		vec_set_sz((vec), sz + 1);                                                               \
	} while(0)                                                                                   \

#define vec_insert(vec, val, idx)                                                                \
	do {                                                                                         \
		if (vec_cap(vec) <= (idx)) {                                                             \
			vec_grow((vec), vec_cap(vec) ? vec_cap(vec) * 2 : 1);                                \
		}                                                                                        \
		if ((idx) < vec_sz(vec)) {                                                               \
			memmove((vec) + (idx) + 1, (vec) + (idx), sizeof(*(vec)) * ((vec_sz(vec)) - (idx))); \
		}                                                                                        \
		(vec)[(idx)] = (val);                                                                      \
		vec_set_sz((vec), vec_sz(vec) + 1);                                                      \
	} while(0)                                                                                   \

#define vec_free(vec)                                                                            \
	do {                                                                                         \
		if ((vec)) {                                                                             \
			free(&((size_t *) (vec))[-2]);                                                       \
		}                                                                                        \
	} while(0)                                                                                   \

#endif /* VECTOR_H_ */
