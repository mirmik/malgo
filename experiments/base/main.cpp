#include <malgo/vector.h>
#include <malgo/matrix.h>
#include <nos/print.h>

int main()
{
	double arr[3] = {0,1,2};

	//malgo::vector<double> vec;
	malgo::vector<double> vec2(arr, 3);

	malgo::matrix<double, malgo::column_order> m {{3,4,5,7}, {5,6,7,8}, {2,3,5,1}};

	nos::println(vec2 * vec2);
	nos::println(malgo::length(vec2));
	nos::println(m.size1());
	nos::println(m.size2());
	nos::println(m);
}