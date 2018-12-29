#include <malgo/vector.h>
#include <malgo/matrix.h>
#include <malgo/basealgo.h>
#include <nos/print.h>

int main()
{
	double arr[3] = {0,1,2};

	malgo::vector<double> vec {0,1,2};
	malgo::vector<double> vec2(arr, 3);

	malgo::matrix<double, malgo::rowmajor> m {{3,4,5,7}, {5,6,7,8}, {2,3,5,1}};
	malgo::matrix<double, malgo::rowmajor> m2 {{3,4,5,7}, {5,6,7,8}, {2,3,5,2}};
	//malgo::matrix<double, malgo::row_order> m2(4,3);

	/*nos::println(vec2 * vec2);
	nos::println(malgo::length(vec2));
	nos::println(m.size1());
	nos::println(m.size2());
	nos::println(m);

	nos::println(vec2);

	malgo::raw::elwise_add(vec2.size(), vec2.data(), vec2.data(), vec2.data());
	malgo::raw::transpose(m.data(), 3, 4, m2.data());

	nos::println(m);
	nos::println(m2);*/

	nos::println(vec == vec2);
	nos::println(m == m);
	nos::println(m == m2);
}