#include <malgo/vector.h>
#include <malgo/matrix.h>
#include <malgo/nrecipes/gaussj.h>

#include <nos/print.h>

int main()
{
	malgo::matrix<double> A {{1,1,1}, {0,1,1}, {0,0,1}};

	gaussj(A);
	nos::println(A);
}