TEST_FUNC2(int, int, long)
TEST_FUNC2(char, char, long)
TEST_FUNC2(long, long, long)
TEST_FUNC2(short, short, long)

TEST_FUNC2(int, float, long)
TEST_FUNC2(float, int, long)
TEST_FUNC2(float, float, long)
TEST_FUNC2(int, double, long)
TEST_FUNC2(double, int, long)
TEST_FUNC2(double, double, long)

TEST_FUNC2(intptr, double, long)
TEST_FUNC2(double, intptr, long)

TEST_FUNC4(int, int, int, int, long)
TEST_FUNC4(char, char, char, char, long)
TEST_FUNC4(long, long, long, long, long)
TEST_FUNC4(short, short, short, short, long)

TEST_FUNC4(int, int, float, float, long)
TEST_FUNC4(int, float, int, float, long)
TEST_FUNC4(float, float, float, float, long)
TEST_FUNC4(int, int, double, double, long)
TEST_FUNC4(int, double, int, double, long)
TEST_FUNC4(double, double, double, double, long)
TEST_FUNC4(double, double, double, double, double)

TEST_FUNC6(int, int, int, int, int, int, long)
TEST_FUNC6(char, char, char, char, char, char, long)
TEST_FUNC6(long, long, long, long, long, long, long)
TEST_FUNC6(short, short, short, short, short, short, long)

TEST_FUNC6(int, int, int, float, float, float, long)
TEST_FUNC6(int, float, int, float, int, float, long)
TEST_FUNC6(float, float, float, float, float, float, long)

TEST_FUNC6(int, int, int, double, double, double, long)
TEST_FUNC6(int, double, int, double, int, double, long)
TEST_FUNC6(double, double, double, double, double, double, long)
TEST_FUNC6(double, double, double, double, double, double, double)

