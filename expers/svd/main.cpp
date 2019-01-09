#include <malgo/vector.h>
#include <malgo/matrix.h>
#include <malgo/nrecipes/gaussj.h>

#include <nos/print.h>

#include <malgo/echelon.h>
#include <malgo/algo/pseudosqr.h>
#include <malgo/algo/svd.h>

#include <malgo/nrecipes/svd.h>

int main()
{
	//auto m = malgo::matrix<double>{{1,1,2,1},{1,1,1,1},{1,1,1,1}};
	auto m = malgo::matrix<double>{{0,0,0},{0,1,0},{0,0,0},{0,0,0}};
	auto b = malgo::vector<double>{0,1,1,1};
	auto x = malgo::vector<double>(3);

	malgo::SVD<decltype(m)> svd(m);

	PRINT(svd.u);
	PRINT(svd.v);
	PRINT(svd.w);

	svd.solve(b,x);
	PRINT(b);
	PRINT(x);

	PRINT(svd.rank());

}