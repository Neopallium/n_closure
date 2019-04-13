TEST_FUNC2(int, int, uint64_t)
TEST_FUNC2(char, char, uint64_t)
TEST_FUNC2(long, long, uint64_t)
TEST_FUNC2(short, short, uint64_t)

TEST_FUNC2(int, float, uint64_t)
TEST_FUNC2(float, int, uint64_t)
TEST_FUNC2(float, float, uint64_t)
TEST_FUNC2(int, double, uint64_t)
TEST_FUNC2(double, int, uint64_t)
TEST_FUNC2(double, double, uint64_t)

TEST_FUNC2(intptr, double, uint64_t)
TEST_FUNC2(double, intptr, uint64_t)

TEST_FUNC4(int, int, int, int, uint64_t)
TEST_FUNC4(char, char, char, char, uint64_t)
TEST_FUNC4(long, long, long, long, uint64_t)
TEST_FUNC4(short, short, short, short, uint64_t)

TEST_FUNC4(int, int, float, float, uint64_t)
TEST_FUNC4(int, float, int, float, uint64_t)
TEST_FUNC4(float, float, float, float, uint64_t)
TEST_FUNC4(int, int, double, double, uint64_t)
TEST_FUNC4(int, double, int, double, uint64_t)
TEST_FUNC4(double, double, double, double, uint64_t)
TEST_FUNC4(double, double, double, double, double)

TEST_FUNC6(int, int, int, int, int, int, uint64_t)
TEST_FUNC6(char, char, char, char, char, char, uint64_t)
TEST_FUNC6(long, long, long, long, long, long, uint64_t)
TEST_FUNC6(short, short, short, short, short, short, uint64_t)

TEST_FUNC6(int, int, int, float, float, float, uint64_t)
TEST_FUNC6(int, float, int, float, int, float, uint64_t)
TEST_FUNC6(float, float, float, float, float, float, uint64_t)

TEST_FUNC6(int, int, int, double, double, double, uint64_t)
TEST_FUNC6(int, double, int, double, int, double, uint64_t)
TEST_FUNC6(double, double, double, double, double, double, uint64_t)
TEST_FUNC6(double, double, double, double, double, double, double)

