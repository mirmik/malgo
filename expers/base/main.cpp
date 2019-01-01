#include <malgo/vector.h>
#include <malgo/matrix.h>

#include <nos/print.h>

int main()
{
	malgo::matrix<double> A {{1,0,0}, {0,1,0}, {0,0,1}, {0,0,1}};
	malgo::matrix<double> B {{1,0,0,0}, {0,1,0,0}, {0,0,1,1}};

	PRINT(A);
	PRINT(B);

	nos::println(multiply(A, B));
	nos::println(multiply(B, A));
}