
#include <stdio.h>

#include "../n_closure.h"

typedef int (*cb_func)(int x, int y, double percent, void *userdata);

typedef struct {
	int counter;
} counter_state;

int test_func(int x, int y, double percent, void *userdata) {
	counter_state *state = (counter_state *)userdata;
	state->counter++;
	printf("test_func(%d, %d, %f, %p): counter=%d\n",
			x, y, percent, userdata, state->counter);
	return (x * percent) + (y * percent) + state->counter;
}

int main(int argc, char *argv[]) {
	counter_state state = {0};
	cb_func func1 = (cb_func)n_closure_new((void *)test_func);

	printf("rc = %d\n", func1(100, 200, 0.10, &state));
	printf("rc = %d\n", func1(123, 234, 0.12, &state));
	return 0;
}

