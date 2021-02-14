#include "gtest/gtest.h"
#include "../spline/spline.h"


TEST(SimpleTest, Test1) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(SplineTest, Test1) {
	const unsigned int N = 6;
	double x[N] = { 1, 2, 3, 4, 5, 6 };
	double y[N] = { 1.0002, 1.0341, 0.6, 0.40105, 0.1, 0.23975 };
	Spline s(N, x, y);
	for (unsigned int i = 0; i < N; i++)
		EXPECT_DOUBLE_EQ(s.calculate(x[i]), y[i]);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}