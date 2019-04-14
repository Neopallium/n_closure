
#ifndef __N_CLOSURE_H___
#define __N_CLOSURE_H___

#include <stdlib.h>
#include <stdio.h>

#ifndef N_ASSERT
#include <assert.h>
#define N_ASSERT(e) assert(e)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN64
// For Win64
// Warning: Trampoline functions can not touch the floating-point registers (XMM0-XMM3)
#define N_USER_PARAMS_DEF \
	void *p1, void *p2, void *p3, void *p4, void *p5, void *p6, \
	void *f1, void *f2, void *f3, void *f4, void *f5, void *f6

#define N_USER_PARAMS \
	p1, p2, p3, p4, p5, p6, f1, f2, f3, f4, f5, f6
#else
#define N_USER_PARAMS_DEF \
	void *p1, void *p2, void *p3, void *p4, void *p5, void *p6, \
	double f1, double f2, double f3, double f4, double f5, double f6

#define N_USER_PARAMS \
	p1, p2, p3, p4, p5, p6, f1, f2, f3, f4, f5, f6
#endif

#define N_TRAMPOLINE_PARAMS_DEF \
	int slot, N_USER_PARAMS_DEF
#define N_TRAMPOLINE_PARAMS \
	slot, N_USER_PARAMS

typedef void *(n_user_func)(N_USER_PARAMS_DEF);

typedef struct {
	n_user_func *func;
	int slot;
} n_closure;

#ifndef MAX_SLOTS
#define MAX_SLOTS 400
#endif

#if MAX_SLOTS > 1000
#error "Unsupported MAX_SLOTS > 1000"
#elif MAX_SLOTS > 900
#define SLOTS 1000
#define TR_FUNC_REPEAT TR_FUNC_A9
#elif MAX_SLOTS > 800
#define SLOTS 900
#define TR_FUNC_REPEAT TR_FUNC_A4 TR_FUNC_B(5) TR_FUNC_B(6) TR_FUNC_B(7) TR_FUNC_B(8)
#elif MAX_SLOTS > 700
#define SLOTS 800
#define TR_FUNC_REPEAT TR_FUNC_A4 TR_FUNC_B(5) TR_FUNC_B(6) TR_FUNC_B(7)
#elif MAX_SLOTS > 600
#define SLOTS 700
#define TR_FUNC_REPEAT TR_FUNC_A4 TR_FUNC_B(5) TR_FUNC_B(6)
#elif MAX_SLOTS > 500
#define SLOTS 600
#define TR_FUNC_REPEAT TR_FUNC_A4 TR_FUNC_B(5)
#elif MAX_SLOTS > 400
#define SLOTS 500
#define TR_FUNC_REPEAT TR_FUNC_A4
#elif MAX_SLOTS > 300
#define SLOTS 400
#define TR_FUNC_REPEAT TR_FUNC_B(0) TR_FUNC_B(1) TR_FUNC_B(2) TR_FUNC_B(3)
#elif MAX_SLOTS > 200
#define SLOTS 300
#define TR_FUNC_REPEAT TR_FUNC_B(0) TR_FUNC_B(1) TR_FUNC_B(2)
#elif MAX_SLOTS > 100
#define SLOTS 200
#define TR_FUNC_REPEAT TR_FUNC_B(0) TR_FUNC_B(1)
#else
#define SLOTS 100
#define TR_FUNC_REPEAT TR_FUNC_B(0)
#endif
#undef MAX_SLOTS
#define MAX_SLOTS SLOTS

static n_closure *g_closure_slots[MAX_SLOTS];
static int g_closure_next_slot = 0;
static int g_closure_freed_slots = 0;

static void *n_trampoline_func_handler(N_TRAMPOLINE_PARAMS_DEF) {
	n_closure *closure = g_closure_slots[slot];
	if (closure && closure->func) {
		return closure->func(N_USER_PARAMS);
	}
	return 0;
}

#define TR_FUNC_SLOT(a,b,c) (((a) * 100) + ((b) * 10) + c)
#define TR_FUNC_NAME(a,b,c) n_trampoline_func_ ## a ## b ## c

#define TR_FUNC_C(a,b) \
	TR_FUNC(a,b,0) TR_FUNC(a,b,1) TR_FUNC(a,b,2) TR_FUNC(a,b,3) TR_FUNC(a,b,4) \
	TR_FUNC(a,b,5) TR_FUNC(a,b,6) TR_FUNC(a,b,7) TR_FUNC(a,b,8) TR_FUNC(a,b,9)

#define TR_FUNC_B(a) \
	TR_FUNC_C(a,0) TR_FUNC_C(a,1) TR_FUNC_C(a,2) TR_FUNC_C(a,3) TR_FUNC_C(a,4) \
	TR_FUNC_C(a,5) TR_FUNC_C(a,6) TR_FUNC_C(a,7) TR_FUNC_C(a,8) TR_FUNC_C(a,9)

#define TR_FUNC_A4 TR_FUNC_B(0) TR_FUNC_B(1) TR_FUNC_B(2) TR_FUNC_B(3) TR_FUNC_B(4)
#define TR_FUNC_A9 TR_FUNC_A4 TR_FUNC_B(5) TR_FUNC_B(6) TR_FUNC_B(7) TR_FUNC_B(8) TR_FUNC_B(9)

#define TR_FUNC(a,b,c) \
	static void *TR_FUNC_NAME(a,b,c)(N_USER_PARAMS_DEF) { \
		int slot = TR_FUNC_SLOT(a,b,c); \
		return n_trampoline_func_handler(N_TRAMPOLINE_PARAMS); \
	}
TR_FUNC_REPEAT
#undef TR_FUNC

#define TR_FUNC(a,b,c) \
	TR_FUNC_NAME(a,b,c),
static n_user_func * const g_trampoline_slots[] = {
TR_FUNC_REPEAT
};
#undef TR_FUNC

static int n_closure_get_free_slot() {
	// try to re-used freed slots.
	if (g_closure_freed_slots > 0) {
		int max = g_closure_next_slot;
		for (int i = 0; i < max; i++) {
			if (g_closure_slots[i] == NULL) {
				g_closure_freed_slots--;
				return i;
			}
		}
	}
	int slot = g_closure_next_slot;
	if (slot >= MAX_SLOTS) return -1;
	g_closure_next_slot++;
	return slot;
}

#define n_closure_new(user_func, pclosure) n_closure_new_impl((void *)user_func, pclosure)
void *n_closure_new_impl(void *user_func, n_closure **pclosure) {
	n_closure *closure;
	// Get slot number
	int slot = n_closure_get_free_slot();
	printf("---- use slot=%d\n", slot);
	if (slot < 0) return 0;

	closure = (n_closure *)malloc(sizeof(n_closure));
	closure->func = (n_user_func *)user_func;
	closure->slot = slot;
	g_closure_slots[slot] = closure;
	if (pclosure) {
		*pclosure = closure;
	}
	// return trampoline function
	return (void *)g_trampoline_slots[slot];
}

#define n_closure_find_from_func(func) n_closure_find_from_func_impl((n_user_func *)func)
n_closure *n_closure_find_from_func_impl(n_user_func *func) {
	int max = g_closure_next_slot;
	for (int i = 0; i < max; i++) {
		if (g_trampoline_slots[i] == func) {
			return g_closure_slots[i];
		}
	}
	return NULL;
}

void n_closure_free(n_closure *closure) {
	N_ASSERT(closure);
	int slot = closure->slot;
	N_ASSERT(slot >= 0 && slot < MAX_SLOTS);
	g_closure_slots[slot] = NULL;
	g_closure_freed_slots++;
}

#ifdef __cplusplus
}
#endif

#endif // #ifndef __N_CLOSURE_H___

