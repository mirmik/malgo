#include <malgo/vector.h>
#include <malgo/matrix.h>
#include <malgo/basealgo.h>

#include <malgo/gaussj.h>
#include <malgo/svd.h>

#include <nos/print.h>

int main()
{
	/*double arr[3] = {0,1,2};

	malgo::vector<double> vec {0,1,2};
	malgo::vector<double> vec2(arr, 3);
	*/
	malgo::matrix<double> m {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
	malgo::matrix<double> sqm {{2, 1, 1}, {0, 1, 1}, {0, 0, 1}};
	/*malgo::matrix<double, malgo::colmajor> m2 {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}};
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

	//nos::println(*m.begin1());
	//nos::println(*m.begin2());

	//auto m2 = transpose(m);

	nos::println(m == m);

	for (auto it = m.begin1(); it != m.end1(); ++it)
	{
		for (auto it2 = it.begin2(); it2 != it.end2(); ++it2)
		{
			nos::print(*it2, " ");
		}
		nos::println();
	}

	auto m2 = transpose(m);

	for (auto it = m2.begin1(); it != m2.end1(); ++it)
	{
		for (auto it2 = it.begin2(); it2 != it.end2(); ++it2)
		{
			nos::print(*it2, " ");
		}
		nos::println();
	}

	auto m3 = inverse(sqm);
	nos::println(sqm);
	nos::println(m3);
	auto m4 = exponent(sqm);
	nos::println(sqm);
	nos::println(m4);

	malgo::gaussj(sqm);
	nos::println(sqm);
}