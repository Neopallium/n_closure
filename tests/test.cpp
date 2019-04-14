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

#define TEST_FUNC2_NUM \
	TEST_FUNC2(int, int, uint64_t) \
	TEST_FUNC2(char, char, uint64_t) \
	TEST_FUNC2(long, long, uint64_t) \
	TEST_FUNC2(short, short, uint64_t)

#define TEST_FUNC2_FLOAT \
	TEST_FUNC2(int, float, uint64_t) \
	TEST_FUNC2(float, int, uint64_t) \
	TEST_FUNC2(float, float, uint64_t)

#define TEST_FUNC2_DOUBLE \
	TEST_FUNC2(int, double, uint64_t) \
	TEST_FUNC2(double, int, uint64_t) \
	TEST_FUNC2(double, double, uint64_t)

#define TEST_FUNC2_PTR \
	TEST_FUNC2(intptr, double, uint64_t) \
	TEST_FUNC2(double, intptr, uint64_t)

#define TEST_FUNC2_ALL \
	TEST_FUNC2_NUM \
	TEST_FUNC2_FLOAT \
	TEST_FUNC2_DOUBLE \
	TEST_FUNC2_PTR

#define TEST_FUNC4_NUM \
	TEST_FUNC4(int, int, int, int, uint64_t) \
	TEST_FUNC4(char, char, char, char, uint64_t) \
	TEST_FUNC4(long, long, long, long, uint64_t) \
	TEST_FUNC4(short, short, short, short, uint64_t)

#define TEST_FUNC4_FLOAT \
	TEST_FUNC4(int, int, float, float, uint64_t) \
	TEST_FUNC4(int, float, int, float, uint64_t) \
	TEST_FUNC4(float, float, float, float, uint64_t)

#define TEST_FUNC4_DOUBLE \
	TEST_FUNC4(int, int, double, double, uint64_t) \
	TEST_FUNC4(int, double, int, double, uint64_t) \
	TEST_FUNC4(double, double, double, double, uint64_t) \
	TEST_FUNC4(double, double, double, double, double) \

#define TEST_FUNC4_ALL \
	TEST_FUNC4_NUM \
	TEST_FUNC4_FLOAT \
	TEST_FUNC4_DOUBLE

#define TEST_FUNC6_NUM \
	TEST_FUNC6(int, int, int, int, int, int, uint64_t) \
	TEST_FUNC6(char, char, char, char, char, char, uint64_t) \
	TEST_FUNC6(long, long, long, long, long, long, uint64_t) \
	TEST_FUNC6(short, short, short, short, short, short, uint64_t)

#define TEST_FUNC6_FLOAT \
	TEST_FUNC6(int, int, int, float, float, float, uint64_t) \
	TEST_FUNC6(int, float, int, float, int, float, uint64_t) \
	TEST_FUNC6(float, float, float, float, float, float, uint64_t)

#define TEST_FUNC6_DOUBLE \
	TEST_FUNC6(int, int, int, double, double, double, uint64_t) \
	TEST_FUNC6(int, double, int, double, int, double, uint64_t) \
	TEST_FUNC6(double, double, double, double, double, double, uint64_t) \
	TEST_FUNC6(double, double, double, double, double, double, double)

#define TEST_FUNC6_ALL \
	TEST_FUNC6_NUM \
	TEST_FUNC6_FLOAT \
	TEST_FUNC6_DOUBLE

#define TEST_FUNCS_ALL \
	TEST_FUNC2_ALL \
	TEST_FUNC4_ALL \
	TEST_FUNC6_ALL

#define TEST_EXPECT2(NAME, V1, V2) \
	EXPECT_EQ(impl_ ## NAME(V1, V2), func_ ## NAME(V1, V2))
#define TEST_EXPECT4(NAME, V1, V2, V3, V4) \
	EXPECT_EQ(impl_ ## NAME(V1, V2, V3, V4), func_ ## NAME(V1, V2, V3, V4))
#define TEST_EXPECT6(NAME, V1, V2, V3, V4, V5, V6) \
	EXPECT_EQ(impl_ ## NAME(V1, V2, V3, V4, V5, V6), func_ ## NAME(V1, V2, V3, V4, V5, V6))

#define TEST_CODE_CLEANUP(NAME) \
	static n_closure *closure_ ## NAME = NULL; \
	static cb_ ## NAME *func_ ## NAME; \
	static void test_cleanup_ ## NAME() { \
		n_closure *test_find = n_closure_find_from_func(func_ ## NAME); \
		EXPECT_EQ(test_find, closure_ ## NAME); \
		n_closure_free(closure_ ## NAME); \
		closure_ ## NAME = NULL; \
	}

#define TEST_CODE_REG(NAME) \
	func_ ## NAME = (cb_ ## NAME *)n_closure_new(impl_ ## NAME, &closure_ ## NAME);

#define TEST_CODE_FUNC2(NAME, T1, T2, RC) \
	typedef RC (cb_ ## NAME)(T1 p1, T2 p2); \
	static RC impl_ ## NAME(T1 p1, T2 p2) { \
		return ((RC)p1) + ((RC)p2) + (((RC)p1) / ((RC)p2)); \
	} \
	TEST_CODE_CLEANUP(NAME) \
	static void test_ ## NAME() { \
		TEST_CODE_REG(NAME) \
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
	typedef RC (cb_ ## NAME)(T1 p1, T2 p2, T3 p3, T4 p4); \
	static RC impl_ ## NAME(T1 p1, T2 p2, T3 p3, T4 p4) { \
		return ((RC)p1) + ((RC)p2) + ((RC)p3) + ((RC)p4) + (((RC)p1) / ((RC)p2)); \
	} \
	TEST_CODE_CLEANUP(NAME) \
	static void test_ ## NAME() { \
		TEST_CODE_REG(NAME) \
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
	typedef RC (cb_ ## NAME) \
		(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6); \
	static RC impl_ ## NAME\
		(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) { \
		return ((RC)p1) + ((RC)p2) + ((RC)p3) + ((RC)p4) + ((RC)p5) + ((RC)p6) + (((RC)p1) / ((RC)p2)); \
	} \
	TEST_CODE_CLEANUP(NAME) \
	static void test_ ## NAME() { \
		TEST_CODE_REG(NAME) \
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
TEST_FUNCS_ALL
#undef TEST_FUNC2
#undef TEST_FUNC4
#undef TEST_FUNC6

TEST(closureTest, func_2_parms) {
#define TEST_FUNC2(T1, T2, RC) \
	test_2_ ## T1 ## _ ## T2 ## _ ## RC();
TEST_FUNC2_ALL
#undef TEST_FUNC2

#define TEST_FUNC2(T1, T2, RC) \
	test_cleanup_2_ ## T1 ## _ ## T2 ## _ ## RC();
TEST_FUNC2_ALL
#undef TEST_FUNC2
}

TEST(closureTest, func_4_params) {
#define TEST_FUNC4(T1, T2, T3, T4, RC) \
	test_4_ ## T1 ## _ ## T2 ## _ ## T3 ## _ ## T4 ## _ ## RC();
TEST_FUNC4_ALL
#undef TEST_FUNC4

#define TEST_FUNC4(T1, T2, T3, T4, RC) \
	test_cleanup_4_ ## T1 ## _ ## T2 ## _ ## T3 ## _ ## T4 ## _ ## RC();
TEST_FUNC4_ALL
#undef TEST_FUNC4
}

TEST(closureTest, func_6_params) {
#define TEST_FUNC6(T1, T2, T3, T4, T5, T6, RC) \
	test_6_ ## T1 ## _ ## T2 ## _ ## T3 ## _ ## T4 ## _ ## T5 ## _ ## T6 ## _ ## RC();
TEST_FUNC6_ALL
#undef TEST_FUNC6

#define TEST_FUNC6(T1, T2, T3, T4, T5, T6, RC) \
	test_cleanup_6_ ## T1 ## _ ## T2 ## _ ## T3 ## _ ## T4 ## _ ## T5 ## _ ## T6 ## _ ## RC();
TEST_FUNC6_ALL
#undef TEST_FUNC6

}

