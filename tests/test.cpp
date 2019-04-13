#include <gtest/gtest.h>

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "../n_closure.h"

typedef int * intptr;

#define TEST2_FMT \
	"%" PRIu64 ",%" PRIu64
#define TEST4_FMT \
	TEST2_FMT "," TEST2_FMT
#define TEST6_FMT \
	TEST4_FMT "," TEST2_FMT

#define TEST_EXPECT2(NAME, V1, V2) \
	EXPECT_EQ(impl_ ## NAME(V1, V2), func(V1, V2))
#define TEST_EXPECT4(NAME, V1, V2, V3, V4) \
	EXPECT_EQ(impl_ ## NAME(V1, V2, V3, V4), func(V1, V2, V3, V4))
#define TEST_EXPECT6(NAME, V1, V2, V3, V4, V5, V6) \
	EXPECT_EQ(impl_ ## NAME(V1, V2, V3, V4, V5, V6), func(V1, V2, V3, V4, V5, V6))

#define TEST_CODE_FUNC2(NAME, T1, T2, RC) \
	typedef RC (*cb_ ## NAME)(T1 p1, T2 p2); \
	static RC impl_ ## NAME(T1 p1, T2 p2) { \
		return ((RC)p1) + ((RC)p2) + (((RC)p1) / ((RC)p2)); \
	} \
	static void test_ ## NAME() { \
		cb_ ## NAME func = (cb_ ## NAME)n_closure_new((void *)impl_ ## NAME); \
		uint64_t p1 = 1; uint64_t p2 = 2; \
		printf("test " #NAME "(" TEST2_FMT ")\n", p1, p2); \
		TEST_EXPECT2(NAME, (T1)p1, (T2)p2); \
 		p1 *= 10; p2 *= 100; \
		printf("test " #NAME "(" TEST2_FMT ")\n", p1, p2); \
		TEST_EXPECT2(NAME, (T1)p1, (T2)p2); \
		p1 /= 1000000; p2 /= 100000; \
		p1 += 1000000; p2 += 100000; \
		printf("test " #NAME "(" TEST2_FMT ")\n", p1, p2); \
		TEST_EXPECT2(NAME, (T1)p1, (T2)p2); \
	}
#define TEST_CODE_FUNC4(NAME, T1, T2, T3, T4, RC) \
	typedef RC (*cb_ ## NAME)(T1 p1, T2 p2, T3 p3, T4 p4); \
	static RC impl_ ## NAME(T1 p1, T2 p2, T3 p3, T4 p4) { \
		return ((RC)p1) + ((RC)p2) + ((RC)p3) + ((RC)p4) + (((RC)p1) / ((RC)p2)); \
	} \
	static void test_ ## NAME() { \
		cb_ ## NAME func = (cb_ ## NAME)n_closure_new((void *)impl_ ## NAME); \
		uint64_t p1 = 1; uint64_t p2 = 2; uint64_t p3 = 3; uint64_t p4 = 4; \
		printf("test " #NAME "(" TEST4_FMT ")\n", p1, p2, p3, p4); \
		TEST_EXPECT4(NAME, (T1)p1, (T2)p2, (T3)p3, (T4)p4); \
 		p1 *= 10; p2 *= 100; p3 *= 1000; p4 *= 10000; \
		printf("test " #NAME "(" TEST4_FMT ")\n", p1, p2, p3, p4); \
		TEST_EXPECT4(NAME, (T1)p1, (T2)p2, (T3)p3, (T4)p4); \
		p1 /= 1000000; p2 /= 100000; p3 /= 10000; p4 /= 1000; \
		p1 += 1000000; p2 += 100000; p3 += 10000; p4 += 1000; \
		printf("test " #NAME "(" TEST4_FMT ")\n", p1, p2, p3, p4); \
		TEST_EXPECT4(NAME, (T1)p1, (T2)p2, (T3)p3, (T4)p4); \
 }
#define TEST_CODE_FUNC6(NAME, T1, T2, T3, T4, T5, T6, RC) \
	typedef RC (*cb_ ## NAME) \
		(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6); \
	static RC impl_ ## NAME\
		(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) { \
		return ((RC)p1) + ((RC)p2) + ((RC)p3) + ((RC)p4) + ((RC)p5) + ((RC)p6) + (((RC)p1) / ((RC)p2)); \
	} \
	static void test_ ## NAME() { \
		cb_ ## NAME func = (cb_ ## NAME)n_closure_new((void *)impl_ ## NAME); \
		uint64_t p1 = 1; uint64_t p2 = 2; uint64_t p3 = 3; uint64_t p4 = 4; uint64_t p5 = 5; uint64_t p6 = 6; \
		printf("test " #NAME "(" TEST6_FMT ")\n", p1, p2, p3, p4, p5, p6); \
		TEST_EXPECT6(NAME, (T1)p1, (T2)p2, (T3)p3, (T4)p4, (T5)p5, (T6)p6); \
		p1 *= 10; p2 *= 100; p3 *= 1000; p4 *= 10000; p5 *= 100000; p6 *= 1000000; \
		printf("test " #NAME "(" TEST6_FMT ")\n", p1, p2, p3, p4, p5, p6); \
		TEST_EXPECT6(NAME, (T1)p1, (T2)p2, (T3)p3, (T4)p4, (T5)p5, (T6)p6); \
		p1 /= 1000000; p2 /= 100000; p3 /= 10000; p4 /= 1000; p5 /= 100; p6 /= 10; \
		p1 += 1000000; p2 += 100000; p3 += 10000; p4 += 1000; p5 += 100; p6 += 10; \
		printf("test " #NAME "(" TEST6_FMT ")\n", p1, p2, p3, p4, p5, p6); \
		TEST_EXPECT6(NAME, (T1)p1, (T2)p2, (T3)p3, (T4)p4, (T5)p5, (T6)p6); \
	}
#define TEST_FUNC2(T1, T2, RC) \
	TEST_CODE_FUNC2(2_ ## T1 ## _ ## T2 ## _ ## RC, T1, T2, RC)
#define TEST_FUNC4(T1, T2, T3, T4, RC) \
	TEST_CODE_FUNC4(4_ ## T1 ## _ ## T2 ## _ ## T3 ## _ ## T4 ## _ ## RC, \
			T1, T2, T3, T4, RC)
#define TEST_FUNC6(T1, T2, T3, T4, T5, T6, RC) \
	TEST_CODE_FUNC6(6_ ## T1 ## _ ## T2 ## _ ## T3 ## _ ## T4 ## _ ## T5 ## _ ## T6 ## _ ## RC, \
			T1, T2, T3, T4, T5, T6, RC)
#include "test_list.h"
#undef TEST_FUNC2
#undef TEST_FUNC4
#undef TEST_FUNC6

TEST(closureTest, basic) {
#define TEST_FUNC2(T1, T2, RC) \
	test_2_ ## T1 ## _ ## T2 ## _ ## RC();
#define TEST_FUNC4(T1, T2, T3, T4, RC) \
	test_4_ ## T1 ## _ ## T2 ## _ ## T3 ## _ ## T4 ## _ ## RC();
#define TEST_FUNC6(T1, T2, T3, T4, T5, T6, RC) \
	test_6_ ## T1 ## _ ## T2 ## _ ## T3 ## _ ## T4 ## _ ## T5 ## _ ## T6 ## _ ## RC();
#include "test_list.h"
#undef TEST_FUNC2
#undef TEST_FUNC4
#undef TEST_FUNC6
}

