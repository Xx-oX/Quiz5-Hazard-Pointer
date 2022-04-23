#ifndef HP_H
#define HP_H

/* shortcuts */
#define atomic_load(src) __atomic_load_n(src, __ATOMIC_SEQ_CST)
#define atomic_store(dst, val) __atomic_store(dst, val, __ATOMIC_SEQ_CST)
#define atomic_exchange(ptr, val) \
    __atomic_exchange_n(ptr, val, __ATOMIC_SEQ_CST)
#define atomic_cas(dst, expected, desired)                                 \
    __atomic_compare_exchange(dst, expected, desired, 0, __ATOMIC_SEQ_CST, \
                              __ATOMIC_SEQ_CST)

#include <stdint.h>

#define LIST_ITER(head, node) \
    for (node = atomic_load(head); node; node = atomic_load(&node->next))

typedef struct __hp {
    uintptr_t ptr;
    struct __hp *next;
} hp_t;

#define DEFER_DEALLOC 1

typedef struct {
    hp_t *pointers;
    hp_t *retired;
    void (*deallocator)(void *);
} domain_t;

domain_t *domain_new(void (*deallocator)(void *));
void domain_free(domain_t *);
uintptr_t load(domain_t *, const uintptr_t *);
void drop(domain_t *, uintptr_t);
static void cleanup_ptr(domain_t *, uintptr_t, int);
void swap(domain_t *, uintptr_t *, uintptr_t, int);
void cleanup(domain_t *, int);

#endif