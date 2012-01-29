/*
 * Copyright (c) 2012 by Internet Systems Consortium, Inc. ("ISC")
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.	IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <assert.h>

#define VECTOR_GENERATE(name, type)					\
typedef struct name##__vector {						\
	type *		_v;						\
	type *		_p;						\
	unsigned	_n, _n_alloced, _hint;				\
} name;									\
static inline name *							\
name##_init(unsigned hint)						\
{									\
	name *vec;							\
	vec = calloc(1, sizeof(name));					\
	assert(vec != NULL);						\
	if (hint == 0) hint = 1;					\
	vec->_hint = vec->_n_alloced = hint;				\
	vec->_v = malloc(vec->_n_alloced * sizeof(void *));		\
	assert(vec->_v != NULL);					\
	vec->_p = &(vec->_v[0]);					\
	return (vec);							\
}									\
static inline void							\
name##_detach(name *vec, type **out, size_t *outsz)			\
{									\
	*(out) = (vec)->_v;						\
	*(outsz) = (vec)->_n;						\
	(vec)->_n_alloced = (vec)->_hint;				\
	(vec)->_v = malloc((vec)->_n_alloced * sizeof(void *));		\
	assert((vec)->_v != NULL);					\
	(vec)->_p = &(vec->_v[0]);					\
}									\
static inline void							\
name##_destroy(name **vec)						\
{									\
	free((*vec)->_v);						\
	free((*vec));							\
	*vec = NULL;							\
}									\
static inline void							\
name##_add(name *vec, type elem)					\
{									\
	if ((vec)->_n == (vec)->_n_alloced - 1) {			\
		(vec)->_n_alloced *= 2;					\
		(vec)->_v = realloc((vec)->_v, (vec)->_n_alloced	\
				   * sizeof(void *));			\
		assert((vec)->_v != NULL);				\
	}								\
	(vec)->_v[(vec)->_n] = elem;					\
	(vec)->_n += 1;							\
	(vec)->_p = &((vec)->_v[(vec)->_n]);				\
}									\
static inline void							\
name##_need(name *vec, size_t n)					\
{									\
	while ((n) > ((vec)->_n_alloced - (vec)->_n)) {			\
		(vec)->_n_alloced *= 2;					\
		(vec)->_v = realloc((vec)->_v, (vec)->_n_alloced	\
				   * sizeof(void *));			\
		assert((vec)->_v != NULL);				\
	}								\
}									\
static inline void							\
name##_append(name *vec, const type *elems, size_t n)			\
{									\
	name##_need(vec, n);						\
	memcpy((vec)->_v + (vec)->_n, elems, (n)*sizeof(*elems));	\
	(vec)->_n += (n) / sizeof(type);				\
	(vec)->_p = &((vec)->_v[(vec)->_n]);				\
}									\
static inline void							\
name##_reset(name *vec)							\
{									\
	(vec)->_n = 0;							\
	if ((vec)->_n_alloced > (vec)->_hint) {				\
		(vec)->_n_alloced = (vec)->_hint;			\
		(vec)->_v = realloc((vec)->_v, (vec)->_n_alloced	\
				   * sizeof(void *));			\
		assert((vec)->_v != NULL);				\
	}								\
	(vec)->_p = &(vec->_v[0]);					\
}									\
static inline size_t							\
name##_bytes(name *vec)							\
{									\
	return ((vec)->_n * sizeof(type));				\
}									\
static inline size_t							\
name##_size(name *vec)							\
{									\
	return ((vec)->_n);						\
}									\
static inline type							\
name##_value(name *vec, size_t i)					\
{									\
	assert(i < (vec)->_n);						\
	return ((vec)->_v[i]);						\
}									\
static inline type *							\
name##_ptr(name *vec)							\
{									\
	return ((vec)->_p);						\
}									\
static inline void							\
name##_advance(name *vec, size_t x)					\
{									\
	assert(x < ((vec)->_n_alloced - (vec)->_n));			\
	(vec)->_n += x;							\
	(vec)->_p = &((vec)->_v[(vec)->_n]);				\
}