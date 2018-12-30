#include <malgo/vector.h>
#include <malgo/matrix.h>
#include <malgo/basealgo.h>

#include <malgo/nrecipes/poly.h>
#include <malgo/nrecipes/gaussj.h>
#include <malgo/nrecipes/svd.h>

#include <nos/print.h>

int main()
{
/*	malgo::vector<double> u {4,4,4,0,0};
	malgo::vector<double> v {2,0,0,1};
	malgo::vector<double> q;
	malgo::vector<double> r;

	poldiv(u,v,q,r);

	PRINT(u);
	PRINT(v);
	PRINT(q);
	PRINT(r);*/

	malgo::vector<double> v {1,2,3};

	malgo::matrix<double> m {{1,2,3,4},{5,6,7,8},{9,10,11,12}};

	nos::println(m + m);
	nos::println(v * 3);
}