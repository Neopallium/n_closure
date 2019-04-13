
#ifndef __N_CLOSURE_H___
#define __N_CLOSURE_H___

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
// For Win32/Win64
// Warning: Trampoline functions can not touch the floating-point registers (XMM0-XMM3)
#define N_USER_PARAMS_DEF \
	void *p1, void *p2, void *p3, void *p4, void *p5, void *p6, \
	void *f1, void *f2, void *f3, void *f4, void *f5, void *f6

#define N_USER_PARAMS \
	p1, p2, p3, p4, p5, p6, f1, f2, f3, f4, f5, f6

#define N_TRAMPOLINE_PARAMS_DEF \
	int slot, N_USER_PARAMS_DEF

#define N_TRAMPOLINE_PARAMS \
	slot, N_USER_PARAMS

#define N_USER_FMT \
	"%p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p"

#define N_TRAMPOLINE_FMT \
	"%d, " N_USER_FMT
#else
#define N_USER_PARAMS_DEF \
	void *p1, void *p2, void *p3, void *p4, void *p5, void *p6, \
	double f1, double f2, double f3, double f4, double f5, double f6

#define N_USER_PARAMS \
	p1, p2, p3, p4, p5, p6, f1, f2, f3, f4, f5, f6

#define N_TRAMPOLINE_PARAMS_DEF \
	int slot, N_USER_PARAMS_DEF

#define N_TRAMPOLINE_PARAMS \
	slot, N_USER_PARAMS

#define N_USER_FMT \
	"%p, %p, %p, %p, %p, %p, %f, %f, %f, %f, %f, %f"

#define N_TRAMPOLINE_FMT \
	"%d, " N_USER_FMT
#endif

typedef void *(*n_user_func)(N_USER_PARAMS_DEF);

typedef struct {
	n_user_func func;
} n_closure;

#define MAX_SLOTS 100

static n_closure *g_closure_slots[MAX_SLOTS];
static int g_closure_next_slot = 0;

static void *n_trampoline_func_handler(N_TRAMPOLINE_PARAMS_DEF) {
	n_closure *closure = g_closure_slots[slot];
	if (closure && closure->func) {
		return closure->func(N_USER_PARAMS);
	}
	return 0;
}

#define TR_FUNC(num) \
	static void *n_trampoline_func_ ## num(N_USER_PARAMS_DEF) { \
		int slot = num; \
		return n_trampoline_func_handler(N_TRAMPOLINE_PARAMS); \
	}
#include "funcs.h"
#undef TR_FUNC

#define TR_FUNC(num) \
	n_trampoline_func_ ## num,
static n_user_func g_trampoline_slots[] = {
#include "funcs.h"
};
#undef TR_FUNC

void *n_closure_new(void *user_func) {
	n_closure *closure;
	// Get next slot number
	int slot = g_closure_next_slot;
	if (slot >= MAX_SLOTS) return 0;
	g_closure_next_slot++;

	closure = (n_closure *)malloc(sizeof(n_closure));
	closure->func = (n_user_func)user_func;
	g_closure_slots[slot] = closure;
	// return trampoline function
	return (void *)g_trampoline_slots[slot];
}

#ifdef __cplusplus
}
#endif

#endif // #ifndef __N_CLOSURE_H___

