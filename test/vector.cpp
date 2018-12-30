#include <gtest/gtest.h>
#include <malgo/vector.h>

TEST(vector, add) {
	malgo::vector<double> a {1,2,3};
	malgo::vector<double> b {3,2,1};
	malgo::vector<double> c {4,4,4};
	EXPECT_EQ(a+b, c);
}