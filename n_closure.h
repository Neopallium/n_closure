
#ifndef __N_CLOSURE_H___
#define __N_CLOSURE_H___

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
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

#ifndef MAX_SLOTS
#define MAX_SLOTS 400
#endif

#if MAX_SLOTS > 1000
#error "Unsupported MAX_SLOTS > 1000"
#elif MAX_SLOTS > 900
#define TR_FUNC_REPEAT TR_FUNC_A9
#elif MAX_SLOTS > 800
#define TR_FUNC_REPEAT TR_FUNC_A8
#elif MAX_SLOTS > 700
#define TR_FUNC_REPEAT TR_FUNC_A7
#elif MAX_SLOTS > 600
#define TR_FUNC_REPEAT TR_FUNC_A6
#elif MAX_SLOTS > 500
#define TR_FUNC_REPEAT TR_FUNC_A5
#elif MAX_SLOTS > 400
#define TR_FUNC_REPEAT TR_FUNC_A4
#elif MAX_SLOTS > 300
#define TR_FUNC_REPEAT TR_FUNC_A3
#elif MAX_SLOTS > 200
#define TR_FUNC_REPEAT TR_FUNC_A2
#elif MAX_SLOTS > 100
#define TR_FUNC_REPEAT TR_FUNC_A1
#else
#define TR_FUNC_REPEAT TR_FUNC_A0
#endif

static n_closure *g_closure_slots[MAX_SLOTS];
static int g_closure_next_slot = 0;

static void *n_trampoline_func_handler(N_TRAMPOLINE_PARAMS_DEF) {
	n_closure *closure = g_closure_slots[slot];
	if (closure && closure->func) {
		return closure->func(N_USER_PARAMS);
	}
	return 0;
}

#define TR_FUNC_SLOT(a,b,c) (((a) * 100) + ((b) * 10) + c)
#define TR_FUNC_NAME(a,b,c) n_trampoline_func_ ## a ## b ## c

#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define TR_FUNC_C(a,b) \
	TR_FUNC(a,b,0) TR_FUNC(a,b,1) TR_FUNC(a,b,2) TR_FUNC(a,b,3) TR_FUNC(a,b,4) \
	TR_FUNC(a,b,5) TR_FUNC(a,b,6) TR_FUNC(a,b,7) TR_FUNC(a,b,8) TR_FUNC(a,b,9)

#define TR_FUNC_B(a) \
	TR_FUNC_C(a,0) TR_FUNC_C(a,1) TR_FUNC_C(a,2) TR_FUNC_C(a,3) TR_FUNC_C(a,4) \
	TR_FUNC_C(a,5) TR_FUNC_C(a,6) TR_FUNC_C(a,7) TR_FUNC_C(a,8) TR_FUNC_C(a,9)

#define TR_FUNC_A0 TR_FUNC_B(0)
#define TR_FUNC_A1 TR_FUNC_A0 TR_FUNC_B(1)
#define TR_FUNC_A2 TR_FUNC_A1 TR_FUNC_B(2)
#define TR_FUNC_A3 TR_FUNC_A2 TR_FUNC_B(3)
#define TR_FUNC_A4 TR_FUNC_A3 TR_FUNC_B(4)
#define TR_FUNC_A5 TR_FUNC_A4 TR_FUNC_B(5)
#define TR_FUNC_A6 TR_FUNC_A5 TR_FUNC_B(6)
#define TR_FUNC_A7 TR_FUNC_A6 TR_FUNC_B(7)
#define TR_FUNC_A8 TR_FUNC_A7 TR_FUNC_B(8)
#define TR_FUNC_A9 TR_FUNC_A8 TR_FUNC_B(9)
#define TR_FUNC_A(a) TR_FUNC_A ## a

#define TR_FUNC(a,b,c) \
	static void *TR_FUNC_NAME(a,b,c)(N_USER_PARAMS_DEF) { \
		int slot = TR_FUNC_SLOT(a,b,c); \
		return n_trampoline_func_handler(N_TRAMPOLINE_PARAMS); \
	}
TR_FUNC_REPEAT
#undef TR_FUNC

#define TR_FUNC(a,b,c) \
	TR_FUNC_NAME(a,b,c),
static n_user_func g_trampoline_slots[] = {
TR_FUNC_REPEAT
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

